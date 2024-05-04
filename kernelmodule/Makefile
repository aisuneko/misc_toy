obj-m += aisutestmodule.o show_all_kernel_thread.o show_task_family.o
PWD := $(CURDIR)

KERNEL_VER = $(shell uname -r)
all:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean
