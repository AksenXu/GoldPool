include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR) -I$(LIBRARY_DIR)/libpagemap/include
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wno-unused-variable
COMPILE_OPTS	= $(INCLUDES) -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	=
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -L$(LIBRARY_DIR) -Wl,-rpath-link=$(LIBRARY_DIR) -lpagemap
#--------- END OF ENVIRONMENT SETTING -------------


#--------- Compiling -------------------
BIN = procmem
SRC = $(APP_DIR)/procmem/procmem.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean install

all: $(BIN)

$(BIN): $(OBJ)
	$(CPP)  -o $@ $(OBJ) $(LD_FLAGS)
	$(NM) -n $@ > $@.sym
	$(STRIP) $@
	$(OBJCOPY) -R .comment -R .note.ABI-tag -R .gnu.version $@

%.o:%.c $(HEADER)
	$(CPP) $(C_FLAGS) -c -o $@ $<

clean:
	rm -vf $(BIN) $(OBJ) $(BIN).sym *.o *.a *.so*


install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@cp -avf ${BIN} ${ROOTFS_DIR}/rootfs/bin/
