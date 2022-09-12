SDKROOT = /opt/Nutaq/bas
SDKEMB  = /$(SDKROOT)/sdk/embedded/lib
SDKHOST = /$(SDKROOT)/sdk/host/api

LIBS = -lrecplay -leapi -lrtdex -lpthread -lpcap -lm -lrt

CFLAGS = -I../inc -I $(SDKEMB)/common/inc -I $(SDKEMB)/carrier_lib/inc -I $(SDKHOST)/eapi/inc -I $(SDKHOST)/rtdex/inc -I $(SDKHOST)/record_playback/inc -I $(SDKHOST)/utils/inc -Werror-implicit-function-declaration

vpath %.a = $(SDKROOT)/sdk/host/lib64

VPATH = ../src $(SDKHOST)/utils/src

BIN_DIR := ../../../../bin

SRC := linux_utils.c PlaybackData.c Main.c

OUT := PlaybackData

all: ${BIN_DIR}/${OUT}

${BIN_DIR}/${OUT}: ${SRC} ${LIBS}
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f ${BIN_DIR}/${OUT}
