obj-m += usbkey.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm-f usbDaemon

daemon:
	gcc -Wall -o usbkeyd usbDaemon.c