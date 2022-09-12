ifndef PETALINUX
$(error You must source the petalinux/settings.sh script before working with PetaLinux)
endif

# Point to default PetaLinux root directory
ifndef ROOTDIR
ROOTDIR=$(PETALINUX)/software/petalinux-dist
endif

PATH:=$(PATH):$(ROOTDIR)/tools

UCLINUX_BUILD_USER = 1
-include $(ROOTDIR)/.config
-include $(ROOTDIR)/$(CONFIG_LINUXDIR)/.config
LIBCDIR = $(CONFIG_LIBCDIR)
-include $(ROOTDIR)/config.arch
ROMFSDIR=$(ROOTDIR)/romfs
ROMFSINST=$(ROOTDIR)/tools/romfs-inst.sh

# OPTIMIZATION BREAKS TTRFT API
#CFLAGS += -I$(ROOTDIR)/../linux-2.6.x-petalogix/include
CFLAGS= -O2 -pipe -fno-common -fno-builtin -Wall  -mno-xl-soft-div -mxl-barrel-shift -mcpu=v7.20.d -mxl-multiply-high -mno-xl-soft-mul -mhard-float  -DEMBED -I/home/administrator/petalinux-v2.2-final-full/software/petalinux-dist -I/home/administrator/petalinux-v2.2-final-full/software/petalinux-dist/include -Dlinux -D__linux__ -Dunix -DLINUX -D__CCE__ -Wno-trigraphs -DPERSEUS

CFLAGS += -D_MODULE_IDENT_

# Remove 1st character on next line to compile a debug version (default would be release version)
#CFLAGS += -DDEBUG

BASROOT = projets/bas
EMBEDDED = $(BASROOT)/sdk/embedded
HOST = $(BASROOT)/sdk/host/api
APP = cce

# application include directories
CFLAGS += -I${EMBEDDED}/lib/radio640_lib/inc -I${EMBEDDED}/lib/radio640_lib/src -I${EMBEDDED}/lib/adac250_lib/inc -I${EMBEDDED}/lib/adc5000_lib/inc -I${EMBEDDED}/lib/aurora_lib/inc -I${EMBEDDED}/lib/carrier_lib/inc -I${EMBEDDED}/lib/common/inc -I${EMBEDDED}/lib/drivers_lib/inc -I${EMBEDDED}/lib/FMCEEprom_lib/inc -I${EMBEDDED}/lib/lvds_lib/inc -I${EMBEDDED}/lib/lvdsxinxout_lib/inc -I${EMBEDDED}/lib/mi125_lib/inc -I${EMBEDDED}/lib/mi250_lib/inc -I${EMBEDDED}/lib/mo1000_lib/inc -I${EMBEDDED}/lib/perseus601x_lib/inc -I${EMBEDDED}/lib/perseus611x_lib/inc -I${EMBEDDED}/lib/ppssync_lib/inc -I${EMBEDDED}/lib/radio420_lib/inc -I${EMBEDDED}/lib/sysmon_lib/inc -I${EMBEDDED}/lib/timestamp_lib/inc -I${EMBEDDED}/lib/timestamp_lib/src -I${EMBEDDED}/cce/inc -I${HOST}/eapi/inc  -I${HOST}/rtdex/inc -I${HOST}/record_playback/inc -I${HOST}/rtdexsync/inc -I${HOST}/rtdexsync/src

XILINX_OBJS = ${EMBEDDED}/cce/src/xiic.o ${EMBEDDED}/cce/src/xiic_dyn_master.o ${EMBEDDED}/cce/src/xiic_intr.o ${EMBEDDED}/cce/src/xiic_l.o ${EMBEDDED}/cce/src/xiic_master.o ${EMBEDDED}/cce/src/xiic_multi_master.o ${EMBEDDED}/cce/src/xiic_options.o ${EMBEDDED}/cce/src/xiic_slave.o ${EMBEDDED}/cce/src/xiic_stats.o ${EMBEDDED}/cce/src/xil_assert.o

