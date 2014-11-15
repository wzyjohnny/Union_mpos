 MKCONF_DIR = configs
 LIBC_DIR = libs


include $(MKCONF_DIR)/compile.cfg
include $(MKCONF_DIR)/subdir.cfg

export CPPFLAGS += -I$(TOPDIR)/inc


Target = build/$(APP)_$(BOARD)
Middle = $(obj)/$(APP)_$(BOARD)

SOBJS-y :=
COBJS-y	:= 

DIRS-y += api
DIRS-y += cmd
DIRS-y += app

LIBS-y = $(obj)/api/api.o
LIBS-y += $(obj)/cmd/cmd.o
LIBS-y += $(obj)/app/app.o


SRCS	:= $(COBJS-y:.o=.c)
OBJS 	:= $(addprefix $(obj)/, $(COBJS-y))
OBJS	+= $(LIB_API) 
OBJS	+= $(LIBS-y) 

all: $(DIRS-y)  $(Target).bin $(Target).hex

$(DIRS-y): depend
	$(MAKE) -C $@
$(Target).bin:$(Middle).elf
	$(OBJCOPY) -S -O binary $< $@

$(Target).hex:$(Middle).elf
	$(OBJCOPY) -S -O ihex $< $@

$(Middle).elf:  $(OBJS) $(LINKSCRIPT)  $(MKCONF_DIR)/$(BOARD)/$(APP).mk
	$(LD)  $(OBJS)  $(LDFLAGS) -Map $(Middle).map -o $@

depend:


.PHONY:clean
clean:
	-@$(RMDIR) $(BUILD_DIR)
