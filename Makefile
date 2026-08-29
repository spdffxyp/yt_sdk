# 如果传入了 KVER 则使用 /lib/modules/$(KVER)/build，否则默认当前运行内核
KVER ?= $(shell uname -r)
KDIR ?= /lib/modules/$(KVER)/build
PWD  := $(shell pwd)

# 声明需要编译生成的两个模块
obj-m += yt921x.o
obj-m += tag_yt921x.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