ADAC250_OBJS = ${EMBEDDED}/cce/src/cce_adac250.o ${EMBEDDED}/lib/adac250_lib/src/adac250_serialize.o  ${EMBEDDED}/lib/adac250_lib/src/adac250.o ${EMBEDDED}/lib/adac250_lib/src/adac250_dac.o ${EMBEDDED}/lib/adac250_lib/src/adac250_adc.o ${EMBEDDED}/lib/adac250_lib/src/adac250_pll.o ${EMBEDDED}/lib/adac250_lib/src/adac250_mux.o

ADC5000_OBJS = ${EMBEDDED}/cce/src/cce_adc5000.o ${EMBEDDED}/lib/adc5000_lib/src/adc5000_serialize.o ${EMBEDDED}/lib/adc5000_lib/src/ad9517.o ${EMBEDDED}/lib/adc5000_lib/src/adc5000.o ${EMBEDDED}/lib/adc5000_lib/src/adt7411.o ${EMBEDDED}/lib/adc5000_lib/src/com_wrapper.o ${EMBEDDED}/lib/adc5000_lib/src/ev10aq190.o

AURORA_OBJS = ${EMBEDDED}/cce/src/cce_aurora.o ${EMBEDDED}/lib/aurora_lib/src/aurora.o

PERSEUS601X_OBJS = ${EMBEDDED}/lib/perseus601x_lib/src/perseus601x.o ${EMBEDDED}/lib/perseus601x_lib/src/perseus601x_clkctrl.o ${EMBEDDED}/lib/perseus601x_lib/src/perseus601x_i2c.o

PERSEUS611X_OBJS = ${EMBEDDED}/lib/perseus611x_lib/src/perseus611x.o ${EMBEDDED}/lib/perseus611x_lib/src/perseus611x_clkctrl.o ${EMBEDDED}/lib/perseus611x_lib/src/perseus611x_i2c.o

DRIVERS_OBJS = ${EMBEDDED}/lib/drivers_lib/src/Lm75.o ${EMBEDDED}/lib/drivers_lib/src/Ina226.o ${EMBEDDED}/lib/drivers_lib/src/DevCom.o ${EMBEDDED}/lib/drivers_lib/src/Ad9361Util.o ${EMBEDDED}/lib/drivers_lib/src/Ad9361DacCore.o ${EMBEDDED}/lib/drivers_lib/src/Ad9361Api.o ${EMBEDDED}/lib/drivers_lib/src/Ad9361AdcCore.o ${EMBEDDED}/lib/drivers_lib/src/Ad9361.o ${EMBEDDED}/lib/drivers_lib/src/AD9511_drv.o ${EMBEDDED}/lib/drivers_lib/src/DAC5682_drv.o ${EMBEDDED}/lib/drivers_lib/src/ADS62P49_drv.o ${EMBEDDED}/lib/drivers_lib/src/LTC2641_drv.o ${EMBEDDED}/lib/drivers_lib/src/EE24LC02B_drv.o ${EMBEDDED}/lib/drivers_lib/src/EEM24128_drv.o ${EMBEDDED}/lib/drivers_lib/src/bas_spi.o 

CARRIER_OBJS =  ${EMBEDDED}/cce/src/cce_carrier.o ${EMBEDDED}/lib/carrier_lib/src/carrier_service.o ${EMBEDDED}/lib/carrier_lib/src/carrier_fmc_service.o

RADIO420_OBJS = ${EMBEDDED}/lib/radio420_lib/src/radio420_cdce62005.o ${EMBEDDED}/lib/radio420_lib/src/fmc_radio.o  ${EMBEDDED}/lib/radio420_lib/src/fmc_radio_calibrate.o ${EMBEDDED}/lib/radio420_lib/src/fmc_spi.o ${EMBEDDED}/lib/radio420_lib/src/lms6002.o ${EMBEDDED}/cce/src/cce_radio420.o ${EMBEDDED}/lib/radio420_lib/src/fmcradio_serialize.o ${EMBEDDED}/lib/radio420_lib/src/calibration_lut.o

