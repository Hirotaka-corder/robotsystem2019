obj-m:= myledshingou.o

myledshingou.ko: myledshingou.c
	make -C /usr/src/linux M=`pwd` V=1 modules
clean:
	make -C /usr/src/linux M=`pwd` V=1 clean

