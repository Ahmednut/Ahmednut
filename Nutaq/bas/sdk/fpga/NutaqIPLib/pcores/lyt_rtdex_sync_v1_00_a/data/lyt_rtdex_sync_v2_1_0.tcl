
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

   # Constraints start here...
   #
   puts $outputFileUcf "NET \"${name_lower}/ov32_IndRdReg_p<*>\" TIG;"

   # TIG constraints on the RTDEx Frame Size since they can be seen as constants 
   # (they are set a while before enabling the core).
   #
   puts $outputFileUcf "NET \"${name_lower}/a8v20_RTDExSyncRxFrameSize_s*\" TIG;"
   
   puts $outputFileUcf "NET \"${name_lower}/a8v20_RTDExSyncTxFrameSize_s*\" TIG;"
   
   puts $outputFileUcf "NET \"${name_lower}/a8v20_RTDExSyncTxCntxFrameSize_s*\" TIG;"
   
   # Delay constraints on the signals coming from the Microblaze clock domain
   #
   puts $outputFileUcf "NET \"${name_lower}/i_IndWrEn_p\" MAXDELAY = 10 ns;"
   puts $outputFileUcf "NET \"${name_lower}/iv8_IndAddr_p<*>\" MAXDELAY = 10 ns;"
   puts $outputFileUcf "NET \"${name_lower}/iv32_IndWrReg_p<*>\" MAXDELAY = 10 ns;"

   # Delay constraints on the event and error FIFOs of the encoder module
   #
   puts $outputFileUcf "NET \"${name_lower}/*/*link_encoder*/v169_encFifoErrDout_s<*>\" MAXDELAY = 10 ns;"
   puts $outputFileUcf "NET \"${name_lower}/*/*link_encoder*/v129_encFifoEventDout_s<*>\" MAXDELAY = 10 ns;"

   # TIG constraint on the reset signal (the enable is set to '1' later.
   #  
   puts $outputFileUcf "NET \"${name_lower}/core_reset_s\"  TIG;"

   # Relax the constraint on the enable signal (from core clock to user clock)
   #
   puts $outputFileUcf "INST \"${name_lower}/v8_chEnable_s*\"  TNM = ${name_lower}_v8_chEnable_s;"
   puts $outputFileUcf "INST \"${name_lower}/*/u_lyt_downlink_fsm/coreEnableUClk_s\"  TNM = ${name_lower}_coreEnableUClk_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_ENABLE_UCLK = FROM ${name_lower}_v8_chEnable_s TO ${name_lower}_coreEnableUClk_s 10 ns;"

   # The encoder FIFO reset is held many clock cycles.
   #   
   puts $outputFileUcf "INST \"${name_lower}*/u_lyt_uplink_fsm/encFifoRst_s\"  TNM = ${name_lower}_encFifoRst_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_encFifoRst_FFS = FROM ${name_lower}_encFifoRst_s TO FFS 10 ns DATAPATHONLY;"
   
   
   # Syncer instances does not need timing when the signal cross clock domains.
   #
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/OutSig_s\"            TNM = ${name_lower}_syncer_from_group0;"
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/OutSigTmp1_s\"        TNM = ${name_lower}_syncer_to_group0;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_syncer_from_to_tig0 =     FROM ${name_lower}_syncer_from_group0 TO ${name_lower}_syncer_to_group0 TIG;"
   
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/OutSigTmp2_s\"        TNM = ${name_lower}_syncer_from_group1;"
   puts $outputFileUcf "INST \"${name_lower}*/Syncer_*/InSigClearTmp1_s\"    TNM = ${name_lower}_syncer_to_group1;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_syncer_from_to_tig1 =     FROM ${name_lower}_syncer_from_group1 TO ${name_lower}_syncer_to_group1 TIG;"
   
   
   # Bus Syncer instances does not need timing when the signal cross clock domains.
   #
   puts $outputFileUcf "INST \"${name_lower}*/BusSyncer_*/NewValuePending\"          TNM = ${name_lower}_bussyncer_from_group0;"
   puts $outputFileUcf "INST \"${name_lower}*/BusSyncer_*/NewValuePendingOclkD1_s\"  TNM = ${name_lower}_bussyncer_to_group0;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_bussyncer_from_to_tig0 =          FROM ${name_lower}_bussyncer_from_group0 TO ${name_lower}_bussyncer_to_group0 TIG;"
   
   puts $outputFileUcf "INST \"${name_lower}*/BusSyncer_*/NewValuePendingOclkD2_s\"  TNM = ${name_lower}_bussyncer_from_group1;"
   puts $outputFileUcf "INST \"${name_lower}*/BusSyncer_*/NewValuePendingIclkD1_s\"  TNM = ${name_lower}_bussyncer_to_group1;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_bussyncer_from_to_tig1 =          FROM ${name_lower}_bussyncer_from_group1 TO ${name_lower}_bussyncer_to_group1 TIG;"
   
   puts $outputFileUcf "INST \"${name_lower}*/BusSyncer_*/v_ValueFrozen_s*\"  TNM = ${name_lower}_v_ValueFrozen_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v_ValueFrozen_s = FROM ${name_lower}_v_ValueFrozen_s TO FFS 10 ns DATAPATHONLY;"
   
   
   # Busses that need to be stable at the syncer pulse rising edge
   # Minimum requirement : 2 clock cycles at 200 MHz (10 ns)
   #
   puts $outputFileUcf "INST \"${name_lower}/*/v64_StartTime_s*\"               TNM = v64_StartTime_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v64_StartTime_s =             FROM v64_StartTime_s TO FFS 10 ns DATAPATHONLY;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v64_StopTime_s*\"                TNM = v64_StopTime_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v64_StopTime_s =              FROM v64_StopTime_s TO FFS 10 ns DATAPATHONLY;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v64_PendingTime_s*\"             TNM = v64_PendingTime_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v64_PendingTime_s =           FROM v64_PendingTime_s TO FFS 10 ns DATAPATHONLY;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v64_vrtTimeStamp_s*\"            TNM = v64_vrtTimeStamp_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v64_vrtTimeStamp_s =          FROM v64_vrtTimeStamp_s TO FFS TIG;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v64_InvalidTimeStampValue_s*\"   TNM = v64_InvalidTimeStampValue_s;"
   #puts $outputFileUcf "TIMESPEC TS_${name_lower}_v64_InvalidTimeStampValue_s = FROM v64_InvalidTimeStampValue_s TO RAMS 10 ns DATAPATHONLY;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v64_InvalidTimeStampValue_s = FROM v64_InvalidTimeStampValue_s TO FFS 10 ns DATAPATHONLY;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v64_InvalidCommandTimeStampValue_s*\"    TNM = v64_InvalidCommandTimeStampValue_s;"
   #puts $outputFileUcf "TIMESPEC TS_${name_lower}_vv64_InvalidCommandTimeStampValue_s = FROM v64_InvalidCommandTimeStampValue_s TO RAMS 10 ns DATAPATHONLY;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_vv64_InvalidCommandTimeStampValue_s = FROM v64_InvalidCommandTimeStampValue_s TO FFS 10 ns DATAPATHONLY;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v2_vrtCurrentSyncMode_s*\"       TNM = v2_vrtCurrentSyncMode_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v2_vrtCurrentSyncMode_s =     FROM v2_vrtCurrentSyncMode_s TO FFS 10 ns DATAPATHONLY;"
   
   puts $outputFileUcf "INST \"${name_lower}/*/v2_vrtTrigSel_s*\"               TNM = v2_vrtTrigSel_s;"
   puts $outputFileUcf "TIMESPEC TS_${name_lower}_v2_vrtTrigSel_s =             FROM v2_vrtTrigSel_s TO FFS 10 ns DATAPATHONLY;"
   
   # Close the UCF file
   close  $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]
}