MI125_OBJS =  ${EMBEDDED}/cce/src/cce_mi125.o ${EMBEDDED}/lib/mi125_lib/src/mi125_serialize.o ${EMBEDDED}/lib/mi125_lib/src/MI125_com_wrapper.o ${EMBEDDED}/lib/mi125_lib/src/MI125_lm75.o ${EMBEDDED}/lib/mi125_lib/src/MI125_ltm9012.o ${EMBEDDED}/lib/mi125_lib/src/MI125_mi125.o ${EMBEDDED}/lib/mi125_lib/src/MI125_pcf8574.o ${EMBEDDED}/lib/mi125_lib/src/MI125_pca9535.o

MI250_OBJS =  ${EMBEDDED}/cce/src/cce_mi250.o ${EMBEDDED}/lib/mi250_lib/src/mi250.o ${EMBEDDED}/lib/mi250_lib/src/mi250_AD9510_drv.o ${EMBEDDED}/lib/mi250_lib/src/mi250_adc.o ${EMBEDDED}/lib/mi250_lib/src/mi250_pll.o ${EMBEDDED}/lib/mi250_lib/src/mi250_serialize.o

LVDSXINXOUT_OBJS =  ${EMBEDDED}/cce/src/cce_lvdsxinxout.o ${EMBEDDED}/lib/lvdsxinxout_lib/src/fmclvds.o ${EMBEDDED}/lib/lvdsxinxout_lib/src/fmclvds_com_wrapper.o ${EMBEDDED}/lib/lvdsxinxout_lib/src/fmclvds_pcf8574.o

LVDS_OBJS =  ${EMBEDDED}/cce/src/cce_lvds.o ${EMBEDDED}/lib/lvds_lib/src/lvds.o

FMCEEPROM_OBJS =  ${EMBEDDED}/cce/src/cce_fmcvita.o ${EMBEDDED}/lib/FMCEEprom_lib/src/FMCEEprom_eeprom.o ${EMBEDDED}/lib/FMCEEprom_lib/src/FMCEEprom_vita.o ${EMBEDDED}/lib/FMCEEprom_lib/src/FMCEEprom_ifru.o ${EMBEDDED}/lib/FMCEEprom_lib/src/FMCEEprom_subs.o

OSCONFIG_OBJS = ${EMBEDDED}/cce/src/cce_osconfig.o

PPSSYNC_OBJS =  ${EMBEDDED}/lib/ppssync_lib/src/ppssync.o ${EMBEDDED}/cce/src/cce_ppssync.o

SYSMON_OBJS =  ${EMBEDDED}/lib/sysmon_lib/src/sysmon.o ${EMBEDDED}/cce/src/cce_sysmon.o

MO1000_OBJS =  ${EMBEDDED}/cce/src/cce_mo1000.o ${EMBEDDED}/lib/mo1000_lib/src/Mo1000DevCom.o ${EMBEDDED}/lib/mo1000_lib/src/Ad9148.o ${EMBEDDED}/lib/mo1000_lib/src/Mo1000.o ${EMBEDDED}/lib/mo1000_lib/src/Expander.o ${EMBEDDED}/lib/mo1000_lib/src/Cdce62005.o

TIMESTAMP_OBJS =  ${EMBEDDED}/lib/timestamp_lib/src/timestamp.o ${EMBEDDED}/cce/src/cce_timestamp.o

RTDEXSYNC_OBJS =  ${EMBEDDED}/cce/src/cce_rtdexsync.o

FMCOMMS3_OBJS =  ${EMBEDDED}/cce/src/cce_fmcomms3.o ${EMBEDDED}/lib/fmcomms3_lib/src/fmcomms3_serialize.o ${EMBEDDED}/lib/fmcomms3_lib/src/ad9361.o ${EMBEDDED}/lib/fmcomms3_lib/src/ad9361_api.o ${EMBEDDED}/lib/fmcomms3_lib/src/adc_core.o ${EMBEDDED}/lib/fmcomms3_lib/src/dac_core.o ${EMBEDDED}/lib/fmcomms3_lib/src/fmcomms3.o ${EMBEDDED}/lib/fmcomms3_lib/src/platform.o ${EMBEDDED}/lib/fmcomms3_lib/src/util.o

