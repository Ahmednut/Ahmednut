
# Validate the Existence of BINGEN (For the binaryGenerator)
if {[info exists env(BINGEN)]} {
  set packagesfilesDir $env(BINGENCHECKOUTDIR)/projets/bas/sdk/fpga/packages
  set netlistDestCores $env(BinaryDir)/pcores
  set coresDir $env(BinaryDir)
} else {
  set packagesfilesDir $env(BASROOT)/sdk/fpga/packages
  set netlistDestCores $env(BASROOT)/sdk/fpga/lyrtech_ip_cores/pcores
  set coresDir $env(BASROOT)/sdk/fpga/netlist
}


set srcCore1 $packagesfilesDir/clkmon_p.vhd
set srcCore2 $coresDir/clkmon.ngc

set destcore $netlistDestCores/lyt_clkmon_v1_00_a

set vhdlRep $destcore/hdl/vhdl
set netlistRep $destcore/netlist

file mkdir $vhdlRep
file mkdir $netlistRep

file copy -force $srcCore1 $vhdlRep
file copy -force $srcCore2 $netlistRep
