obj-m+=query.o
INCLUDES = -I./build/SimpleLogger -I./build/inih -I./build/uthash/src
CCFLAGS = -g -O2 -fpic
CCC = gcc
LIB = -lprovenance -lpthread -lz -lrt
.SUFFIXES: .c

dependencies:
	mkdir -p build
	cd build && git clone https://github.com/camflow/camflow-dev
	cd build/camflow-dev && git checkout dev
	cd build/camflow-dev && make prepare_kernel
	cd build/camflow-dev && make config_travis
	cd build/camflow-dev && make compile_kernel
	cd build/camflow-dev && make install_kernel

build:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	sudo $(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install

build_travis:
	$(MAKE) -C /lib/modules/$(shell cd build/camflow-dev/scripts && ruby version.rb)/build/ M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

load:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	sudo $(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install
	sudo modprobe query

unload:
	sudo rmmod query

reload:
	sudo modprobe -r query
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	sudo $(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install
	sudo modprobe query

start:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	sudo $(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install
	sudo modprobe query

install:
	cd ./include && sudo $(MAKE) install