RADIO640_OBJS =  ${EMBEDDED}/cce/src/cce_radio640.o ${EMBEDDED}/lib/radio640_lib/src/Radio640Serialize.o ${EMBEDDED}/lib/radio640_lib/src/Radio640Spi.o ${EMBEDDED}/lib/radio640_lib/src/Radio640DevCom.o ${EMBEDDED}/lib/radio640_lib/src/Radio640.o

XILINX_SPI_OBJS = ${EMBEDDED}/cce/src/xspi.o ${EMBEDDED}/cce/src/xspi_options.o ${EMBEDDED}/cce/src/xspi_selftest.o ${EMBEDDED}/cce/src/xspi_stats.o 

LDFLAGS += -lm -lpthread


# Add any other object files to this list below
APP_OBJS =  ${HOST}/eapi/src/eapi_serialize.o ${HOST}/eapi/src/connection_state.o ${EMBEDDED}/cce/src/bitstream.o ${EMBEDDED}/cce/src/cce_io.o ${EMBEDDED}/cce/src/utils.o ${EMBEDDED}/cce/src/common_utils.o ${EMBEDDED}/cce/src/main.o ${EMBEDDED}/cce/src/cce_mainloop.o 

APP_OBJS += $(ADAC250_OBJS)
APP_OBJS += $(ADC5000_OBJS)
APP_OBJS += $(AURORA_OBJS)
APP_OBJS += $(CARRIER_OBJS)
APP_OBJS += $(DRIVERS_OBJS)
APP_OBJS += $(FMCEEPROM_OBJS)
APP_OBJS += $(LVDS_OBJS)
APP_OBJS += $(LVDSXINXOUT_OBJS)
APP_OBJS += $(MI125_OBJS)
APP_OBJS += $(MI250_OBJS)
APP_OBJS += $(MO1000_OBJS)
APP_OBJS += $(OSCONFIG_OBJS)
APP_OBJS += $(PERSEUS601X_OBJS)
APP_OBJS += $(PERSEUS611X_OBJS)
APP_OBJS += $(PPSSYNC_OBJS)
APP_OBJS += $(RADIO420_OBJS)
APP_OBJS += $(RTDEXSYNC_OBJS)
APP_OBJS += $(SYSMON_OBJS)
APP_OBJS += $(TIMESTAMP_OBJS)
APP_OBJS += $(XILINX_OBJS)
APP_OBJS += $(RADIO640_OBJS)
APP_OBJS += $(XILINX_SPI_OBJS)

#CFLAGS   += -DFMCOMMS3_IN_USE
#CFLAGS   += -I${EMBEDDED}/lib/fmcomms3_lib/inc
#APP_OBJS += $(FMCOMMS3_OBJS)

all: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

clean:
	-rm -f $(APP) *.elf *.gdb *.o

.PHONY: romfs image

romfs:
	$(ROMFSINST) $(APP) /bin/$(APP)
	
	$(ROMFSINST) projets/bas/sdk/embedded/cce/cce.sh /etc/init.d/cce	
	chmod +x $(ROMFSDIR)/etc/init.d/*
	$(ROMFSINST) -s /etc/init.d/cce /etc/rc.d/S91cce

image: romfs
	make -C ${PETALINUX}/software/petalinux-dist image

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


# Targets for the required .config files - if they don't exist, the tree isn't
# configured.  Tell the user this, how to fix it, and exit.
${ROOTDIR}/config.arch ${ROOTDIR}/.config:
	@echo "Error: You must configure the PetaLinux tree before compiling your application"
	@echo ""
	@echo "Change directory to ../../petalinux-dist and 'make menuconfig' or 'make xconfig'"
	@echo ""
	@echo "Once the tree is configured, return to this directory, and re-run make."
	@echo ""
	@exit -1

