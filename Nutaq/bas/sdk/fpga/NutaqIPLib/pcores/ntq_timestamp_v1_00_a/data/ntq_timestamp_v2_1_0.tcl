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

   
   # Syncer instances does not need timing when the signal cross clock domains.
   #
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/OutSig_s\"           TNM = ${name_lower}_syncer_from_group0;"
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/OutSigTmp1_s\"       TNM = ${name_lower}_syncer_to_group0;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_syncer_from_to_tig0 =     FROM ${name_lower}_syncer_from_group0 TO ${name_lower}_syncer_to_group0 TIG;"
   
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/OutSigTmp2_s\"       TNM = ${name_lower}_syncer_from_group1;"
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/InSigClearTmp1_s\"   TNM = ${name_lower}_syncer_to_group1;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_syncer_from_to_tig1 =     FROM ${name_lower}_syncer_from_group1 TO ${name_lower}_syncer_to_group1 TIG;"

   
   # Path between two registers with two different clocks that does not affect the timing of the design.
   #
   puts $outputFileUcf "NET \"${name_lower}/v64_TimeStampLatch_s*\" TIG;"
   
   # Close the UCF file
   close  $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]
}