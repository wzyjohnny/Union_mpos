CROSS_COMPILE := arm-none-eabi-
#CROSS_COMPILE = "F:\Programs\GNU Tools ARM Embedded\4.7 2012q4\bin\arm-none-eabi-"
AS      := $(CROSS_COMPILE)as
LD      := $(CROSS_COMPILE)ld
CC      := $(CROSS_COMPILE)gcc
CPP     := $(CC) -E
AR      := $(CROSS_COMPILE)ar
NM      := $(CROSS_COMPILE)nm
STRIP   := $(CROSS_COMPILE)strip
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
RANLIB  := $(CROSS_COMPILE)RANLIB
SIZE    := $(CROSS_COMPILE)size -d

export CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB SIZE LD

ARCH_CFLAGS := -mthumb -Wall -Os -fno-common
ARCH_LDFLAGS :=