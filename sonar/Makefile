obj-m = sensor_module.o sensor2_module.o
KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.c .led.* Module.symvers modules.order
