obj-m := esprimo.o
KVERSION := $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules

clean:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean

insmod: esprimo.ko
	cp esprimo.ko ~
	sudo insmod ~/esprimo.ko

rmmod: ~/esprimo.ko
	sudo rmmod ~/esprimo.ko