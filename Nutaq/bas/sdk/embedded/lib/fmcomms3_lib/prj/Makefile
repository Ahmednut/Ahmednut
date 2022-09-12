MKNAME := $(CURDIR)/$(lastword $(MAKEFILE_LIST))

PROJECTNAME=fmcomms3_lib
CC=mb-gcc
ARCHIVER=mb-ar
COMPILER_FLAGS_DEBUG=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -c -Wall
COMPILER_FLAGS_RELEASE=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -O2 -c -Wall
DEBUG_FLAG=-g
EXTRA_COMPILER_FLAGS = -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.50.c -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt
BINDEBUG=libfmcomms3_standaloned.a
BINRELEASE=libfmcomms3_standalone.a

BINDIR=../bin
XPATH:=$(subst \,/,$(XILINX_EDK_ROOT))

SPI_SRC_PATH:=$(XPATH)/sw/XilinxProcessorIPLib/drivers/spi_v3_03_a/src

INCLUDES = -I../inc -I../../../../host/api/eapi/inc -I../../common/inc -I$(XPATH)/sw/XilinxProcessorIPLib/drivers/spi_v3_03_a/src -I$(XPATH)/sw/XilinxProcessorIPLib/drivers/cpu_v1_12_b/src -I$(XPATH)/sw/XilinxProcessorIPLib/drivers/common_v1_00_a/src -I$(XPATH)/sw/lib/bsp/standalone_v3_03_a/src/common -I$(XPATH)/sw/lib/bsp/standalone_v3_03_a/src/microblaze
SOURCES=	../src/util.c ../src/platform.c ../src/fmcomms3.c ../src/dac_core.c ../src/adc_core.c ../src/ad9361_api.c ../src/ad9361.c $(SPI_SRC_PATH)/xspi.c $(SPI_SRC_PATH)/xspi_options.c $(SPI_SRC_PATH)/xspi_selftest.c $(SPI_SRC_PATH)/xspi_stats.c
LIBDIR=../bin
OBJS=$(SOURCES:.c=.o)

OUTS = *.o

.PHONY : bins
.PHONY : clean
.PHONY : lib_debug
.PHONY : lib_release

bins:
		gmkdir -p ${LIBDIR}
		$(MAKE) -C $(BINDIR) -f $(MKNAME) lib_debug
		$(MAKE) -C $(BINDIR) -f $(MKNAME) lib_release
clean:
		rm	-rf	$(BINDIR)/${OUTS}

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(@F)

lib_debug: CFLAGS=$(COMPILER_FLAGS_DEBUG) $(DEBUG_FLAG) $(EXTRA_COMPILER_FLAGS) $(INCLUDES)

lib_release: CFLAGS=$(COMPILER_FLAGS_RELEASE) $(EXTRA_COMPILER_FLAGS) $(INCLUDES)


lib_debug: $(OBJS)
	rm -rf ${LIBDIR}/${BINDEBUG} 
	$(ARCHIVER) cru ${LIBDIR}/${BINDEBUG} ${OUTS}
	mb-ranlib ${LIBDIR}/${BINDEBUG}
	rm	-rf	${OUTS}

lib_release: $(OBJS)
	rm -rf ${LIBDIR}/${BINRELEASE} 
	$(ARCHIVER) cru ${LIBDIR}/${BINRELEASE} ${OUTS}
	mb-ranlib ${LIBDIR}/${BINRELEASE}
	rm	-rf	${OUTS}

