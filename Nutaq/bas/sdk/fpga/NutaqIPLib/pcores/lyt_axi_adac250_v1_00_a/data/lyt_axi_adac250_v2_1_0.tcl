
set netlistDestCores $env(BASROOT)/sdk/fpga/NutaqIPLib/pcores
set mmcmDir   $env(BASROOT)/sdk/fpga/NutaqIPLib/hdl/mmcm_calib


set destcore $netlistDestCores/lyt_axi_adac250_v1_00_a

set ucfFile2 "lyt_axi_adac250_lx240_sx315_v2_1_0.ucf" 
set ucfFile1 "lyt_axi_adac250_lx550_sx475_v2_1_0.ucf" 

set dataSrc $destcore/data

set srcUcf2 $dataSrc/$ucfFile2 
set srcUcf1 $dataSrc/$ucfFile1 
set dstUcf ucf

set srcCore5 $mmcmDir/mmcm_calib_e.vhd
set srcCore6 $mmcmDir/mmcm_calib_rtl_a.vhd
set srcCore7 $mmcmDir/mmcm_phase_calibration.vhd

set vhdlRep $destcore/hdl/vhdl

file mkdir $dstUcf

file copy -force $srcUcf2 $dstUcf/$ucfFile2
file copy -force $srcUcf1 $dstUcf/$ucfFile1

file copy -force $srcCore5 $vhdlRep
file copy -force $srcCore6 $vhdlRep
file copy -force $srcCore7 $vhdlRep

proc generate_corelevel_ucf {mhsinst} {

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
    
    foreach line $data {
        if [string match "Device: xc6vlx240t" $line] {
          set deviceFileName "./ucf/lyt_axi_adac250_lx240_sx315_v2_1_0.ucf"  
        } 
        if [string match "Device: xc6vsx315t" $line] {
          set deviceFileName "./ucf/lyt_axi_adac250_lx240_sx315_v2_1_0.ucf"  
        } 
        if [string match "Device: xc6vlx550t" $line] {
           set deviceFileName "./ucf/lyt_axi_adac250_lx550_sx475_v2_1_0.ucf"  
        }
        if [string match "Device: xc6vsx475t" $line] {   
           set deviceFileName "./ucf/lyt_axi_adac250_lx550_sx475_v2_1_0.ucf" 
       } }   
    
    set inputFile1 [open $deviceFileName "r"]
    
    
    puts $outputFile [read $inputFile1]
    
    # Close the file
    close $outputFile
    close $xmpFile
    close $inputFile1
    puts  [xget_ncf_loc_info $mhsinst]    
    

}