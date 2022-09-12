
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
   
   #Bypass timing check at async FIFO pointers
   puts $outputFileUcf "NET \"*/U0/xst_fifo_generator/gconvfifo.rf/grf.rf/gntv_or_sync_fifo.gcx.clkx/wr_pntr_gc*\" TIG;"
   puts $outputFileUcf "NET \"*/U0/xst_fifo_generator/gconvfifo.rf/grf.rf/gntv_or_sync_fifo.gcx.clkx/rd_pntr_gc*\" TIG;"
   
   # Close the UCF file
   close  $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]
}