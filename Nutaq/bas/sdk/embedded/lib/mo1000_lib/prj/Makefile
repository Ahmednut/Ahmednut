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
#==          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       **** 
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
#============================================================================
#============================================================================
#==                          D E S C R I P T I O N
#============================================================================
#==
#==	This is a Makefile to generate the mo1000_lib (release and debug)
#==
#==
#============================================================================
#                			WARNING
# IF YOU HAVE TO CHANGE THIS FILE, BE SURE THAT YOUR
# TEXT EDITOR WILL NOT REPLACE THE TABS WITH SPACES.
# OTHERWISE THE Makefile WILL ISSUE ERRORS.
#============================================================================*/
#============================================================================*/

PROJECTNAME=mo1000_lib
CC=mb-gcc
ARCHIVER=mb-ar
COMPILER_FLAGS_DEBUG=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -DDEBUG -c -Wall
COMPILER_FLAGS_RELEASE=-DCFG_MICROBLAZE_COMPILATOR -DSTANDALONE -O2 -c -Wall
DEBUG_FLAG=-g
EXTRA_COMPILER_FLAGS = -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v8.50.c -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt
BINDEBUG=libmo1000_standaloned.a
BINRELEASE=libmo1000_standalone.a

BINDIR=../bin
XPATH:=$(subst \,/,$(XILINX_EDK_ROOT))

INCLUDES = -I../inc -I../../../../host/api/eapi/inc -I../../carrier_lib/inc -I../../FMCEEprom_lib/inc -I../../drivers_lib/inc -I../../common/inc
SOURCES=	../src/Mo1000DevCom.c ../src/Ad9148.c ../src/Expander.c ../src/Mo1000.c ../src/Cdce62005.c
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

