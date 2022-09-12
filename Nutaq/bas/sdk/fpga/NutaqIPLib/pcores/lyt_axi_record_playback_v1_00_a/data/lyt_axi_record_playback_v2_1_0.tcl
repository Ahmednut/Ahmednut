# Validate the Existence of BINGEN (For the binaryGenerator)
if {[info exists env(BINGEN)]} {
  set netlistDestCores $env(BinaryDir)/pcores
} else {
  set netlistDestCores $env(BASROOT)/sdk/fpga/NutaqIPLib/pcores
}

set ucfFile2 "lyt_axi_record_playback_lx240_sx315_v2_1_0.ucf" 
set ucfFile1 "lyt_axi_record_playback_lx550_sx475_v2_1_0.ucf" 
set ucfFile3 "lyt_axi_record_playback_lx240_v2_1_0.ucf" 
set ucfFile4 "lyt_axi_record_playback_sx315_v2_1_0.ucf" 
set ucfFile5 "lyt_axi_record_playback_lx550_v2_1_0.ucf" 
set ucfFile6 "lyt_axi_record_playback_sx475_v2_1_0.ucf" 

set destcore $netlistDestCores/lyt_axi_record_playback_v1_00_a

set dataSrc $destcore/data

set srcUcf2 $dataSrc/$ucfFile2 
set srcUcf1 $dataSrc/$ucfFile1 
set srcUcf3 $dataSrc/$ucfFile3 
set srcUcf4 $dataSrc/$ucfFile4 
set srcUcf5 $dataSrc/$ucfFile5 
set srcUcf6 $dataSrc/$ucfFile6 

set dstUcf ucf

file mkdir $dstUcf

file copy -force $srcUcf2 $dstUcf/$ucfFile2
file copy -force $srcUcf1 $dstUcf/$ucfFile1
file copy -force $srcUcf3 $dstUcf/$ucfFile3 
file copy -force $srcUcf4 $dstUcf/$ucfFile4 
file copy -force $srcUcf5 $dstUcf/$ucfFile5 
file copy -force $srcUcf6 $dstUcf/$ucfFile6 

proc generate_corelevel_ucf { mhsinst } {
    
    
    set    instname   [xget_hw_parameter_value $mhsinst "INSTANCE"]
    set    ipname     [xget_hw_option_value    $mhsinst "IPNAME"]
    set    name_lower [string   tolower   $instname]
    set    folderName [string   tolower $instname]
    append folderName "_wrapper"
    set    folderName implementation/$folderName 
    file   mkdir      $folderName    
    set    fileName   $name_lower
    append fileName   "_wrapper.ucf"
    set    filePath   $folderName/$fileName

    # Open a file for writing
    set    outputFile [open $filePath "w"]

    set xmpFileName [glob -type f *{.xmp}*]
    
    set xmpFile [open $xmpFileName "r"]
    
    set xmpFileStr [read $xmpFile]
    
    set data [split $xmpFileStr "\n"]
    
    set deviceFileName "" 
    set deviceFileName2 ""     
    
    foreach line $data {
        if [string match "Device: xc6vlx240t" $line] {
          set deviceFileName "./ucf/lyt_axi_record_playback_lx240_sx315_v2_1_0.ucf"
          set deviceFileName2 "./ucf/lyt_axi_record_playback_lx240_v2_1_0.ucf" 
        } 
        
        if [string match "Device: xc6vsx315t" $line] {
          set deviceFileName "./ucf/lyt_axi_record_playback_lx240_sx315_v2_1_0.ucf"  
          set deviceFileName2 "./ucf/lyt_axi_record_playback_sx315_v2_1_0.ucf"           
        } 
        if [string match "Device: xc6vlx550t" $line] {
          set deviceFileName "./ucf/lyt_axi_record_playback_lx550_sx475_v2_1_0.ucf" 
          set deviceFileName2 "./ucf/lyt_axi_record_playback_lx550_v2_1_0.ucf"               
        }
        if [string match "Device: xc6vsx475t" $line] {   
          set deviceFileName "./ucf/lyt_axi_record_playback_lx550_sx475_v2_1_0.ucf" 
          set deviceFileName2 "./ucf/lyt_axi_record_playback_sx475_v2_1_0.ucf"                 
       } }   
    
    set inputFile1 [open $deviceFileName "r"]
    set inputFile2 [open $deviceFileName2 "r"]    
    
    
    puts $outputFile [read $inputFile1]
    puts $outputFile [read $inputFile2]
    
    # Close the file
    close $outputFile
    close $xmpFile
    close $inputFile1
    close $inputFile2    
    puts  [xget_ncf_loc_info $mhsinst]

}
