SDKROOT = /opt/Nutaq/bas
SDKEMB  = /$(SDKROOT)/sdk/embedded/lib
SDKHOST = /$(SDKROOT)/sdk/host/api

LIBS = -leapi -lm -lrt

CFLAGS = -I../inc -I $(SDKEMB)/common/inc -I $(SDKEMB)/radio640_lib/inc -I $(SDKEMB)/FMCEEprom_lib/inc -I $(SDKEMB)/drivers_lib/inc -I $(SDKEMB)/perseus601x_lib/inc -I $(SDKEMB)/timestamp_lib/inc -I $(SDKEMB)/carrier_lib/inc -I $(SDKHOST)/eapi/inc -I $(SDKHOST)/utils/inc -I $(SDKHOST)/lyt_std_lib/inc  -Werror-implicit-function-declaration

vpath %.a = $(SDKROOT)/sdk/host/lib64

VPATH = ../src $(SDKHOST)/utils/src

BIN_DIR := ../../../../bin

SRC := linux_utils.c TimeGate.c ConfigFile.c Main.c CustomRegister_Write.c

OUT := TimeGate

all: ${BIN_DIR}/${OUT}

${BIN_DIR}/${OUT}: ${SRC} ${LIBS}
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f ${BIN_DIR}/${OUT}
