set netlistDestCores $env(BASROOT)/sdk/fpga/NutaqIPLib/pcores
set mmcmDir   $env(BASROOT)/sdk/fpga/NutaqIPLib/hdl/mmcm_calib

set srcCore1 $mmcmDir/mmcm_calib_e.vhd
set srcCore2 $mmcmDir/mmcm_calib_rtl_a.vhd
set srcCore3 $mmcmDir/mmcm_phase_calibration.vhd

set destcore $netlistDestCores/lyt_axi_mi250_v1_00_a

set vhdlRep $destcore/hdl/vhdl

file mkdir $vhdlRep

file copy -force $srcCore1 $vhdlRep
file copy -force $srcCore2 $vhdlRep
file copy -force $srcCore3 $vhdlRep


proc generate_corelevel_ucf {mhsinst} {


set  filePath [xget_ncf_dir $mhsinst]

   file mkdir    $filePath

   # specify file name
   set    instname   [xget_hw_parameter_value $mhsinst "INSTANCE"]
   set    name_lower [string      tolower    $instname]
   set    fileName   $name_lower
   append filePath   $fileName

   set    connector   [xget_hw_parameter_value $mhsinst "C_FMC_CONNECTOR"]

   # Find FPGA type
   #
   set xmpFileName [glob -type f *{.xmp}*]
   set xmpFile [open $xmpFileName "r"]
   set xmpFileStr [read $xmpFile]
   set data [split $xmpFileStr "\n"]

   set fpgatype 0
   foreach line $data {
        if [string match "Device: xc6vlx240t" $line] {
            set fpgatype 240
        }
        if [string match "Device: xc6vsx315t" $line] {
            set fpgatype 315
        }
        if [string match "Device: xc6vlx550t" $line] {
            set fpgatype 550
        }
        if [string match "Device: xc6vsx475t" $line] {
            set fpgatype 475
        }
    }

   close $xmpFile


   # Open UCF file for writing and delete XDC file (if any)
   set outputFileUcf [open "${filePath}_wrapper.ucf" "w"]
   file delete -force "${filePath}.xdc"

   puts "INFO: Setting timing constaints for ${instname}."

   # Constraints start here...
   #

    if [string match "0" $connector] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/IDELAYCTRL_inst\"                          IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk12_iodelaye1_inst\"                     IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk34_iodelaye1_inst\"                     IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk56_iodelaye1_inst\"                     IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk78_iodelaye1_inst\"                     IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i0_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i0_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i1_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i1_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i2_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i2_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i3_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i3_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC1_IODELAY_GRP;"
        
        if {$fpgatype==240 || $fpgatype==315} then {
            puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/bufr_inst_12\" LOC = BUFR_X0Y7;"
        }

        if {$fpgatype==475 || $fpgatype==550} then {
            puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/bufr_inst_12\" LOC = BUFR_X0Y11;"
        }
    }

    if [string match "1" $connector] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/IDELAYCTRL_inst\"                          IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk12_iodelaye1_inst\"                     IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk34_iodelaye1_inst\"                     IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk56_iodelaye1_inst\"                     IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/clk78_iodelaye1_inst\"                     IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i0_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i0_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i1_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i1_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i2_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i2_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i3_iobuff/data_2*.data2_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/i3_iobuff/data_1*.data1_iodelaye1_inst\"   IODELAY_GROUP = FMC2_IODELAY_GRP;"

        if {$fpgatype==240 || $fpgatype==315} then {
            # Not supported
        }
        
        if {$fpgatype==475 || $fpgatype==550} then {
            puts $outputFileUcf "INST \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/bufr_inst_12\" LOC = BUFR_X1Y7;"
            puts $outputFileUcf "PIN \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/ibufds_inst_12.O\" CLOCK_DEDICATED_ROUTE = FALSE;"
            puts $outputFileUcf "PIN \"*${instname}/mi250Top_u0/Mi250Ctrl_inst/io_buf_adc_12/bufr_inst_12.I\" CLOCK_DEDICATED_ROUTE = FALSE;"
        }

    }

   # Close the UCF file
   close $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]


}