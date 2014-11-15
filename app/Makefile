include $(MKCONF_DIR)/subdir.cfg
include  $(MKCONF_DIR)/Cortex-M.mk
include $(MKCONF_DIR)/$(BOARD)/$(APP).mk

O_TARGET = $(obj)/app.o

SOBJS-y :=
COBJS-y	:= mpos_cmd_$(APP).o

COBJS-$(CONFIG_PRINTER_APP) += mpos_cmd_prn_$(APP).o
COBJS-$(CONFIG_PBOC_APP) += mpos_cmd_pboc_$(APP).o


SRCS	:= $(COBJS-y:.o=.c)

OBJS 	:= $(addprefix $(obj)/, $(COBJS-y))

all: $(O_TARGET)

$(O_TARGET): $(OBJS) 
	$(call cmd_link_o_target, $(OBJS))

include $(MKCONF_DIR)/rules.cfg
