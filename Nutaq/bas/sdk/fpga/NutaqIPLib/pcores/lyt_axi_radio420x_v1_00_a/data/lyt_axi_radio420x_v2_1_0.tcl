
# # Validate the Existence of BINGEN (For the binaryGenerator)
if {[info exists env(BINGEN)]} {
  set netlistDestCores $env(BinaryDir)/pcores
} else {
  set netlistDestCores $env(BASROOT)/sdk/fpga/NutaqIPLib/pcores
}

set destcore $netlistDestCores/lyt_axi_radio420x_v1_00_a

set ucfFile4 "lyt_axi_radio420x_ch2_lx240_sx315_v2_1_0.ucf" 
set ucfFile3 "lyt_axi_radio420x_ch2_lx550_sx475_v2_1_0.ucf" 
set ucfFile2 "lyt_axi_radio420x_ch1_lx240_sx315_v2_1_0.ucf" 
set ucfFile1 "lyt_axi_radio420x_ch1_lx550_sx475_v2_1_0.ucf" 

set dataSrc $destcore/data

set srcUcf4 $dataSrc/$ucfFile4 
set srcUcf3 $dataSrc/$ucfFile3 
set srcUcf2 $dataSrc/$ucfFile2 
set srcUcf1 $dataSrc/$ucfFile1 
set dstUcf ucf

file mkdir $dstUcf

file copy -force $srcUcf4 $dstUcf/$ucfFile4
file copy -force $srcUcf3 $dstUcf/$ucfFile3
file copy -force $srcUcf2 $dstUcf/$ucfFile2
file copy -force $srcUcf1 $dstUcf/$ucfFile1

proc generate_corelevel_ucf {mhsinst} {

    set    instname   [xget_hw_parameter_value $mhsinst "INSTANCE"]
    set    channel2   [xget_hw_parameter_value $mhsinst "C_FMCRADIO_CH2"]
    set    ipname     [xget_hw_option_value    $mhsinst "IPNAME"]
    set    name_lower [string   tolower   $instname]
    set    radioCh2   [string   tolower   $channel2]
    set    folderName [string   tolower   $instname]
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
    
    if [string match "0" $radioCh2] {
      set deviceFileName "./ucf/lyt_axi_radio420x_ch1_"
    } else {
      set deviceFileName "./ucf/lyt_axi_radio420x_ch2_"
    }
    
    foreach line $data {
        if [string match "Device: xc6vlx240t" $line] {
          append deviceFileName "lx240_sx315_v2_1_0.ucf"  
        } 
        if [string match "Device: xc6vsx315t" $line] {
          append deviceFileName "lx240_sx315_v2_1_0.ucf"  
        } 
        if [string match "Device: xc6vlx550t" $line] {
          append deviceFileName "lx550_sx475_v2_1_0.ucf"  
        }
        if [string match "Device: xc6vsx475t" $line] {   
          append deviceFileName "lx550_sx475_v2_1_0.ucf" 
        }
    }   
    
    set inputFile1 [open $deviceFileName "r"]
    
    
    puts $outputFile [read $inputFile1]
    
    # Close the file
    close $outputFile
    close $xmpFile
    close $inputFile1
    puts  [xget_ncf_loc_info $mhsinst]    
    

}