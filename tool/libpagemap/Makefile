include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)/../lib/libpagemap/include/

WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces 
#-Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -fPIC -shared  -L$(LIBRARY_DIR)
#--------- END OF ENVIRONMENT SETTING -------------
DEP_LIBRARIES := 

LIB_NAME=libpagemap
SRC = 	$(LIBRARY_DIR)/libpagemap/pm_kernel.c \
		$(LIBRARY_DIR)/libpagemap/pm_process.c \
        $(LIBRARY_DIR)/libpagemap/pm_map.c \
        $(LIBRARY_DIR)/libpagemap/pm_memusage.c
                
HEADER = $(shell find . -name "*.h")

OBJ = $(SRC:.c=.o)

all : $(OBJ)
	$(CPP) -o $(LIB_NAME).so $(OBJ) $(LD_FLAGS)
	$(AR) rcs $(LIB_NAME).a $(OBJ)

%.o:%.c $(HEADER)
	$(CPP) $(C_FLAGS) -c -o $@ $<

clean:
	rm -vf $(OBJ) *.o *.a *.so*

install: 
	@cp -avf *.so* *.a $(LIBRARY_DIR)
	@cp -af *.so* ${ROOTFS_DIR}/rootfs/lib/