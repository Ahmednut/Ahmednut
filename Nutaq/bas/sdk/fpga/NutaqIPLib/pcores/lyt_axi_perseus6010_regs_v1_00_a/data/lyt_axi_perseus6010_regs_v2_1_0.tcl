#***--------------------------------***-----------------------------------***
#
#                CORE_LEVEL_CONSTRAINTS
#
#***--------------------------------***-----------------------------------***

proc generate_corelevel_ucf {mhsinst} {

   set  filePath [xget_ncf_dir $mhsinst]

   file mkdir    $filePath

   # specify file name
   set    instname   [xget_hw_parameter_value $mhsinst "INSTANCE"]
   set    name_lower [string      tolower    $instname]
   set    fileName   $name_lower
   append filePath   $fileName

   # Open UCF file for writing and delete XDC file (if any)
   set outputFileUcf [open "${filePath}_wrapper.ucf" "w"]
   file delete -force "${filePath}.xdc"

   puts "INFO: Setting timing constaints for ${instname}."

   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/v8_nCtrlLedGrn_s*\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/v8_nCtrlLedRed_s*\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/v2_uart_mode_s*\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/v2_CtrlVadjSel_s*\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/nFpgaProg_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/Ctrl100mhzOutEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlAmctclka2Fmcclk2En_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlAmctclkc2Fmcclk3En_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlFclkaHighz_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlFmcclk02AmctclkbEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlFmcclk12AmctclkdEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlLedBufOd_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkaRxDis_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkaTxEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkbRxDis_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkbTxEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkcRxDis_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkcTxEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkdRxDis_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlTclkdTxEn_s\" TIG;"
   puts $outputFileUcf "NET \"${instname}/USER_LOGIC_I/CtrlVadjEn_s\" TIG;"
   
   # Close the UCF file
   close  $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]
}