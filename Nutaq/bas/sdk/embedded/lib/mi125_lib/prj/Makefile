MKNAME := $(CURDIR)/$(lastword $(MAKEFILE_LIST))

#/*============================================================================
#============================================================================
#==
#==          **  **     **  ******  ********  ********  ********  **    **
#==         **    **   **  **   ** ********  ********  ********  **    **
#==        **     *****   **   **    **     **        **        **    **
#==       **       **    ******     **     ****      **        ********
#==      **       **    **  **     **     **        **        **    **
#==     *******  **    **   **    **     ********  ********  **    **
#==    *******  **    **    **   **     ********  ********  **    **
#==
#==
#==
#==   Project     : MI125
#==
#== 						Copyright (c) Lyrtech RD inc. 2012
#==
#==============================================================================
#==
#==   MODULE NAME         : Makefile
#==   PUBLIC PREFIX       :
#==
#==   COMPILER VERSION OS :
#==   PROCESSOR           :
#==
#============================================================================*/
#/*==========================================================================
#==                          D E S C R I P T I O N
#============================================================================
#==
#==	This is a Makefile to generate the mi125_lib (release and debug)
#==
#==
#============================================================================*/
#                			WARNING
# IF YOU HAVE TO CHANGE THIS FILE, BE SURE THAT YOUR
# TEXT EDITOR WILL NOT REPLACE THE TABS WITH SPACES.
# OTHERWISE THE Makefile WILL ISSUE ERRORS.
#============================================================================*/
#============================================================================*/

PROJECTNAME=mi125_lib
CC=mb-gcc
ARCHIVER=mb-ar
COMPILER_FLAGS_DEBUG=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -c -Wall -DDEBUG
COMPILER_FLAGS_RELEASE=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -O2 -c -Wall
DEBUG_FLAG=-g
EXTRA_COMPILER_FLAGS = -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.50.c -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt
BINDEBUG=libmi125_standaloned.a
BINRELEASE=libmi125_standalone.a

BINDIR=../bin
XPATH:=$(subst \,/,$(XILINX_EDK_ROOT))

INCLUDES = -I../inc -I../../../../host/api/eapi/inc -I../../carrier_lib/inc -I../../FMCEEprom_lib/inc
SOURCES=	../src/MI125_com_wrapper.c ../src/MI125_ltm9012.c ../src/MI125_lm75.c ../src/MI125_pcf8574.c ../src/MI125_mi125.c ../src/MI125_pca9535.c
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

