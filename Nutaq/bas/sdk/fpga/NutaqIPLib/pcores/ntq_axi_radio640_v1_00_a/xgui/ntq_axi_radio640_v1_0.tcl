# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "C_BASEADDR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_BOARD_POSITION" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_CARRIER" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_CLOCK_MASTER" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_DESIGN_CLK_RATE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_FMC_CONNECTOR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_HIGHADDR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_S_AXI_MIN_SIZE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "PCORE_ADC_DP_DISABLE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "PCORE_DAC_DP_DISABLE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "PCORE_DEVICE_TYPE" -parent ${Page_0}


}

proc update_PARAM_VALUE.C_BASEADDR { PARAM_VALUE.C_BASEADDR } {
	# Procedure called to update C_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_BASEADDR { PARAM_VALUE.C_BASEADDR } {
	# Procedure called to validate C_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_BOARD_POSITION { PARAM_VALUE.C_BOARD_POSITION } {
	# Procedure called to update C_BOARD_POSITION when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_BOARD_POSITION { PARAM_VALUE.C_BOARD_POSITION } {
	# Procedure called to validate C_BOARD_POSITION
	return true
}

proc update_PARAM_VALUE.C_CARRIER { PARAM_VALUE.C_CARRIER } {
	# Procedure called to update C_CARRIER when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_CARRIER { PARAM_VALUE.C_CARRIER } {
	# Procedure called to validate C_CARRIER
	return true
}

proc update_PARAM_VALUE.C_CLOCK_MASTER { PARAM_VALUE.C_CLOCK_MASTER } {
	# Procedure called to update C_CLOCK_MASTER when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_CLOCK_MASTER { PARAM_VALUE.C_CLOCK_MASTER } {
	# Procedure called to validate C_CLOCK_MASTER
	return true
}

proc update_PARAM_VALUE.C_DESIGN_CLK_RATE { PARAM_VALUE.C_DESIGN_CLK_RATE } {
	# Procedure called to update C_DESIGN_CLK_RATE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_DESIGN_CLK_RATE { PARAM_VALUE.C_DESIGN_CLK_RATE } {
	# Procedure called to validate C_DESIGN_CLK_RATE
	return true
}

proc update_PARAM_VALUE.C_FMC_CONNECTOR { PARAM_VALUE.C_FMC_CONNECTOR } {
	# Procedure called to update C_FMC_CONNECTOR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_FMC_CONNECTOR { PARAM_VALUE.C_FMC_CONNECTOR } {
	# Procedure called to validate C_FMC_CONNECTOR
	return true
}

proc update_PARAM_VALUE.C_HIGHADDR { PARAM_VALUE.C_HIGHADDR } {
	# Procedure called to update C_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_HIGHADDR { PARAM_VALUE.C_HIGHADDR } {
	# Procedure called to validate C_HIGHADDR
	return true
}

proc update_PARAM_VALUE.C_S_AXI_MIN_SIZE { PARAM_VALUE.C_S_AXI_MIN_SIZE } {
	# Procedure called to update C_S_AXI_MIN_SIZE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S_AXI_MIN_SIZE { PARAM_VALUE.C_S_AXI_MIN_SIZE } {
	# Procedure called to validate C_S_AXI_MIN_SIZE
	return true
}

proc update_PARAM_VALUE.PCORE_ADC_DP_DISABLE { PARAM_VALUE.PCORE_ADC_DP_DISABLE } {
	# Procedure called to update PCORE_ADC_DP_DISABLE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.PCORE_ADC_DP_DISABLE { PARAM_VALUE.PCORE_ADC_DP_DISABLE } {
	# Procedure called to validate PCORE_ADC_DP_DISABLE
	return true
}

proc update_PARAM_VALUE.PCORE_DAC_DP_DISABLE { PARAM_VALUE.PCORE_DAC_DP_DISABLE } {
	# Procedure called to update PCORE_DAC_DP_DISABLE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.PCORE_DAC_DP_DISABLE { PARAM_VALUE.PCORE_DAC_DP_DISABLE } {
	# Procedure called to validate PCORE_DAC_DP_DISABLE
	return true
}

