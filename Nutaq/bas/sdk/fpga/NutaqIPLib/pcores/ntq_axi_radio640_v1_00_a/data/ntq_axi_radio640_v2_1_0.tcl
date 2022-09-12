proc generate_corelevel_ucf {mhsinst} {


set  filePath [xget_ncf_dir $mhsinst]

   file mkdir    $filePath

   # specify file name
   set    instname   [xget_hw_parameter_value $mhsinst "INSTANCE"]
   set    name_lower [string      tolower    $instname]
   set    fileName   $name_lower
   append filePath   $fileName

   set    connector    [xget_hw_parameter_value $mhsinst "C_FMC_CONNECTOR"]
   set    position     [xget_hw_parameter_value $mhsinst "C_BOARD_POSITION"]
   set    carrier      [string tolower [xget_hw_parameter_value $mhsinst "C_CARRIER"]]
   set    clock_master [xget_hw_parameter_value $mhsinst "C_CLOCK_MASTER"]

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
   
    puts $outputFileUcf "INST \"*${instname}/*/spiMosiPin_inst\"   IOB=TRUE;"
    puts $outputFileUcf "INST \"*${instname}/*.spiCsPin_inst\"     IOB=TRUE;"
    puts $outputFileUcf "INST \"*${instname}/*/spiClkOutPin_inst\" IOB=TRUE;"
    puts $outputFileUcf "INST \"*${instname}/*/spiMisoPin_inst\"   IOB=TRUE;"
    

    if {[string match "1" $connector] && [string match "1" $position]} {
        if {[string match "perseus601x" $carrier] || [string match "perseus611x" $carrier]} {
        
            puts $outputFileUcf "INST \"*${instname}/idelayctrl_inst\"      IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_rx_data_idelay\"   IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_data_delay\"       IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_refclk_idelay\"    IODELAY_GROUP = FMC_IODELAY_GRP;"
                
            puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" BEL = AFF;"
            
            if {[string match "1" $clock_master]} {
                if {$fpgatype==315 || $fpgatype==240} then {

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X1Y5;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_rbuf\"   LOC = BUFR_X1Y4;"

                    if {$fpgatype==240} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X64Y100;"
                    }
                    if {$fpgatype==315} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X88Y100;"
                    }

                }
                if {$fpgatype==475 || $fpgatype==550} then {

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X1Y9;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_rbuf\"   LOC = BUFR_X1Y8;"
                    
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y2;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_gbuf\"   LOC = BUFGCTRL_X0Y14;"

                    if {$fpgatype==475} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X88Y180;"
                    }
                    if {$fpgatype==550} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X100Y180;"
                    }
                }
            } else {
                if {$fpgatype==315 || $fpgatype==240} then {
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X1Y5;"
                }
                if {$fpgatype==475 || $fpgatype==550} then {
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X1Y9;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y2;"
                }
            }
        }
    }

    if {[string match "1" $connector] && [string match "2" $position]} {
        if {[string match "perseus601x" $carrier] || [string match "perseus611x" $carrier]} {

            puts $outputFileUcf "INST \"*${instname}/idelayctrl_inst\"      IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_rx_data_idelay\"   IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_data_delay\"       IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_refclk_idelay\"    IODELAY_GROUP = FMC_IODELAY_GRP;"
                
            puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" BEL = AFF;"
            if {[string match "1" $clock_master]} then {
                if {$fpgatype==315 || $fpgatype==240} then {

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X0Y5;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_rbuf\"   LOC = BUFR_X0Y4;"

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X0Y100;"

                }
                if {$fpgatype==475 || $fpgatype==550} then {

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X0Y9;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_rbuf\"   LOC = BUFR_X0Y8;"
                    
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y12;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_gbuf\"   LOC = BUFGCTRL_X0Y14;"

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X0Y180;"

                }
            } else {
                if {$fpgatype==315 || $fpgatype==240} then {
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X0Y5;"
                }
                if {$fpgatype==475 || $fpgatype==550} then {
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X0Y9;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y12;"
                }
            }            
        }

    }

    if {[string match "2" $connector] && [string match "1" $position]} {
        if [string match "perseus611x" $carrier] {

            puts $outputFileUcf "INST \"*${instname}/idelayctrl_inst\"      IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_rx_data_idelay\"   IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_data_delay\"       IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_refclk_idelay\"    IODELAY_GROUP = FMC_IODELAY_GRP;"
            
            puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" BEL = AFF;"
            if {[string match "1" $clock_master]} then {
                if {$fpgatype==475 || $fpgatype==550} then {

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X1Y11;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_rbuf\"   LOC = BUFR_X1Y10;"
                    
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y9;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_gbuf\"   LOC = BUFGCTRL_X0Y14;"

                    if {$fpgatype==475} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X88Y220;"
                    }
                    if {$fpgatype==550} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X100Y220;"
                    }
                }
            } else {
                if {$fpgatype==475 || $fpgatype==550} then {
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X0Y8;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y9;"
                }
            }
        }
    }

    if {[string match "2" $connector] && [string match "2" $position]} {
        if [string match "perseus611x" $carrier] {
            
            puts $outputFileUcf "INST \"*${instname}/idelayctrl_inst\"      IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_rx_data_idelay\"   IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_data_delay\"       IODELAY_GROUP = FMC_IODELAY_GRP;"
            puts $outputFileUcf "INST \"*${instname}/*/i_refclk_idelay\"    IODELAY_GROUP = FMC_IODELAY_GRP;"
            
            puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" BEL = AFF;"
            if {[string match "1" $clock_master]} then {
                if {$fpgatype==475 || $fpgatype==550} then {

                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X2Y5;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_rbuf\"   LOC = BUFR_X2Y4;"
                    
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y15;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_divclk_gbuf\"   LOC = BUFGCTRL_X0Y14;"                   

                    if {$fpgatype==475} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X116Y100;"
                    }
                    if {$fpgatype==550} {
                        puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/divclk_clr_R2_s\" LOC = SLICE_X128Y100;"
                    }

                }
            } else {
                if {$fpgatype==475 || $fpgatype==550} then {
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_rbuf\"      LOC = BUFR_X2Y5;"
                    puts $outputFileUcf "INST \"*${instname}/*/i_dev_if/i_clk/i_clk_gbuf\"      LOC = BUFGCTRL_X0Y15;"                    
                }
            }            
        }
    }

   # Close the UCF file
   close $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]

}
