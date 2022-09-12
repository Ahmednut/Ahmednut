SDKROOT = /opt/Nutaq/bas
SDKEMB  = /$(SDKROOT)/sdk/embedded/lib
SDKHOST = /$(SDKROOT)/sdk/host/api

LIBS = -lrtdex -leapi -lpthread -lpcap -lm -lrt

CFLAGS = -I../inc -I $(SDKEMB)/common/inc -I $(SDKEMB)/carrier_lib/inc -I $(SDKHOST)/eapi/inc -I $(SDKHOST)/rtdex/inc -I $(SDKHOST)/record_playback/inc -I $(SDKHOST)/utils/inc -Werror-implicit-function-declaration

vpath %.a = $(SDKROOT)/sdk/host/lib64

VPATH = ../src $(SDKHOST)/utils/src

BIN_DIR := ../../../../bin

SRC := bas_buffer.c bas_semaphore.c bas_thread.c linux_utils.c mac_utils.c media_utils.c terminal_utils.c TxStreaming.c ConfigFile.c Main.c

OUT := TxStreaming

all: ${BIN_DIR}/${OUT}

${BIN_DIR}/${OUT}: ${SRC} ${LIBS}
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f ${BIN_DIR}/${OUT}
