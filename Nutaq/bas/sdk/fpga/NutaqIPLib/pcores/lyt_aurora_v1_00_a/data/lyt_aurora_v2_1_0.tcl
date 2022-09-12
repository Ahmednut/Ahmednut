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
  
   # Find MGT used
   #  
   set    mgt       [string tolower [xget_hw_parameter_value $mhsinst "MGT_USED"]]
   
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

   if {[string match "117" $mgt] || [string match "118" $mgt]} {
      if {$fpgatype==475} then {
          #If aurora uses MGT117 or MGT118 on the sx475, lock the microblaze to the top
          #left corner of the device to avoid congestion and help PAR
          puts $outputFileUcf "INST \"main_cpu\" AREA_GROUP = \"pblock_main_cpu\";"
          puts $outputFileUcf "AREA_GROUP \"pblock_main_cpu\" RANGE=SLICE_X0Y240:SLICE_X87Y359;"
          puts $outputFileUcf "AREA_GROUP \"pblock_main_cpu\" RANGE=DSP48_X0Y96:DSP48_X6Y143;"
          puts $outputFileUcf "AREA_GROUP \"pblock_main_cpu\" RANGE=RAMB18_X0Y96:RAMB18_X6Y143;"
          puts $outputFileUcf "AREA_GROUP \"pblock_main_cpu\" RANGE=RAMB36_X0Y48:RAMB36_X6Y71;"
      }
   }

   # Path between two registers with two different clocks that does not affect the timing of the design.
   #
   
   # Close the UCF file
   close  $outputFileUcf

   puts   [xget_ncf_loc_info $mhsinst]
}