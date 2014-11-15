include $(MKCONF_DIR)/subdir.cfg
include  $(MKCONF_DIR)/Cortex-M.mk
include $(MKCONF_DIR)/$(BOARD)/$(APP).mk

O_TARGET = $(obj)/api.o

SOBJS-y :=
#COBJS-y	:= mpos_api_lcd.o mpos_api_tlv.o mpos_api_alg.o 
COBJS-y	:= mpos_api_lcd.o mpos_api_tlv.o


COBJS-$(CONFIG_FRAME_NL) += mpos_api_frame.o
COBJS-$(CONFIG_FRAME_LKL) += mpos_api_frame_lkl.o
COBJS-$(CONFIG_NDK_PATCH) += mpos_api_patch.o
#COBJS-$(CONFIG_BT) += mpos_api_bt.o


SRCS	:= $(COBJS-y:.o=.c)

OBJS 	:= $(addprefix $(obj)/, $(COBJS-y))

all: $(O_TARGET)

$(O_TARGET): $(OBJS) 
	$(call cmd_link_o_target, $(OBJS))

include $(MKCONF_DIR)/rules.cfg
