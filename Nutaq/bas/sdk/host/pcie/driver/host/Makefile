DEBUG=n
#DEBUG=y

ifeq ($(DEBUG),y)
 DEBFLAGS= -O -g -DPERSEUS_DEBUG # -O needed for inline expension
else
 DEBFLAGS= -O2
endif

ccflags-y  += $(DEBFLAGS)


obj-m := host_drv.o
