include $(MKCONF_DIR)/subdir.cfg
include  $(MKCONF_DIR)/Cortex-M.mk
include $(MKCONF_DIR)/$(BOARD)/$(APP).mk

O_TARGET = $(obj)/cmd.o

SOBJS-y :=
#COBJS-y	:= main.o mpos_command.o wdebug.o oringin_send.o
COBJS-y	:= main.o mpos_command.o wdebug.o

COBJS-$(CONFIG_AUTH) += mpos_cmd_auth.o   
COBJS-$(CONFIG_ME30LKL) += mpos_cmd_me30.o


SRCS	:= $(COBJS-y:.o=.c)

OBJS 	:= $(addprefix $(obj)/, $(COBJS-y))

all: $(O_TARGET)

$(O_TARGET): $(OBJS) 
	$(call cmd_link_o_target, $(OBJS))

include $(MKCONF_DIR)/rules.cfg
