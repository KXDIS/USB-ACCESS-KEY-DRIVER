obj-m += usbkey.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

daemon:
	gcc -Wall -o usbkeyd usbDaemon.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f usbkeyd