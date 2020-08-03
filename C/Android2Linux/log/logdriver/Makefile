obj-m := logger.o 
KERNELDIR := /lib/modules/4.4.0-142-generic/build
PWD := $(shell pwd) 

modules: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

modules_install: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install 

clean:
	rm -fv *.ko *.o modules.order Module.symvers *.mod.c
    