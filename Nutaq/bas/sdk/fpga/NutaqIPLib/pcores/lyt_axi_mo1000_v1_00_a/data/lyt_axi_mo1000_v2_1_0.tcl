

proc generate_corelevel_ucf {mhsinst} {


set  filePath [xget_ncf_dir $mhsinst]

   file mkdir    $filePath

   # specify file name
   set    instname   [xget_hw_parameter_value $mhsinst "INSTANCE"]
   set    name_lower [string      tolower    $instname]
   set    fileName   $name_lower
   append filePath   $fileName

   set    position   [xget_hw_parameter_value $mhsinst "C_FMC_POSITION"]

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

    if [string match "0" $position] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/delay_ctrl_i\"                      IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[*\].iodelay_i\"    IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_dci_i\"                     IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_frame_i\"                   IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"

        # MI125 #0 (bottom) constraints
        puts $outputFileUcf "NET \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_rst_s_TIG\" TIG;"

        puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y4;"

        # Since the path from IBUFDS to BUFG is not clock dedicated (pin in outer column)
        # force it to be consistent from onebuild to another.
        if {$fpgatype==240 || $fpgatype==315} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y1;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X1Y5;"

            # LOC constraint for logic around SERDES to ease routing
            # All the following constraints are not necessary but will greatly help the routing
            # at high operating frequency (>250 MHz)

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_0\"                LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_1\"                LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_33\"       LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_33\"    LOC = SLICE_X0Y141;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_0\"                              LOC = SLICE_X0Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_1\"                              LOC = SLICE_X0Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_2\"                              LOC = SLICE_X0Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_3\"                              LOC = SLICE_X0Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_32\"       LOC = SLICE_X0Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_32\"    LOC = SLICE_X0Y139;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_0\"                            LOC = SLICE_X0Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_0\"                            LOC = SLICE_X0Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_0\"                            LOC = SLICE_X0Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_0\"                            LOC = SLICE_X0Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_0\"        LOC = SLICE_X0Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_0\"     LOC = SLICE_X0Y159;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_1\"                            LOC = SLICE_X0Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_1\"                            LOC = SLICE_X0Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_1\"                            LOC = SLICE_X0Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_1\"                            LOC = SLICE_X0Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_1\"        LOC = SLICE_X0Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_1\"     LOC = SLICE_X0Y125;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_2\"                            LOC = SLICE_X0Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_2\"                            LOC = SLICE_X0Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_2\"                            LOC = SLICE_X0Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_2\"                            LOC = SLICE_X0Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_2\"        LOC = SLICE_X0Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_2\"     LOC = SLICE_X0Y155;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_3\"                            LOC = SLICE_X0Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_3\"                            LOC = SLICE_X0Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_3\"                            LOC = SLICE_X0Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_3\"                            LOC = SLICE_X0Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_3\"        LOC = SLICE_X0Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_3\"     LOC = SLICE_X0Y149;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_4\"                            LOC = SLICE_X0Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_4\"                            LOC = SLICE_X0Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_4\"                            LOC = SLICE_X0Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_4\"                            LOC = SLICE_X0Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_4\"        LOC = SLICE_X0Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_4\"     LOC = SLICE_X0Y153;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_5\"                            LOC = SLICE_X0Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_5\"                            LOC = SLICE_X0Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_5\"                            LOC = SLICE_X0Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_5\"                            LOC = SLICE_X0Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_5\"        LOC = SLICE_X0Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_5\"     LOC = SLICE_X0Y137;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_6\"                            LOC = SLICE_X0Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_6\"                            LOC = SLICE_X0Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_6\"                            LOC = SLICE_X0Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_6\"                            LOC = SLICE_X0Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_6\"        LOC = SLICE_X0Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_6\"     LOC = SLICE_X0Y129;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_7\"                            LOC = SLICE_X0Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_7\"                            LOC = SLICE_X0Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_7\"                            LOC = SLICE_X0Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_7\"                            LOC = SLICE_X0Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_7\"        LOC = SLICE_X0Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_7\"     LOC = SLICE_X0Y145;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_8\"                            LOC = SLICE_X0Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_8\"                            LOC = SLICE_X0Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_8\"                            LOC = SLICE_X0Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_8\"                            LOC = SLICE_X0Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_8\"        LOC = SLICE_X0Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_8\"     LOC = SLICE_X0Y143;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_9\"                            LOC = SLICE_X0Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_9\"                            LOC = SLICE_X0Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_9\"                            LOC = SLICE_X0Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_9\"                            LOC = SLICE_X0Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_9\"        LOC = SLICE_X0Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_9\"     LOC = SLICE_X0Y133;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_10\"                           LOC = SLICE_X0Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_10\"                           LOC = SLICE_X0Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_10\"                           LOC = SLICE_X0Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_10\"                           LOC = SLICE_X0Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_10\"       LOC = SLICE_X0Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_10\"    LOC = SLICE_X0Y147;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_11\"                           LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_11\"                           LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_11\"                           LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_11\"                           LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_11\"       LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_11\"    LOC = SLICE_X0Y135;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_12\"                           LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_12\"                           LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_12\"                           LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_12\"                           LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_12\"       LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_12\"    LOC = SLICE_X0Y127;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_13\"                           LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_13\"                           LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_13\"                           LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_13\"                           LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_13\"       LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_13\"    LOC = SLICE_X0Y199;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_14\"                           LOC = SLICE_X0Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_14\"                           LOC = SLICE_X0Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_14\"                           LOC = SLICE_X0Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_14\"                           LOC = SLICE_X0Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_14\"       LOC = SLICE_X0Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_14\"    LOC = SLICE_X0Y157;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_15\"                           LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_15\"                           LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_15\"                           LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_15\"                           LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_15\"       LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_15\"    LOC = SLICE_X0Y181;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_0\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_0\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_0\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_0\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_16\"       LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_16\"    LOC = SLICE_X0Y169;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_1\"                            LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_1\"                            LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_1\"                            LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_1\"                            LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_17\"       LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_17\"    LOC = SLICE_X0Y179;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_2\"                            LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_2\"                            LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_2\"                            LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_2\"                            LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_18\"       LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_18\"    LOC = SLICE_X0Y161;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_3\"                            LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_3\"                            LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_3\"                            LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_3\"                            LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_19\"       LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_19\"    LOC = SLICE_X0Y177;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_4\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_4\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_4\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_4\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_20\"       LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_20\"    LOC = SLICE_X0Y165;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_5\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_5\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_5\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_5\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_21\"       LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_21\"    LOC = SLICE_X0Y187;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_6\"                            LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_6\"                            LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_6\"                            LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_6\"                            LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_22\"       LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_22\"    LOC = SLICE_X0Y189;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_7\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_7\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_7\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_7\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_23\"       LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_23\"    LOC = SLICE_X0Y173;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_8\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_8\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_8\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_8\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_24\"       LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_24\"    LOC = SLICE_X0Y183;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_9\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_9\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_9\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_9\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_25\"       LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_25\"    LOC = SLICE_X0Y185;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_10\"                           LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_10\"                           LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_10\"                           LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_10\"                           LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_26\"       LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_26\"    LOC = SLICE_X0Y195;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_11\"                           LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_11\"                           LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_11\"                           LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_11\"                           LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_27\"       LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_27\"    LOC = SLICE_X0Y193;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_12\"                           LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_12\"                           LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_12\"                           LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_12\"                           LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_28\"       LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_28\"    LOC = SLICE_X0Y197;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_29\"       LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_29\"    LOC = SLICE_X0Y163;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_14\"                           LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_14\"                           LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_14\"                           LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_14\"                           LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_30\"       LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_30\"    LOC = SLICE_X0Y175;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_15\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_15\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_15\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_15\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_31\"       LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_31\"    LOC = SLICE_X0Y167;"

        }
        if {$fpgatype==475 || $fpgatype==550} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X1Y9;"

            if {$fpgatype==550} then {
                puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y16;"

                puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_s\""
                puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;25912672!-1;-58264;3104;S!0;875;64!1;37;18!2;\""
                puts $outputFileUcf "\"48;126!3;2555;1990!3;72;-693!4;1314;2!5;-39;-2083!6;-2122;7260!7;42;22!8;\""
                puts $outputFileUcf "\"0;12800!9;145;-38;L!10;4228;5003!12;3508;7797!13;3998;7928!14;4306;11272!\""
                puts $outputFileUcf "\"15;15646;8592!16;15832;532!17;923;-2;L!}\";"
            }

            if {$fpgatype==475} then {
                puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y1;"
            }

            # LOC constraint for logic around SERDES to ease routing
            # All the following constraints are not necessary but will greatly help the routing
            # at high operating frequency (>250 MHz)

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_0\"                LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_1\"                LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_33\"       LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_33\"    LOC = SLICE_X0Y221;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_0\"                              LOC = SLICE_X0Y219;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_1\"                              LOC = SLICE_X0Y219;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_2\"                              LOC = SLICE_X0Y219;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_3\"                              LOC = SLICE_X0Y219;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_32\"       LOC = SLICE_X0Y219;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_32\"    LOC = SLICE_X0Y219;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_0\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_0\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_0\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_0\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_0\"        LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_0\"     LOC = SLICE_X0Y239;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_1\"                            LOC = SLICE_X0Y205;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_1\"                            LOC = SLICE_X0Y205;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_1\"                            LOC = SLICE_X0Y205;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_1\"                            LOC = SLICE_X0Y205;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_1\"        LOC = SLICE_X0Y205;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_1\"     LOC = SLICE_X0Y205;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_2\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_2\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_2\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_2\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_2\"        LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_2\"     LOC = SLICE_X0Y235;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_3\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_3\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_3\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_3\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_3\"        LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_3\"     LOC = SLICE_X0Y229;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_4\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_4\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_4\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_4\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_4\"        LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_4\"     LOC = SLICE_X0Y233;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_5\"                            LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_5\"                            LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_5\"                            LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_5\"                            LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_5\"        LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_5\"     LOC = SLICE_X0Y217;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_6\"                            LOC = SLICE_X0Y209;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_6\"                            LOC = SLICE_X0Y209;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_6\"                            LOC = SLICE_X0Y209;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_6\"                            LOC = SLICE_X0Y209;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_6\"        LOC = SLICE_X0Y209;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_6\"     LOC = SLICE_X0Y209;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_7\"                            LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_7\"                            LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_7\"                            LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_7\"                            LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_7\"        LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_7\"     LOC = SLICE_X0Y225;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_8\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_8\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_8\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_8\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_8\"        LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_8\"     LOC = SLICE_X0Y223;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_9\"                            LOC = SLICE_X0Y213;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_9\"                            LOC = SLICE_X0Y213;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_9\"                            LOC = SLICE_X0Y213;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_9\"                            LOC = SLICE_X0Y213;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_9\"        LOC = SLICE_X0Y213;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_9\"     LOC = SLICE_X0Y213;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_10\"                           LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_10\"                           LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_10\"                           LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_10\"                           LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_10\"       LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_10\"    LOC = SLICE_X0Y227;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_11\"                           LOC = SLICE_X0Y215;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_11\"                           LOC = SLICE_X0Y215;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_11\"                           LOC = SLICE_X0Y215;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_11\"                           LOC = SLICE_X0Y215;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_11\"       LOC = SLICE_X0Y215;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_11\"    LOC = SLICE_X0Y215;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_12\"                           LOC = SLICE_X0Y207;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_12\"                           LOC = SLICE_X0Y207;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_12\"                           LOC = SLICE_X0Y207;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_12\"                           LOC = SLICE_X0Y207;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_12\"       LOC = SLICE_X0Y207;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_12\"    LOC = SLICE_X0Y207;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_13\"                           LOC = SLICE_X0Y279;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_13\"                           LOC = SLICE_X0Y279;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_13\"                           LOC = SLICE_X0Y279;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_13\"                           LOC = SLICE_X0Y279;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_13\"       LOC = SLICE_X0Y279;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_13\"    LOC = SLICE_X0Y279;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_14\"                           LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_14\"                           LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_14\"                           LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_14\"                           LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_14\"       LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_14\"    LOC = SLICE_X0Y237;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_15\"                           LOC = SLICE_X0Y261;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_15\"                           LOC = SLICE_X0Y261;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_15\"                           LOC = SLICE_X0Y261;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_15\"                           LOC = SLICE_X0Y261;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_15\"       LOC = SLICE_X0Y261;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_15\"    LOC = SLICE_X0Y261;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_0\"                            LOC = SLICE_X0Y249;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_0\"                            LOC = SLICE_X0Y249;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_0\"                            LOC = SLICE_X0Y249;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_0\"                            LOC = SLICE_X0Y249;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_16\"       LOC = SLICE_X0Y249;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_16\"    LOC = SLICE_X0Y249;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_1\"                            LOC = SLICE_X0Y259;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_1\"                            LOC = SLICE_X0Y259;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_1\"                            LOC = SLICE_X0Y259;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_1\"                            LOC = SLICE_X0Y259;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_17\"       LOC = SLICE_X0Y259;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_17\"    LOC = SLICE_X0Y259;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_2\"                            LOC = SLICE_X0Y241;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_2\"                            LOC = SLICE_X0Y241;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_2\"                            LOC = SLICE_X0Y241;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_2\"                            LOC = SLICE_X0Y241;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_18\"       LOC = SLICE_X0Y241;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_18\"    LOC = SLICE_X0Y241;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_3\"                            LOC = SLICE_X0Y257;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_3\"                            LOC = SLICE_X0Y257;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_3\"                            LOC = SLICE_X0Y257;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_3\"                            LOC = SLICE_X0Y257;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_19\"       LOC = SLICE_X0Y257;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_19\"    LOC = SLICE_X0Y257;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_4\"                            LOC = SLICE_X0Y245;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_4\"                            LOC = SLICE_X0Y245;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_4\"                            LOC = SLICE_X0Y245;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_4\"                            LOC = SLICE_X0Y245;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_20\"       LOC = SLICE_X0Y245;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_20\"    LOC = SLICE_X0Y245;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_5\"                            LOC = SLICE_X0Y267;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_5\"                            LOC = SLICE_X0Y267;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_5\"                            LOC = SLICE_X0Y267;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_5\"                            LOC = SLICE_X0Y267;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_21\"       LOC = SLICE_X0Y267;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_21\"    LOC = SLICE_X0Y267;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_6\"                            LOC = SLICE_X0Y269;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_6\"                            LOC = SLICE_X0Y269;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_6\"                            LOC = SLICE_X0Y269;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_6\"                            LOC = SLICE_X0Y269;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_22\"       LOC = SLICE_X0Y269;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_22\"    LOC = SLICE_X0Y269;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_7\"                            LOC = SLICE_X0Y253;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_7\"                            LOC = SLICE_X0Y253;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_7\"                            LOC = SLICE_X0Y253;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_7\"                            LOC = SLICE_X0Y253;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_23\"       LOC = SLICE_X0Y253;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_23\"    LOC = SLICE_X0Y253;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_8\"                            LOC = SLICE_X0Y263;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_8\"                            LOC = SLICE_X0Y263;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_8\"                            LOC = SLICE_X0Y263;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_8\"                            LOC = SLICE_X0Y263;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_24\"       LOC = SLICE_X0Y263;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_24\"    LOC = SLICE_X0Y263;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_9\"                            LOC = SLICE_X0Y265;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_9\"                            LOC = SLICE_X0Y265;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_9\"                            LOC = SLICE_X0Y265;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_9\"                            LOC = SLICE_X0Y265;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_25\"       LOC = SLICE_X0Y265;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_25\"    LOC = SLICE_X0Y265;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_10\"                           LOC = SLICE_X0Y275;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_10\"                           LOC = SLICE_X0Y275;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_10\"                           LOC = SLICE_X0Y275;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_10\"                           LOC = SLICE_X0Y275;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_26\"       LOC = SLICE_X0Y275;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_26\"    LOC = SLICE_X0Y275;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_11\"                           LOC = SLICE_X0Y273;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_11\"                           LOC = SLICE_X0Y273;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_11\"                           LOC = SLICE_X0Y273;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_11\"                           LOC = SLICE_X0Y273;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_27\"       LOC = SLICE_X0Y273;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_27\"    LOC = SLICE_X0Y273;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_12\"                           LOC = SLICE_X0Y277;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_12\"                           LOC = SLICE_X0Y277;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_12\"                           LOC = SLICE_X0Y277;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_12\"                           LOC = SLICE_X0Y277;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_28\"       LOC = SLICE_X0Y277;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_28\"    LOC = SLICE_X0Y277;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_13\"                           LOC = SLICE_X0Y243;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_13\"                           LOC = SLICE_X0Y243;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_13\"                           LOC = SLICE_X0Y243;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_13\"                           LOC = SLICE_X0Y243;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_29\"       LOC = SLICE_X0Y243;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_29\"    LOC = SLICE_X0Y243;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_14\"                           LOC = SLICE_X0Y255;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_14\"                           LOC = SLICE_X0Y255;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_14\"                           LOC = SLICE_X0Y255;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_14\"                           LOC = SLICE_X0Y255;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_30\"       LOC = SLICE_X0Y255;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_30\"    LOC = SLICE_X0Y255;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_15\"                           LOC = SLICE_X0Y247;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_15\"                           LOC = SLICE_X0Y247;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_15\"                           LOC = SLICE_X0Y247;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_15\"                           LOC = SLICE_X0Y247;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_31\"       LOC = SLICE_X0Y247;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_31\"    LOC = SLICE_X0Y247;"

        }

    }

    if [string match "1" $position] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/delay_ctrl_i\"                      IODELAY_GROUP = FMC1_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[*\].iodelay_i\"    IODELAY_GROUP = FMC1_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_dci_i\"                     IODELAY_GROUP = FMC1_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_frame_i\"                   IODELAY_GROUP = FMC1_HPC_IODELAY_GRP;"

        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[28\].iodelay_i\"   IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[30\].iodelay_i\"   IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"

        # MI125 #0 (bottom) constraints
        puts $outputFileUcf "NET \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_rst_s_TIG\" TIG;"
        puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y9;"
        puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y10;"

        # Since the path from IBUFDS to BUFG is not clock dedicated (pin in outer column)
        # force it to be consistent from onebuild to another.
        if {$fpgatype==240 || $fpgatype==315} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y5;"

            # LOC constraint for logic around SERDES to ease routing
            # All the following constraints are not necessary but will greatly help the routing
            # at high operating frequency (>250 MHz)

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_0\"                LOC = SLICE_X0Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_1\"                LOC = SLICE_X0Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_33\"       LOC = SLICE_X0Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_33\"    LOC = SLICE_X0Y101;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_0\"                              LOC = SLICE_X0Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_1\"                              LOC = SLICE_X0Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_2\"                              LOC = SLICE_X0Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_3\"                              LOC = SLICE_X0Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_32\"       LOC = SLICE_X0Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_32\"    LOC = SLICE_X0Y117;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_0\"                            LOC = SLICE_X0Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_0\"                            LOC = SLICE_X0Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_0\"                            LOC = SLICE_X0Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_0\"                            LOC = SLICE_X0Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_0\"        LOC = SLICE_X0Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_0\"     LOC = SLICE_X0Y119;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_1\"                            LOC = SLICE_X0Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_1\"                            LOC = SLICE_X0Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_1\"                            LOC = SLICE_X0Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_1\"                            LOC = SLICE_X0Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_1\"        LOC = SLICE_X0Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_1\"     LOC = SLICE_X0Y113;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_2\"                            LOC = SLICE_X0Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_2\"                            LOC = SLICE_X0Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_2\"                            LOC = SLICE_X0Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_2\"                            LOC = SLICE_X0Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_2\"        LOC = SLICE_X0Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_2\"     LOC = SLICE_X0Y115;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_3\"                            LOC = SLICE_X0Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_3\"                            LOC = SLICE_X0Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_3\"                            LOC = SLICE_X0Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_3\"                            LOC = SLICE_X0Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_3\"        LOC = SLICE_X0Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_3\"     LOC = SLICE_X0Y107;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_4\"                            LOC = SLICE_X0Y93; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_4\"                            LOC = SLICE_X0Y93; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_4\"                            LOC = SLICE_X0Y93; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_4\"                            LOC = SLICE_X0Y93; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_4\"        LOC = SLICE_X0Y93; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_4\"     LOC = SLICE_X0Y93; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_5\"                            LOC = SLICE_X0Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_5\"                            LOC = SLICE_X0Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_5\"                            LOC = SLICE_X0Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_5\"                            LOC = SLICE_X0Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_5\"        LOC = SLICE_X0Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_5\"     LOC = SLICE_X0Y105;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_6\"                            LOC = SLICE_X0Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_6\"                            LOC = SLICE_X0Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_6\"                            LOC = SLICE_X0Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_6\"                            LOC = SLICE_X0Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_6\"        LOC = SLICE_X0Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_6\"     LOC = SLICE_X0Y103;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_7\"                            LOC = SLICE_X0Y85; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_7\"                            LOC = SLICE_X0Y85; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_7\"                            LOC = SLICE_X0Y85; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_7\"                            LOC = SLICE_X0Y85; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_7\"        LOC = SLICE_X0Y85; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_7\"     LOC = SLICE_X0Y85; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_8\"                            LOC = SLICE_X0Y95; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_8\"                            LOC = SLICE_X0Y95; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_8\"                            LOC = SLICE_X0Y95; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_8\"                            LOC = SLICE_X0Y95; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_8\"        LOC = SLICE_X0Y95; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_8\"     LOC = SLICE_X0Y95; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_9\"                            LOC = SLICE_X0Y89; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_9\"                            LOC = SLICE_X0Y89; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_9\"                            LOC = SLICE_X0Y89; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_9\"                            LOC = SLICE_X0Y89; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_9\"        LOC = SLICE_X0Y89; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_9\"     LOC = SLICE_X0Y89; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_10\"                           LOC = SLICE_X0Y97; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_10\"                           LOC = SLICE_X0Y97; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_10\"                           LOC = SLICE_X0Y97; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_10\"                           LOC = SLICE_X0Y97; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_10\"       LOC = SLICE_X0Y97; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_10\"    LOC = SLICE_X0Y97; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_11\"                           LOC = SLICE_X0Y87; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_11\"                           LOC = SLICE_X0Y87; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_11\"                           LOC = SLICE_X0Y87; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_11\"                           LOC = SLICE_X0Y87; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_11\"       LOC = SLICE_X0Y87; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_11\"    LOC = SLICE_X0Y87; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_12\"                           LOC = SLICE_X0Y61; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_12\"                           LOC = SLICE_X0Y61; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_12\"                           LOC = SLICE_X0Y61; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_12\"                           LOC = SLICE_X0Y61; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_12\"       LOC = SLICE_X0Y61; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_12\"    LOC = SLICE_X0Y61; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_13\"                           LOC = SLICE_X0Y83; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_13\"                           LOC = SLICE_X0Y83; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_13\"                           LOC = SLICE_X0Y83; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_13\"                           LOC = SLICE_X0Y83; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_13\"       LOC = SLICE_X0Y83; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_13\"    LOC = SLICE_X0Y83; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_14\"                           LOC = SLICE_X0Y43; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_14\"                           LOC = SLICE_X0Y43; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_14\"                           LOC = SLICE_X0Y43; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_14\"                           LOC = SLICE_X0Y43; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_14\"       LOC = SLICE_X0Y43; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_14\"    LOC = SLICE_X0Y43; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_15\"                           LOC = SLICE_X0Y81; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_15\"                           LOC = SLICE_X0Y81; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_15\"                           LOC = SLICE_X0Y81; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_15\"                           LOC = SLICE_X0Y81; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_15\"       LOC = SLICE_X0Y81; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_15\"    LOC = SLICE_X0Y81; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_0\"                            LOC = SLICE_X0Y47; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_0\"                            LOC = SLICE_X0Y47; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_0\"                            LOC = SLICE_X0Y47; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_0\"                            LOC = SLICE_X0Y47; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_16\"       LOC = SLICE_X0Y47; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_16\"    LOC = SLICE_X0Y47; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_1\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_1\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_1\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_1\"                            LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_17\"       LOC = SLICE_X0Y235;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_17\"    LOC = SLICE_X0Y235;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_2\"                            LOC = SLICE_X0Y55; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_2\"                            LOC = SLICE_X0Y55; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_2\"                            LOC = SLICE_X0Y55; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_2\"                            LOC = SLICE_X0Y55; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_18\"       LOC = SLICE_X0Y55; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_18\"    LOC = SLICE_X0Y55; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_3\"                            LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_3\"                            LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_3\"                            LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_3\"                            LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_19\"       LOC = SLICE_X0Y237;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_19\"    LOC = SLICE_X0Y237;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_4\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_4\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_4\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_4\"                            LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_20\"       LOC = SLICE_X0Y239;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_20\"    LOC = SLICE_X0Y239;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_5\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_5\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_5\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_5\"                            LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_21\"       LOC = SLICE_X0Y223;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_21\"    LOC = SLICE_X0Y223;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_6\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_6\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_6\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_6\"                            LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_22\"       LOC = SLICE_X0Y229;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_22\"    LOC = SLICE_X0Y229;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_7\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_7\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_7\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_7\"                            LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_23\"       LOC = SLICE_X0Y233;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_23\"    LOC = SLICE_X0Y233;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_8\"                            LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_8\"                            LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_8\"                            LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_8\"                            LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_24\"       LOC = SLICE_X0Y227;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_24\"    LOC = SLICE_X0Y227;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_9\"                            LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_9\"                            LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_9\"                            LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_9\"                            LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_25\"       LOC = SLICE_X0Y221;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_25\"    LOC = SLICE_X0Y221;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_10\"                           LOC = SLICE_X0Y99; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_10\"                           LOC = SLICE_X0Y99; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_10\"                           LOC = SLICE_X0Y99; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_10\"                           LOC = SLICE_X0Y99; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_26\"       LOC = SLICE_X0Y99; "
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_26\"    LOC = SLICE_X0Y99; "

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_11\"                           LOC = SLICE_X0Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_11\"                           LOC = SLICE_X0Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_11\"                           LOC = SLICE_X0Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_11\"                           LOC = SLICE_X0Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_27\"       LOC = SLICE_X0Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_27\"    LOC = SLICE_X0Y109;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_12\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_12\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_12\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_12\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_28\"       LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_28\"    LOC = SLICE_X0Y123;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_13\"                           LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_13\"                           LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_13\"                           LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_13\"                           LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_29\"       LOC = SLICE_X0Y225;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_29\"    LOC = SLICE_X0Y225;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_14\"                           LOC = SLICE_X0Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_14\"                           LOC = SLICE_X0Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_14\"                           LOC = SLICE_X0Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_14\"                           LOC = SLICE_X0Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_30\"       LOC = SLICE_X0Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_30\"    LOC = SLICE_X0Y121;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_15\"                           LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_15\"                           LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_15\"                           LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_15\"                           LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_31\"       LOC = SLICE_X0Y217;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_31\"    LOC = SLICE_X0Y217;"

        }
        if {$fpgatype==475 || $fpgatype==550} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y9;"

            if {$fpgatype==550} then {
                puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_s\""
                puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;b8ccc5a9!-1;-304322;383496;S!0;875;64!1;37;18!\""
                puts $outputFileUcf "\"2;48;126!3;2903;1990!4;1662;2!5;2106;-537!5;1352;-4096!6;7550;-3559!7;\""
                puts $outputFileUcf "\"4052;-27688!8;4052;-27688!9;0;-51448!10;35153;-26547!11;0;-52224!12;\""
                puts $outputFileUcf "\"67302;0!13;0;-51200!14;32501;-24901!15;0;-51448!16;0;-52224!17;0;-52224!\""
                puts $outputFileUcf "\"18;0;-51200!19;0;-51448!20;0;-51448!21;290;-31356!22;9159;-27537!23;245;\""
                puts $outputFileUcf "\"-8311!24;11701;-3399!25;-3833;-2694!26;4052;-27688!27;-3768;647!28;36780;\""
                puts $outputFileUcf "\"-27100!29;-2759;-1039!30;41277;2675!31;76;-647;L!32;23297;-3475!34;15828;\""
                puts $outputFileUcf "\"-988!35;923;3062;L!}\";"
            }

            # LOC constraint for logic around SERDES to ease routing
            # All the following constraints are not necessary but will greatly help the routing
            # at high operating frequency (>250 MHz)

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_0\"                LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_1\"                LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_33\"       LOC = SLICE_X0Y181;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_33\"    LOC = SLICE_X0Y181;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_0\"                              LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_1\"                              LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_2\"                              LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_3\"                              LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_32\"       LOC = SLICE_X0Y197;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_32\"    LOC = SLICE_X0Y197;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_0\"                            LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_0\"                            LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_0\"                            LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_0\"                            LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_0\"        LOC = SLICE_X0Y199;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_0\"     LOC = SLICE_X0Y199;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_1\"                            LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_1\"                            LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_1\"                            LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_1\"                            LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_1\"        LOC = SLICE_X0Y193;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_1\"     LOC = SLICE_X0Y193;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_2\"                            LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_2\"                            LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_2\"                            LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_2\"                            LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_2\"        LOC = SLICE_X0Y195;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_2\"     LOC = SLICE_X0Y195;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_3\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_3\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_3\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_3\"                            LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_3\"        LOC = SLICE_X0Y187;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_3\"     LOC = SLICE_X0Y187;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_4\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_4\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_4\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_4\"                            LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_4\"        LOC = SLICE_X0Y173;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_4\"     LOC = SLICE_X0Y173;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_5\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_5\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_5\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_5\"                            LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_5\"        LOC = SLICE_X0Y185;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_5\"     LOC = SLICE_X0Y185;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_6\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_6\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_6\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_6\"                            LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_6\"        LOC = SLICE_X0Y183;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_6\"     LOC = SLICE_X0Y183;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_7\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_7\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_7\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_7\"                            LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_7\"        LOC = SLICE_X0Y165;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_7\"     LOC = SLICE_X0Y165;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_8\"                            LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_8\"                            LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_8\"                            LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_8\"                            LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_8\"        LOC = SLICE_X0Y175;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_8\"     LOC = SLICE_X0Y175;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_9\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_9\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_9\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_9\"                            LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_9\"        LOC = SLICE_X0Y169;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_9\"     LOC = SLICE_X0Y169;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_10\"                           LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_10\"                           LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_10\"                           LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_10\"                           LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_10\"       LOC = SLICE_X0Y177;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_10\"    LOC = SLICE_X0Y177;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_11\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_11\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_11\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_11\"                           LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_11\"       LOC = SLICE_X0Y167;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_11\"    LOC = SLICE_X0Y167;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_12\"                           LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_12\"                           LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_12\"                           LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_12\"                           LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_12\"       LOC = SLICE_X0Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_12\"    LOC = SLICE_X0Y141;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_13\"                           LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_13\"       LOC = SLICE_X0Y163;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_13\"    LOC = SLICE_X0Y163;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_14\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_14\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_14\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_14\"                           LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_14\"       LOC = SLICE_X0Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_14\"    LOC = SLICE_X0Y123;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_15\"                           LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_15\"                           LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_15\"                           LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_15\"                           LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_15\"       LOC = SLICE_X0Y161;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_15\"    LOC = SLICE_X0Y161;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_0\"                            LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_0\"                            LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_0\"                            LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_0\"                            LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_16\"       LOC = SLICE_X0Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_16\"    LOC = SLICE_X0Y127;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_1\"                            LOC = SLICE_X0Y315;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_1\"                            LOC = SLICE_X0Y315;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_1\"                            LOC = SLICE_X0Y315;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_1\"                            LOC = SLICE_X0Y315;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_17\"       LOC = SLICE_X0Y315;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_17\"    LOC = SLICE_X0Y315;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_2\"                            LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_2\"                            LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_2\"                            LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_2\"                            LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_18\"       LOC = SLICE_X0Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_18\"    LOC = SLICE_X0Y135;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_3\"                            LOC = SLICE_X0Y317;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_3\"                            LOC = SLICE_X0Y317;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_3\"                            LOC = SLICE_X0Y317;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_3\"                            LOC = SLICE_X0Y317;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_19\"       LOC = SLICE_X0Y317;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_19\"    LOC = SLICE_X0Y317;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_4\"                            LOC = SLICE_X0Y319;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_4\"                            LOC = SLICE_X0Y319;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_4\"                            LOC = SLICE_X0Y319;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_4\"                            LOC = SLICE_X0Y319;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_20\"       LOC = SLICE_X0Y319;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_20\"    LOC = SLICE_X0Y319;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_5\"                            LOC = SLICE_X0Y303;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_5\"                            LOC = SLICE_X0Y303;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_5\"                            LOC = SLICE_X0Y303;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_5\"                            LOC = SLICE_X0Y303;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_21\"       LOC = SLICE_X0Y303;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_21\"    LOC = SLICE_X0Y303;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_6\"                            LOC = SLICE_X0Y309;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_6\"                            LOC = SLICE_X0Y309;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_6\"                            LOC = SLICE_X0Y309;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_6\"                            LOC = SLICE_X0Y309;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_22\"       LOC = SLICE_X0Y309;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_22\"    LOC = SLICE_X0Y309;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_7\"                            LOC = SLICE_X0Y313;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_7\"                            LOC = SLICE_X0Y313;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_7\"                            LOC = SLICE_X0Y313;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_7\"                            LOC = SLICE_X0Y313;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_23\"       LOC = SLICE_X0Y313;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_23\"    LOC = SLICE_X0Y313;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_8\"                            LOC = SLICE_X0Y307;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_8\"                            LOC = SLICE_X0Y307;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_8\"                            LOC = SLICE_X0Y307;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_8\"                            LOC = SLICE_X0Y307;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_24\"       LOC = SLICE_X0Y307;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_24\"    LOC = SLICE_X0Y307;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_9\"                            LOC = SLICE_X0Y301;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_9\"                            LOC = SLICE_X0Y301;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_9\"                            LOC = SLICE_X0Y301;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_9\"                            LOC = SLICE_X0Y301;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_25\"       LOC = SLICE_X0Y301;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_25\"    LOC = SLICE_X0Y301;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_10\"                           LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_10\"                           LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_10\"                           LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_10\"                           LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_26\"       LOC = SLICE_X0Y179;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_26\"    LOC = SLICE_X0Y179;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_11\"                           LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_11\"                           LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_11\"                           LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_11\"                           LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_27\"       LOC = SLICE_X0Y189;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_27\"    LOC = SLICE_X0Y189;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_12\"                           LOC = SLICE_X0Y203;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_12\"                           LOC = SLICE_X0Y203;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_12\"                           LOC = SLICE_X0Y203;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_12\"                           LOC = SLICE_X0Y203;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_28\"       LOC = SLICE_X0Y203;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_28\"    LOC = SLICE_X0Y203;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_13\"                           LOC = SLICE_X0Y305;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_13\"                           LOC = SLICE_X0Y305;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_13\"                           LOC = SLICE_X0Y305;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_13\"                           LOC = SLICE_X0Y305;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_29\"       LOC = SLICE_X0Y305;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_29\"    LOC = SLICE_X0Y305;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_14\"                           LOC = SLICE_X0Y201;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_14\"                           LOC = SLICE_X0Y201;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_14\"                           LOC = SLICE_X0Y201;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_14\"                           LOC = SLICE_X0Y201;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_30\"       LOC = SLICE_X0Y201;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_30\"    LOC = SLICE_X0Y201;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_15\"                           LOC = SLICE_X0Y297;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_15\"                           LOC = SLICE_X0Y297;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_15\"                           LOC = SLICE_X0Y297;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_15\"                           LOC = SLICE_X0Y297;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_31\"       LOC = SLICE_X0Y297;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_31\"    LOC = SLICE_X0Y297;"

        }

    }

    if [string match "2" $position] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/delay_ctrl_i\"                      IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[*\].iodelay_i\"    IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_dci_i\"                     IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_frame_i\"                   IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"

        # MI125 #0 (bottom) constraints
        puts $outputFileUcf "NET \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_rst_s_TIG\" TIG;"

        if {$fpgatype==550} then {
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC=BUFR_X1Y8;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC=BUFGCTRL_X0Y19;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC=BUFGCTRL_X0Y14;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;2d6b140a!-1;-58264;-4320;S!0;875;64!1;37;18!2;\""
            puts $outputFileUcf "\"48;126!3;84;1977!3;2555;1990!4;-49;2591!5;1314;2!6;44;-24!7;1308;3412!8;\""
            puts $outputFileUcf "\"141;40;L!9;4096;29828!11;4196;27784!12;4563;6273!13;15381;2607!14;15852;\""
            puts $outputFileUcf "\"-1256!15;923;274;L!}\";"
        }

        if {$fpgatype==475 || $fpgatype==550} then {
            # LOC constraint for logic around SERDES to ease routing
            # All the following constraints are not necessary but will greatly help the routing
            # at high operating frequency (>250 MHz)

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_0\"                LOC = SLICE_X127Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_1\"                LOC = SLICE_X127Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_33\"       LOC = SLICE_X127Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_33\"    LOC = SLICE_X127Y141;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_0\"                              LOC = SLICE_X127Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_1\"                              LOC = SLICE_X127Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_2\"                              LOC = SLICE_X127Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_3\"                              LOC = SLICE_X127Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_32\"       LOC = SLICE_X127Y139;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_32\"    LOC = SLICE_X127Y139;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_0\"                            LOC = SLICE_X127Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_0\"                            LOC = SLICE_X127Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_0\"                            LOC = SLICE_X127Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_0\"                            LOC = SLICE_X127Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_0\"        LOC = SLICE_X127Y159;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_0\"     LOC = SLICE_X127Y159;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_1\"                            LOC = SLICE_X127Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_1\"                            LOC = SLICE_X127Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_1\"                            LOC = SLICE_X127Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_1\"                            LOC = SLICE_X127Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_1\"        LOC = SLICE_X127Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_1\"     LOC = SLICE_X127Y125;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_2\"                            LOC = SLICE_X127Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_2\"                            LOC = SLICE_X127Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_2\"                            LOC = SLICE_X127Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_2\"                            LOC = SLICE_X127Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_2\"        LOC = SLICE_X127Y155;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_2\"     LOC = SLICE_X127Y155;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_3\"                            LOC = SLICE_X127Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_3\"                            LOC = SLICE_X127Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_3\"                            LOC = SLICE_X127Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_3\"                            LOC = SLICE_X127Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_3\"        LOC = SLICE_X127Y149;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_3\"     LOC = SLICE_X127Y149;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_4\"                            LOC = SLICE_X127Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_4\"                            LOC = SLICE_X127Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_4\"                            LOC = SLICE_X127Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_4\"                            LOC = SLICE_X127Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_4\"        LOC = SLICE_X127Y153;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_4\"     LOC = SLICE_X127Y153;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_5\"                            LOC = SLICE_X127Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_5\"                            LOC = SLICE_X127Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_5\"                            LOC = SLICE_X127Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_5\"                            LOC = SLICE_X127Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_5\"        LOC = SLICE_X127Y137;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_5\"     LOC = SLICE_X127Y137;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_6\"                            LOC = SLICE_X127Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_6\"                            LOC = SLICE_X127Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_6\"                            LOC = SLICE_X127Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_6\"                            LOC = SLICE_X127Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_6\"        LOC = SLICE_X127Y129;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_6\"     LOC = SLICE_X127Y129;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_7\"                            LOC = SLICE_X127Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_7\"                            LOC = SLICE_X127Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_7\"                            LOC = SLICE_X127Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_7\"                            LOC = SLICE_X127Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_7\"        LOC = SLICE_X127Y145;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_7\"     LOC = SLICE_X127Y145;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_8\"                            LOC = SLICE_X127Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_8\"                            LOC = SLICE_X127Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_8\"                            LOC = SLICE_X127Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_8\"                            LOC = SLICE_X127Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_8\"        LOC = SLICE_X127Y143;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_8\"     LOC = SLICE_X127Y143;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_9\"                            LOC = SLICE_X127Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_9\"                            LOC = SLICE_X127Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_9\"                            LOC = SLICE_X127Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_9\"                            LOC = SLICE_X127Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_9\"        LOC = SLICE_X127Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_9\"     LOC = SLICE_X127Y133;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_10\"                           LOC = SLICE_X127Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_10\"                           LOC = SLICE_X127Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_10\"                           LOC = SLICE_X127Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_10\"                           LOC = SLICE_X127Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_10\"       LOC = SLICE_X127Y147;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_10\"    LOC = SLICE_X127Y147;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_11\"                           LOC = SLICE_X127Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_11\"                           LOC = SLICE_X127Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_11\"                           LOC = SLICE_X127Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_11\"                           LOC = SLICE_X127Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_11\"       LOC = SLICE_X127Y135;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_11\"    LOC = SLICE_X127Y135;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_12\"                           LOC = SLICE_X127Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_12\"                           LOC = SLICE_X127Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_12\"                           LOC = SLICE_X127Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_12\"                           LOC = SLICE_X127Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_12\"       LOC = SLICE_X127Y127;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_12\"    LOC = SLICE_X127Y127;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_13\"                           LOC = SLICE_X100Y79;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_13\"                           LOC = SLICE_X100Y79;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_13\"                           LOC = SLICE_X100Y79;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_13\"                           LOC = SLICE_X100Y79;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_13\"       LOC = SLICE_X100Y79;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_13\"    LOC = SLICE_X100Y79;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_14\"                           LOC = SLICE_X127Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_14\"                           LOC = SLICE_X127Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_14\"                           LOC = SLICE_X127Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_14\"                           LOC = SLICE_X127Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_14\"       LOC = SLICE_X127Y157;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_14\"    LOC = SLICE_X127Y157;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_15\"                           LOC = SLICE_X100Y61;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_15\"                           LOC = SLICE_X100Y61;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_15\"                           LOC = SLICE_X100Y61;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_15\"                           LOC = SLICE_X100Y61;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_15\"       LOC = SLICE_X100Y61;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_15\"    LOC = SLICE_X100Y61;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_0\"                            LOC = SLICE_X100Y49;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_0\"                            LOC = SLICE_X100Y49;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_0\"                            LOC = SLICE_X100Y49;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_0\"                            LOC = SLICE_X100Y49;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_16\"       LOC = SLICE_X100Y49;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_16\"    LOC = SLICE_X100Y49;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_1\"                            LOC = SLICE_X100Y59;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_1\"                            LOC = SLICE_X100Y59;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_1\"                            LOC = SLICE_X100Y59;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_1\"                            LOC = SLICE_X100Y59;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_17\"       LOC = SLICE_X100Y59;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_17\"    LOC = SLICE_X100Y59;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_2\"                            LOC = SLICE_X100Y41;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_2\"                            LOC = SLICE_X100Y41;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_2\"                            LOC = SLICE_X100Y41;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_2\"                            LOC = SLICE_X100Y41;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_18\"       LOC = SLICE_X100Y41;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_18\"    LOC = SLICE_X100Y41;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_3\"                            LOC = SLICE_X100Y57;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_3\"                            LOC = SLICE_X100Y57;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_3\"                            LOC = SLICE_X100Y57;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_3\"                            LOC = SLICE_X100Y57;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_19\"       LOC = SLICE_X100Y57;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_19\"    LOC = SLICE_X100Y57;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_4\"                            LOC = SLICE_X100Y45;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_4\"                            LOC = SLICE_X100Y45;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_4\"                            LOC = SLICE_X100Y45;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_4\"                            LOC = SLICE_X100Y45;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_20\"       LOC = SLICE_X100Y45;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_20\"    LOC = SLICE_X100Y45;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_5\"                            LOC = SLICE_X100Y67;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_5\"                            LOC = SLICE_X100Y67;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_5\"                            LOC = SLICE_X100Y67;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_5\"                            LOC = SLICE_X100Y67;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_21\"       LOC = SLICE_X100Y67;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_21\"    LOC = SLICE_X100Y67;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_6\"                            LOC = SLICE_X100Y69;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_6\"                            LOC = SLICE_X100Y69;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_6\"                            LOC = SLICE_X100Y69;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_6\"                            LOC = SLICE_X100Y69;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_22\"       LOC = SLICE_X100Y69;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_22\"    LOC = SLICE_X100Y69;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_7\"                            LOC = SLICE_X100Y53;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_7\"                            LOC = SLICE_X100Y53;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_7\"                            LOC = SLICE_X100Y53;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_7\"                            LOC = SLICE_X100Y53;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_23\"       LOC = SLICE_X100Y53;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_23\"    LOC = SLICE_X100Y53;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_8\"                            LOC = SLICE_X100Y63;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_8\"                            LOC = SLICE_X100Y63;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_8\"                            LOC = SLICE_X100Y63;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_8\"                            LOC = SLICE_X100Y63;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_24\"       LOC = SLICE_X100Y63;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_24\"    LOC = SLICE_X100Y63;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_9\"                            LOC = SLICE_X100Y65;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_9\"                            LOC = SLICE_X100Y65;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_9\"                            LOC = SLICE_X100Y65;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_9\"                            LOC = SLICE_X100Y65;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_25\"       LOC = SLICE_X100Y65;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_25\"    LOC = SLICE_X100Y65;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_10\"                           LOC = SLICE_X100Y75;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_10\"                           LOC = SLICE_X100Y75;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_10\"                           LOC = SLICE_X100Y75;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_10\"                           LOC = SLICE_X100Y75;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_26\"       LOC = SLICE_X100Y75;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_26\"    LOC = SLICE_X100Y75;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_11\"                           LOC = SLICE_X100Y73;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_11\"                           LOC = SLICE_X100Y73;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_11\"                           LOC = SLICE_X100Y73;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_11\"                           LOC = SLICE_X100Y73;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_27\"       LOC = SLICE_X100Y73;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_27\"    LOC = SLICE_X100Y73;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_12\"                           LOC = SLICE_X100Y77;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_12\"                           LOC = SLICE_X100Y77;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_12\"                           LOC = SLICE_X100Y77;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_12\"                           LOC = SLICE_X100Y77;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_28\"       LOC = SLICE_X100Y77;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_28\"    LOC = SLICE_X100Y77;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_13\"                           LOC = SLICE_X100Y43;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_13\"                           LOC = SLICE_X100Y43;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_13\"                           LOC = SLICE_X100Y43;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_13\"                           LOC = SLICE_X100Y43;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_29\"       LOC = SLICE_X100Y43;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_29\"    LOC = SLICE_X100Y43;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_14\"                           LOC = SLICE_X100Y55;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_14\"                           LOC = SLICE_X100Y55;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_14\"                           LOC = SLICE_X100Y55;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_14\"                           LOC = SLICE_X100Y55;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_30\"       LOC = SLICE_X100Y55;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_30\"    LOC = SLICE_X100Y55;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_15\"                           LOC = SLICE_X100Y47;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_15\"                           LOC = SLICE_X100Y47;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_15\"                           LOC = SLICE_X100Y47;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_15\"                           LOC = SLICE_X100Y47;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_31\"       LOC = SLICE_X100Y47;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_31\"    LOC = SLICE_X100Y47;"
        }
    }

    if [string match "3" $position] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/delay_ctrl_i\"                      IODELAY_GROUP = FMC2_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[*\].iodelay_i\"    IODELAY_GROUP = FMC2_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_dci_i\"                     IODELAY_GROUP = FMC2_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/iodelay_frame_i\"                   IODELAY_GROUP = FMC2_HPC_IODELAY_GRP;"

        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[28\].iodelay_i\"   IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_data_gen\[30\].iodelay_i\"   IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"

        # MI125 #0 (bottom) constraints
        puts $outputFileUcf "NET \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/serdes_rst_s_TIG\" TIG;"


        if {$fpgatype==550} then {
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC=BUFR_X1Y3;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC=BUFGCTRL_X0Y20;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC=BUFGCTRL_X0Y15;"

            puts $outputFileUcf "NET \"axi_mo1000_3/axi_mo1000_3/U_MMCM_V6_Inst/clkin1_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;9f0177e0!-1;-58264;-262864;S!0;875;64!1;37;18!\""
            puts $outputFileUcf "\"2;48;126!3;2555;1990!4;1314;2!5;2106;-537!5;-2121;6687!6;-854;-3495!7;-1;\""
            puts $outputFileUcf "\"14397!8;-3584;-27752!9;3430;7928!10;0;-51448!11;4096;29828!12;290;-31356!\""
            puts $outputFileUcf "\"13;0;51448!14;245;-8311!15;0;52224!16;483;-2047!17;0;51448!18;-2411;\""
            puts $outputFileUcf "\"-1039!19;0;52224!20;76;-647;L!21;4196;27784!23;4258;11260!24;0;12800!25;\""
            puts $outputFileUcf "\"15700;8604!26;15838;752!27;923;188;L!}\";"
        }
        
        if {$fpgatype==475 || $fpgatype==550} then {
            # LOC constraint for logic around SERDES to ease routing
            # All the following constraints are not necessary but will greatly help the routing
            # at high operating frequency (>250 MHz)

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_0\"                LOC = SLICE_X127Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v4_dci_s_1\"                LOC = SLICE_X127Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_33\"       LOC = SLICE_X127Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_33\"    LOC = SLICE_X127Y101;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_0\"                              LOC = SLICE_X127Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_1\"                              LOC = SLICE_X127Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_2\"                              LOC = SLICE_X127Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v4_Frame_s_3\"                              LOC = SLICE_X127Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_32\"       LOC = SLICE_X127Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_32\"    LOC = SLICE_X127Y117;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_0\"                            LOC = SLICE_X127Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_0\"                            LOC = SLICE_X127Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_0\"                            LOC = SLICE_X127Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_0\"                            LOC = SLICE_X127Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_0\"        LOC = SLICE_X127Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_0\"     LOC = SLICE_X127Y119;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_1\"                            LOC = SLICE_X127Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_1\"                            LOC = SLICE_X127Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_1\"                            LOC = SLICE_X127Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_1\"                            LOC = SLICE_X127Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_1\"        LOC = SLICE_X127Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_1\"     LOC = SLICE_X127Y113;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_2\"                            LOC = SLICE_X127Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_2\"                            LOC = SLICE_X127Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_2\"                            LOC = SLICE_X127Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_2\"                            LOC = SLICE_X127Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_2\"        LOC = SLICE_X127Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_2\"     LOC = SLICE_X127Y115;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_3\"                            LOC = SLICE_X127Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_3\"                            LOC = SLICE_X127Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_3\"                            LOC = SLICE_X127Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_3\"                            LOC = SLICE_X127Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_3\"        LOC = SLICE_X127Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_3\"     LOC = SLICE_X127Y107;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_4\"                            LOC = SLICE_X127Y93;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_4\"                            LOC = SLICE_X127Y93;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_4\"                            LOC = SLICE_X127Y93;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_4\"                            LOC = SLICE_X127Y93;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_4\"        LOC = SLICE_X127Y93;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_4\"     LOC = SLICE_X127Y93;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_5\"                            LOC = SLICE_X127Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_5\"                            LOC = SLICE_X127Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_5\"                            LOC = SLICE_X127Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_5\"                            LOC = SLICE_X127Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_5\"        LOC = SLICE_X127Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_5\"     LOC = SLICE_X127Y105;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_6\"                            LOC = SLICE_X127Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_6\"                            LOC = SLICE_X127Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_6\"                            LOC = SLICE_X127Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_6\"                            LOC = SLICE_X127Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_6\"        LOC = SLICE_X127Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_6\"     LOC = SLICE_X127Y103;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_7\"                            LOC = SLICE_X127Y85;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_7\"                            LOC = SLICE_X127Y85;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_7\"                            LOC = SLICE_X127Y85;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_7\"                            LOC = SLICE_X127Y85;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_7\"        LOC = SLICE_X127Y85;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_7\"     LOC = SLICE_X127Y85;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_8\"                            LOC = SLICE_X127Y95;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_8\"                            LOC = SLICE_X127Y95;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_8\"                            LOC = SLICE_X127Y95;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_8\"                            LOC = SLICE_X127Y95;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_8\"        LOC = SLICE_X127Y95;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_8\"     LOC = SLICE_X127Y95;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_9\"                            LOC = SLICE_X127Y89;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_9\"                            LOC = SLICE_X127Y89;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_9\"                            LOC = SLICE_X127Y89;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_9\"                            LOC = SLICE_X127Y89;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_9\"        LOC = SLICE_X127Y89;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_9\"     LOC = SLICE_X127Y89;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_10\"                           LOC = SLICE_X127Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_10\"                           LOC = SLICE_X127Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_10\"                           LOC = SLICE_X127Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_10\"                           LOC = SLICE_X127Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_10\"       LOC = SLICE_X127Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_10\"    LOC = SLICE_X127Y97;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_11\"                           LOC = SLICE_X127Y87;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_11\"                           LOC = SLICE_X127Y87;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_11\"                           LOC = SLICE_X127Y87;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_11\"                           LOC = SLICE_X127Y87;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_11\"       LOC = SLICE_X127Y87;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_11\"    LOC = SLICE_X127Y87;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_12\"                           LOC = SLICE_X100Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_12\"                           LOC = SLICE_X100Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_12\"                           LOC = SLICE_X100Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_12\"                           LOC = SLICE_X100Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_12\"       LOC = SLICE_X100Y141;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_12\"    LOC = SLICE_X100Y141;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_13\"                           LOC = SLICE_X127Y83;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_13\"                           LOC = SLICE_X127Y83;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_13\"                           LOC = SLICE_X127Y83;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_13\"                           LOC = SLICE_X127Y83;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_13\"       LOC = SLICE_X127Y83;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_13\"    LOC = SLICE_X127Y83;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_14\"                           LOC = SLICE_X100Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_14\"                           LOC = SLICE_X100Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_14\"                           LOC = SLICE_X100Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_14\"                           LOC = SLICE_X100Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_14\"       LOC = SLICE_X100Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_14\"    LOC = SLICE_X100Y121;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh1_s_15\"                           LOC = SLICE_X127Y81;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh2_s_15\"                           LOC = SLICE_X127Y81;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh3_s_15\"                           LOC = SLICE_X127Y81;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh4_s_15\"                           LOC = SLICE_X127Y81;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_15\"       LOC = SLICE_X127Y81;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_15\"    LOC = SLICE_X127Y81;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_0\"                            LOC = SLICE_X100Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_0\"                            LOC = SLICE_X100Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_0\"                            LOC = SLICE_X100Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_0\"                            LOC = SLICE_X100Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_16\"       LOC = SLICE_X100Y125;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_16\"    LOC = SLICE_X100Y125;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_1\"                            LOC = SLICE_X100Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_1\"                            LOC = SLICE_X100Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_1\"                            LOC = SLICE_X100Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_1\"                            LOC = SLICE_X100Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_17\"       LOC = SLICE_X100Y115;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_17\"    LOC = SLICE_X100Y115;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_2\"                            LOC = SLICE_X100Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_2\"                            LOC = SLICE_X100Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_2\"                            LOC = SLICE_X100Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_2\"                            LOC = SLICE_X100Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_18\"       LOC = SLICE_X100Y133;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_18\"    LOC = SLICE_X100Y133;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_3\"                            LOC = SLICE_X100Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_3\"                            LOC = SLICE_X100Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_3\"                            LOC = SLICE_X100Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_3\"                            LOC = SLICE_X100Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_19\"       LOC = SLICE_X100Y117;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_19\"    LOC = SLICE_X100Y117;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_4\"                            LOC = SLICE_X100Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_4\"                            LOC = SLICE_X100Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_4\"                            LOC = SLICE_X100Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_4\"                            LOC = SLICE_X100Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_20\"       LOC = SLICE_X100Y119;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_20\"    LOC = SLICE_X100Y119;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_5\"                            LOC = SLICE_X100Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_5\"                            LOC = SLICE_X100Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_5\"                            LOC = SLICE_X100Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_5\"                            LOC = SLICE_X100Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_21\"       LOC = SLICE_X100Y103;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_21\"    LOC = SLICE_X100Y103;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_6\"                            LOC = SLICE_X100Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_6\"                            LOC = SLICE_X100Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_6\"                            LOC = SLICE_X100Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_6\"                            LOC = SLICE_X100Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_22\"       LOC = SLICE_X100Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_22\"    LOC = SLICE_X100Y109;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_7\"                            LOC = SLICE_X100Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_7\"                            LOC = SLICE_X100Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_7\"                            LOC = SLICE_X100Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_7\"                            LOC = SLICE_X100Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_23\"       LOC = SLICE_X100Y113;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_23\"    LOC = SLICE_X100Y113;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_8\"                            LOC = SLICE_X100Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_8\"                            LOC = SLICE_X100Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_8\"                            LOC = SLICE_X100Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_8\"                            LOC = SLICE_X100Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_24\"       LOC = SLICE_X100Y107;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_24\"    LOC = SLICE_X100Y107;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_9\"                            LOC = SLICE_X100Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_9\"                            LOC = SLICE_X100Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_9\"                            LOC = SLICE_X100Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_9\"                            LOC = SLICE_X100Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_25\"       LOC = SLICE_X100Y101;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_25\"    LOC = SLICE_X100Y101;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_10\"                           LOC = SLICE_X127Y99;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_10\"                           LOC = SLICE_X127Y99;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_10\"                           LOC = SLICE_X127Y99;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_10\"                           LOC = SLICE_X127Y99;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_26\"       LOC = SLICE_X127Y99;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_26\"    LOC = SLICE_X127Y99;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_11\"                           LOC = SLICE_X127Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_11\"                           LOC = SLICE_X127Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_11\"                           LOC = SLICE_X127Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_11\"                           LOC = SLICE_X127Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_27\"       LOC = SLICE_X127Y109;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_27\"    LOC = SLICE_X127Y109;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_12\"                           LOC = SLICE_X127Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_12\"                           LOC = SLICE_X127Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_12\"                           LOC = SLICE_X127Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_12\"                           LOC = SLICE_X127Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_28\"       LOC = SLICE_X127Y123;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_28\"    LOC = SLICE_X127Y123;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_13\"                           LOC = SLICE_X100Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_13\"                           LOC = SLICE_X100Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_13\"                           LOC = SLICE_X100Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_13\"                           LOC = SLICE_X100Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_29\"       LOC = SLICE_X100Y105;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_29\"    LOC = SLICE_X100Y105;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_14\"                           LOC = SLICE_X127Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_14\"                           LOC = SLICE_X127Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_14\"                           LOC = SLICE_X127Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_14\"                           LOC = SLICE_X127Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_30\"       LOC = SLICE_X127Y121;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_30\"    LOC = SLICE_X127Y121;"

            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh5_s_15\"                           LOC = SLICE_X100Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh6_s_15\"                           LOC = SLICE_X100Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh7_s_15\"                           LOC = SLICE_X100Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/v16_DacCh8_s_15\"                           LOC = SLICE_X100Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_s_31\"       LOC = SLICE_X100Y97;"
            puts $outputFileUcf "INST \"*${name_lower}/mo1000_wrapper_i/dac_interface_i/v34_serdes_rst_r1_s_31\"    LOC = SLICE_X100Y97;"
        }

    }


   # Close the UCF file
   close $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]


}