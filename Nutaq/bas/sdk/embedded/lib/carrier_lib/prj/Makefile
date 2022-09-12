MKNAME := $(CURDIR)/$(lastword $(MAKEFILE_LIST))

#/****************************************************************************  
# *
# *    ****                              I                                    
# *   ******                            ***                                   
# *   *******                           ****                                  
# *   ********    ****  ****     **** *********    ******* ****    ***********
# *   *********   ****  ****     **** *********  **************  *************
# *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
# *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
# *  ****    *********  ****     ****   ****   ****       ****  ****      ****
# *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
# *  ****      ******   ***** ******   *****    ****** *******  ****** *******
# *  ****        ****   ************    ******   *************   *************
# *  ****         ***     ****  ****     ****      *****  ****     *****  ****
# *                                                                       ****
# *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
# *                                                                        ***
# *
# ************************************************************************//**
# *
# * @file       Makefile
# * @brief      carrier_lib module related makefile
# *
# * $Date: 2015/10/09 14:25:57 $
# * $Revision: 1.6 $
# *
# * (c) 2013, Nutaq Inc. All rights reserved.              
# *
# ***************************************************************************/
#============================================================================*/
#                			WARNING
# IF YOU HAVE TO CHANGE THIS FILE, BE SURE THAT YOUR
# TEXT EDITOR WILL NOT REPLACE THE TABS WITH SPACES.
# OTHERWISE THE Makefile WILL ISSUE ERRORS.
#============================================================================*/
#============================================================================*/

PROJECTNAME=carrier_lib
CC=mb-gcc
ARCHIVER=mb-ar
COMPILER_FLAGS_DEBUG=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -c -Wall
COMPILER_FLAGS_RELEASE=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -O2 -c -Wall
DEBUG_FLAG=-g
EXTRA_COMPILER_FLAGS = -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.50.c -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt -DSET_CARRIER_PERSEUS
BINDEBUG=libcarrier_standaloned.a
BINRELEASE=libcarrier_standalone.a

BINDIR=../bin
XPATH:=$(subst \,/,$(XILINX_EDK_ROOT))

INCLUDES = -I../inc -I../../../../host/api/eapi/inc -I../../drivers_lib/inc -I$(XPATH)/sw/XilinxProcessorIPLib/drivers/iic_v2_00_a/src -I$(XPATH)/sw/XilinxProcessorIPLib/drivers/cpu_v1_12_b/src -I$(XPATH)/sw/XilinxProcessorIPLib/drivers/common_v1_00_a/src -I$(XPATH)/sw/lib/bsp/standalone_v3_00_a/src/common -I$(XPATH)/sw/lib/bsp/standalone_v3_00_a/src/microblaze -I../../perseus601x_lib/inc  -I../../FMCEEprom_lib/inc
SOURCES=	../src/carrier_service.c ../src/carrier_fmc_service.c
LIBDIR=../bin
OBJS=$(SOURCES:.c=.o)
CFLAGS=$(COMPILER_FLAGS_DEBUG) $(DEBUG_FLAG) $(EXTRA_COMPILER_FLAGS) $(INCLUDES)

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
	rm -rf ${OUTS}

lib_release: $(OBJS)
	rm -rf ${LIBDIR}/${BINRELEASE} 
	$(ARCHIVER) cru ${LIBDIR}/${BINRELEASE} ${OUTS}
	mb-ranlib ${LIBDIR}/${BINRELEASE}
	rm -rf ${OUTS}