proc update_PARAM_VALUE.PCORE_DEVICE_TYPE { PARAM_VALUE.PCORE_DEVICE_TYPE } {
	# Procedure called to update PCORE_DEVICE_TYPE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.PCORE_DEVICE_TYPE { PARAM_VALUE.PCORE_DEVICE_TYPE } {
	# Procedure called to validate PCORE_DEVICE_TYPE
	return true
}


proc update_MODELPARAM_VALUE.C_CARRIER { MODELPARAM_VALUE.C_CARRIER PARAM_VALUE.C_CARRIER } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_CARRIER}] ${MODELPARAM_VALUE.C_CARRIER}
}

proc update_MODELPARAM_VALUE.C_FMC_CONNECTOR { MODELPARAM_VALUE.C_FMC_CONNECTOR PARAM_VALUE.C_FMC_CONNECTOR } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_FMC_CONNECTOR}] ${MODELPARAM_VALUE.C_FMC_CONNECTOR}
}

proc update_MODELPARAM_VALUE.C_BOARD_POSITION { MODELPARAM_VALUE.C_BOARD_POSITION PARAM_VALUE.C_BOARD_POSITION } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_BOARD_POSITION}] ${MODELPARAM_VALUE.C_BOARD_POSITION}
}

proc update_MODELPARAM_VALUE.C_CLOCK_MASTER { MODELPARAM_VALUE.C_CLOCK_MASTER PARAM_VALUE.C_CLOCK_MASTER } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_CLOCK_MASTER}] ${MODELPARAM_VALUE.C_CLOCK_MASTER}
}

proc update_MODELPARAM_VALUE.C_DESIGN_CLK_RATE { MODELPARAM_VALUE.C_DESIGN_CLK_RATE PARAM_VALUE.C_DESIGN_CLK_RATE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_DESIGN_CLK_RATE}] ${MODELPARAM_VALUE.C_DESIGN_CLK_RATE}
}

proc update_MODELPARAM_VALUE.PCORE_DEVICE_TYPE { MODELPARAM_VALUE.PCORE_DEVICE_TYPE PARAM_VALUE.PCORE_DEVICE_TYPE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.PCORE_DEVICE_TYPE}] ${MODELPARAM_VALUE.PCORE_DEVICE_TYPE}
}

proc update_MODELPARAM_VALUE.PCORE_DAC_DP_DISABLE { MODELPARAM_VALUE.PCORE_DAC_DP_DISABLE PARAM_VALUE.PCORE_DAC_DP_DISABLE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.PCORE_DAC_DP_DISABLE}] ${MODELPARAM_VALUE.PCORE_DAC_DP_DISABLE}
}

proc update_MODELPARAM_VALUE.PCORE_ADC_DP_DISABLE { MODELPARAM_VALUE.PCORE_ADC_DP_DISABLE PARAM_VALUE.PCORE_ADC_DP_DISABLE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.PCORE_ADC_DP_DISABLE}] ${MODELPARAM_VALUE.PCORE_ADC_DP_DISABLE}
}

proc update_MODELPARAM_VALUE.C_S_AXI_MIN_SIZE { MODELPARAM_VALUE.C_S_AXI_MIN_SIZE PARAM_VALUE.C_S_AXI_MIN_SIZE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S_AXI_MIN_SIZE}] ${MODELPARAM_VALUE.C_S_AXI_MIN_SIZE}
}

proc update_MODELPARAM_VALUE.C_BASEADDR { MODELPARAM_VALUE.C_BASEADDR PARAM_VALUE.C_BASEADDR } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_BASEADDR}] ${MODELPARAM_VALUE.C_BASEADDR}
}

proc update_MODELPARAM_VALUE.C_HIGHADDR { MODELPARAM_VALUE.C_HIGHADDR PARAM_VALUE.C_HIGHADDR } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_HIGHADDR}] ${MODELPARAM_VALUE.C_HIGHADDR}
}

