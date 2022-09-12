SDKROOT =../../../../../../bas
SDKEMB  = $(SDKROOT)/sdk/embedded/lib
SDKHOST = $(SDKROOT)/sdk/host/api
SDKMAKE = $(SDKROOT)/sdk/host/api/wdxg/prj_linux

CC=gcc
CFLAGS=-I $(SDKROOT)/sdk/host/api/wdxg/inc -I $(SDKROOT)/sdk/host/api/eapi/inc -I $(SDKROOT)/sdk/host/api/wdxg/inc/include-unix -I $(SDKEMB)/common/inc -I $(SDKEMB)/carrier_lib/inc -I $(SDKHOST)/utils/inc -I $(SDKHOST)/wdxg/inc -I $(SDKEMB)/adac250_lib/inc -I $(SDKROOT)/sdk/host/api/wdxg/src  -Werror-implicit-function-declaration

#SRC= /opt/Nutaq/bas/sdk/host/api/wdxg/src/wdxg.c $(SDKHOST)/utils/src/linux_utils.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_api.c $/opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_sweep_device.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_probe.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_lib.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_dsp.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_debug.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_commons.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/wsa_client.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/kiss_fftr.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/kiss_fft.c /opt/Nutaq/bas/sdk/host/api/wdxg/src/src-unix/wsa_client_unix.c

SRC= $(SDKROOT)/sdk/host/api/wdxg/src/wdxg.c $(SDKHOST)/utils/src/linux_utils.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_api.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_lib.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_commons.c $(SDKROOT)/sdk/host/api/wdxg/src/src-unix/wsa_client_unix.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_debug.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_probe.c $(SDKROOT)/sdk/host/api/wdxg/src/kiss_fft.c $(SDKROOT)/sdk/host/api/wdxg/src/kiss_fftr.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_client.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_dsp.c $(SDKROOT)/sdk/host/api/wdxg/src/wsa_sweep_device.c


LDFLAGS=-L $(SDKROOT)/sdk/host/api/wdxg/src
LDLIBS= -lm -lwsa32 

OBJ=${SRC:.c=.o}
OUT=$(SDKROOT)/sdk/host/lib64/libwdxg.a


.c.o: ${SRC} ${LDLIBS}
	${CC} ${CFLAGS} -Wall -c $< -o $@	


{SDKMAKE}/{BINLIB}: ${OBJ}  
	${AR} cru ${OUT} ${OBJ} 
	$(RANLIB) ${BINLIB}






         












