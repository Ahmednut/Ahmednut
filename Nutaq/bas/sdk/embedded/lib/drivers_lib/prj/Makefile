MKNAME := $(CURDIR)/$(lastword $(MAKEFILE_LIST))

#=============================================================================
#== 
#==    ****                              I                                     
#==   ******                            ***                                    
#==   *******                           ****                                   
#==   ********    ****  ****     **** *********    ******* ****    *********** 
#==   *********   ****  ****     **** *********  **************  ************* 
#==   **** *****  ****  ****     ****   ****    *****    ****** *****     **** 
#==   ****  ***** ****  ****     ****   ****   *****      ****  ****      **** 
#==  ****    *********  ****     ****   ****   ****       ****  ****      **** 
#==  ****     ********  ****    ****I  ****    *****     *****  ****      **** 
#==  ****      ******   ***** ******   *****    ****** *******  ****** ******* 
#==  ****        ****   ************    ******   *************   ************* 
#==  ****         ***     ****  ****     ****      *****  ****     *****  **** 
#==                                                                       **** 
#==          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
#==                                                                        ***       
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
#==	This is a Makefile to generate the adac250_lib (release and debug)
#==
#==
#============================================================================*/
#                			WARNING
# IF YOU HAVE TO CHANGE THIS FILE, BE SURE THAT YOUR
# TEXT EDITOR WILL NOT REPLACE THE TABS WITH SPACES.
# OTHERWISE THE Makefile WILL ISSUE ERRORS.
#============================================================================*/
#============================================================================*/

PROJECTNAME=drivers_lib
CC=mb-gcc
ARCHIVER=mb-ar
COMPILER_FLAGS_DEBUG=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -c
COMPILER_FLAGS_RELEASE=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -O2 -c -Wall
DEBUG_FLAG=-g
EXTRA_COMPILER_FLAGS = -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.50.c -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt
BINDEBUG=libdrivers_standaloned.a
BINRELEASE=libdrivers_standalone.a

ISLIB=1
LEVEL=0

BINDIR=../bin
XPATH:=$(subst \,/,$(XILINX_EDK_ROOT))

INCLUDES = -I../inc -I../../../../host/api/eapi/inc -I../../carrier_lib/inc -I../../FMCEEprom_lib/inc -I../../common/inc 
SOURCES= ../src/ADS62P49_drv.c  ../src/AD9511_drv.c ../src/DAC5682_drv.c  ../src/bas_spi.c ../src/LTC2641_drv.c ../src/EE24LC02B_drv.c ../src/EEM24128_drv.c
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
#	rm -rf ${LIBDIR}/${BINDEBUG} 
	$(ARCHIVER) cru ${LIBDIR}/${BINDEBUG} ${OUTS}
	mb-ranlib ${LIBDIR}/${BINDEBUG}
	rm	-rf	${OUTS}

lib_release: $(OBJS)
#	rm -rf ${LIBDIR}/${BINRELEASE}
	$(ARCHIVER) cru ${LIBDIR}/${BINRELEASE} ${OUTS}
	mb-ranlib ${LIBDIR}/${BINRELEASE}
	rm	-rf	${OUTS}
