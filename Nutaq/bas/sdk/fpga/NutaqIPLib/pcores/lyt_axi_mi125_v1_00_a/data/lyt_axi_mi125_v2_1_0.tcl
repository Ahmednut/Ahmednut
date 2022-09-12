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

    puts $outputFileUcf "PIN \"*${name_lower}/U_MMCM_V6_Inst/SOURCE_SYNC_CLOCK_IN.O\" CLOCK_DEDICATED_ROUTE = FALSE;"
    puts $outputFileUcf "PIN \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR.I\" CLOCK_DEDICATED_ROUTE = FALSE;"

   # Constraints start here...
   #

    if [string match "0" $position] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/RX_IDELAYCTRL\"               IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[*\].IODELAY_RX_DATA\"   IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"

        # MI125 #0 (bottom) constraints

        # Since the path from IBUFDS to BUFG is not clock dedicated (pin in outer column)
        # force it to be consistent from one build to another.
        if {$fpgatype==240} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y17;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y16;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y8;"
            
            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx240tff1759;a96ba6b8!-1;-221152;67314;S!0;16;120!1;64;126!\""
            puts $outputFileUcf "\"2;2903;1990!3;1662;2!4;-2232;-4096!4;-1417;1563!5;3484;-28712!6;2669;\""
            puts $outputFileUcf "\"30653!7;35153;-26547!8;0;51448!9;42062;2370!10;150;32644!11;11765;-5723!\""
            puts $outputFileUcf "\"12;-1176;7056!13;8304;-6400!14;-1888;-1414!15;39193;-3295!16;-2786;1810!\""
            puts $outputFileUcf "\"17;43646;2370!18;103;680;L!19;23251;1357!21;15874;668!22;923;22;L!}\";"
        }
        if {$fpgatype==315} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y17;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y16;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y8;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vsx315tff1759;5a8d143b!-1;-298306;67314;S!0;16;120!1;64;126!\""
            puts $outputFileUcf "\"2;2903;1990!3;1662;2!4;-1853;2149!5;-479;-25357!5;-479;26867!6;34131;\""
            puts $outputFileUcf "\"-26547!7;0;51448!8;68044;0!9;150;32644!10;68108;0!11;345;7685!12;79981;\""
            puts $outputFileUcf "\"-313!13;175;1157!14;47456;-4496!15;-2786;1810!16;128;-9596!17;103;680;L!\""
            puts $outputFileUcf "\"18;1546;-2936!20;592;-912!21;923;22;L!}\";"

        }
        if {$fpgatype==475} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y6;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y7;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y8;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vsx475tff1759;250812c6!-1;-298306;132338;S!0;16;120!1;64;\""
            puts $outputFileUcf "\"126!2;2903;1990!3;1662;2!4;-1797;-1051!4;-2042;-6564!5;-8;-3712!6;3394;\""
            puts $outputFileUcf "\"-11356!7;-8;-3712!8;4052;-27688!9;33612;-2880!10;0;-51448!11;68044;0!12;\""
            puts $outputFileUcf "\"-4152;-29848!13;68108;0!14;-3049;-3419!15;33057;-24653!16;135;-2047!17;\""
            puts $outputFileUcf "\"46924;-26860!18;-2759;-1039!19;47456;-4496!20;76;-695;L!21;1674;-6380!23;\""
            puts $outputFileUcf "\"592;-856!24;923;496;L!}\";"

        }
        if {$fpgatype==550} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y6;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y7;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y8;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;cff98962!-1;-303442;132338;S!0;16;120!1;64;\""
            puts $outputFileUcf "\"126!2;2903;1990!3;1662;2!4;-1853;2149!4;-2042;-6564!5;-479;-25357!6;3394;\""
            puts $outputFileUcf "\"-11356!7;32058;-26852!8;4052;-27688!9;65389;305!10;0;-51448!11;67136;0!\""
            puts $outputFileUcf "\"12;-4152;-29848!13;78785;-313!14;-3049;-3419!15;47456;-4496!16;135;-2047!\""
            puts $outputFileUcf "\"17;0;-12800!18;-2759;-1039!19;1674;-6380!20;76;-695;L!21;592;-856!23;923;\""
            puts $outputFileUcf "\"496;L!}\";"

        }

    }

    if [string match "1" $position] {

        # MI125 #1 (top) constraints

        puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y19;"
        puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y18;"

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/RX_IDELAYCTRL\"               IODELAY_GROUP = FMC1_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[*\].IODELAY_RX_DATA\"   IODELAY_GROUP = FMC1_HPC_IODELAY_GRP;"

        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[24\].IODELAY_RX_DATA\"  IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[26\].IODELAY_RX_DATA\"  IODELAY_GROUP = FMC1_LPC_IODELAY_GRP;"

        # Since the path from IBUFDS to BUFG is not clock dedicated (pin in outer column)
        # force it to be consistent from one build to another.
        if {$fpgatype==240} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y6;"
            
            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx240tff1759;10ec6e4!-1;-221152;-61958;S!0;16;120!1;64;126!\""
            puts $outputFileUcf "\"2;2903;1990!3;1662;2!4;-1853;2149!4;1308;2388!5;-479;26867!6;4096;29828!\""
            puts $outputFileUcf "\"7;0;51448!8;35153;24653!9;150;32644!10;67590;0!11;345;7685!12;42292;2370!\""
            puts $outputFileUcf "\"13;175;1157!14;11041;-207!15;-2786;1810!16;5851;67!17;103;680;L!18;14044;\""
            puts $outputFileUcf "\"120!20;23289;1441!21;15836;564!22;923;274;L!}\";"

        }
        if {$fpgatype==315} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y6;"
            
            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vsx315tff1759;b0ff0833!-1;-298306;-61958;S!0;16;120!1;64;\""
            puts $outputFileUcf "\"126!2;2903;1990!3;1662;2!4;-1417;1563!4;1308;2388!5;2669;30653!6;4096;\""
            puts $outputFileUcf "\"29828!7;0;51448!8;35153;24653!9;150;32644!10;68044;0!11;-1176;7056!12;\""
            puts $outputFileUcf "\"68108;0!13;-1888;-1414!14;42580;2370!15;-2786;1810!16;10911;1425!17;103;\""
            puts $outputFileUcf "\"680;L!18;12212;2024!20;16117;-1!21;23273;-207!22;15852;-1256!23;923;274;L\""
            puts $outputFileUcf "\"!}\";"
            
        }
        if {$fpgatype==475} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y10;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vsx475tff1759;80648c8b!-1;-298306;3066;S!0;16;120!1;64;126!\""
            puts $outputFileUcf "\"2;2903;1990!3;1662;2!4;6259;-697!4;-1853;2149!5;11657;3085!6;0;3200!7;\""
            puts $outputFileUcf "\"4384;29828!8;-479;26867!9;35383;24653!10;0;51448!11;68044;0!12;150;32644!\""
            puts $outputFileUcf "\"13;68108;0!14;345;8197!15;42350;2370!16;175;1669!17;11843;1425!18;-2759;\""
            puts $outputFileUcf "\"-1039!19;10501;2023!20;76;-695;L!21;23273;-207!23;15852;-1256!24;923;274;\""
            puts $outputFileUcf "\"L!}\";"

        }
        if {$fpgatype==550} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X0Y10;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;34e0ca86!-1;-303442;3066;S!0;16;120!1;64;126!\""
            puts $outputFileUcf "\"2;2903;1990!3;1662;2!4;6259;-697!4;-1417;1563!5;16896;0!6;2669;30653!7;\""
            puts $outputFileUcf "\"17126;0!8;0;51448!9;7928;2846!10;150;32644!11;-479;26867!12;-1176;7056!\""
            puts $outputFileUcf "\"13;34411;24653!14;-1888;-1414!15;67654;0!16;-2786;1810!17;67014;0!18;103;\""
            puts $outputFileUcf "\"680;L!19;38153;3131!21;19944;5680!22;15852;-1256!23;923;274;L!}\";"

        }

    }

    if [string match "2" $position] {

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/RX_IDELAYCTRL\"               IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[*\].IODELAY_RX_DATA\"   IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"

        # MI125 #2 (bottom) constraints

        # Since the path from IBUFDS to BUFG is not clock dedicated (pin in outer column)
        # force it to be consistent from one build to another.
        if {$fpgatype==240} then {
            # Not supported
        }
        if {$fpgatype==315} then {
            # Not supported
        }
        if {$fpgatype==475} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y16;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y14;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X1Y7;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vsx475tff1759;e60eb74d!-1;28264;-126206;S!0;-16;120!1;-64;\""
            puts $outputFileUcf "\"126!2;-111;1992!3;-452;0!4;-1890;4064!4;-9845;-657!5;-4726;7260!6;-15417;\""
            puts $outputFileUcf "\"-1020!7;-4084;27292!8;-12527;1020!9;0;51448!10;-16672;0!11;0;52224!12;\""
            puts $outputFileUcf "\"-9995;-1095!13;150;32644!14;-3858;-1920!15;-4358;10476!16;-2024;574!17;\""
            puts $outputFileUcf "\"-6414;4452!18;-2411;-1039!19;-2306;596!20;76;-647;L!21;923;-2;L!}\";"

        }
        if {$fpgatype==550} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y30;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y0;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X1Y7;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;3dc5dcae!-1;16232;-126206;S!0;-16;120!1;-64;\""
            puts $outputFileUcf "\"126!2;-111;1992!3;-452;0!4;-1853;2149!4;-9845;-657!5;-479;26867!6;-15417;\""
            puts $outputFileUcf "\"-1020!7;150;32892!8;-12527;1020!9;-1016;8280!10;-16672;0!11;-3350;26276!\""
            puts $outputFileUcf "\"12;-9995;-1095!13;0;52224!14;-3858;-1920!15;150;32644!16;-2024;574!17;\""
            puts $outputFileUcf "\"-4358;10724!18;-2411;-1039!19;-4930;4204!20;76;-647;L!21;-5644;4!23;\""
            puts $outputFileUcf "\"-2454;728!24;647;-3667!25;432;965;L!}\";"

        }

    }

    if [string match "3" $position] {

        # MI125 #3 (top) constraints

        # IODELAY constraints
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/RX_IDELAYCTRL\"               IODELAY_GROUP = FMC2_HPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[*\].IODELAY_RX_DATA\"   IODELAY_GROUP = FMC2_HPC_IODELAY_GRP;"

        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[24\].IODELAY_RX_DATA\"  IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"
        puts $outputFileUcf "INST \"*${name_lower}/MI125_Wrapper_INST/DDR_8TO1_32CHAN_RX_INST00/pins\[26\].IODELAY_RX_DATA\"  IODELAY_GROUP = FMC2_LPC_IODELAY_GRP;"

        # Since the path from IBUFDS to BUFG is not clock dedicated (pin in outer column)
        # force it to be consistent from one build to another.
        if {$fpgatype==240} then {
            # Not supported
        }
        if {$fpgatype==315} then {
            # Not supported
        }
        if {$fpgatype==475} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y2;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y3;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X1Y9;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vsx475tff1759;a8f1babb!-1;28264;-255478;S!0;-16;120!1;-64;\""
            puts $outputFileUcf "\"126!2;-111;1992!2;553;-693!3;-452;0!4;-11298;1517!5;-6616;4924!6;-9735;\""
            puts $outputFileUcf "\"60965!7;-4084;27292!8;0;129272!9;0;51448!10;0;94242!11;0;52224!12;-2788;\""
            puts $outputFileUcf "\"29977;L!13;0;51200!15;150;32892!16;-4358;10476!17;-6406;992!18;-18302;\""
            puts $outputFileUcf "\"5020!19;-23137;311!20;-8433;5601!21;293;7689!22;555;1153!23;-2438;1810!\""
            puts $outputFileUcf "\"24;103;728;L!}\";"

        }
        if {$fpgatype==550} then {

            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout1_buf\" LOC = BUFGCTRL_X0Y4;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/clkout2_buf\" LOC = BUFGCTRL_X0Y12;"
            puts $outputFileUcf "INST \"*${name_lower}/U_MMCM_V6_Inst/RX_CLK_BUFR\" LOC = BUFR_X2Y8;"

            puts $outputFileUcf "NET \"*${name_lower}/U_MMCM_V6_Inst/clkin1_delay_s\""
            puts $outputFileUcf "ROUTE=\"{3;1;6vlx550tff1759;191433!-1;16232;-255478;S!0;-16;120!1;-64;126!\""
            puts $outputFileUcf "\"2;-111;1992!2;553;-693!3;-452;0!4;-11298;1517!5;-1853;2149!6;-9751;60981!\""
            puts $outputFileUcf "\"7;-479;26867!8;0;129272!9;0;51448!10;0;94242!11;0;52224!12;-2772;30173;L!\""
            puts $outputFileUcf "\"13;0;51200!15;150;32892!16;345;7685!17;-77;5119!18;-248;9596!19;248;9604!\""
            puts $outputFileUcf "\"20;1922;2440!21;1152;1808!22;103;680;L!}\";"

        }

    }


   # Close the UCF file
   close $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]


}