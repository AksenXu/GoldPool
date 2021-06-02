CC = gcc
AR = ar
CFLAGS  := -Wall -O3 -Wno-unused-variable -Wno-comment -Wno-unused-function -fpermissive
Include = -I. 
LDFLAGS :=  -L. -lm

libSrc =   interf_enc.c \
            sp_enc.c 
            
libObj = $(libSrc:.c=.o)
libName = libamrencoder

static_lib=$(libName).a
shared_lib=$(libName).so

all: $(static_lib) $(shared_lib) BIN

BIN:
	gcc -o amrencode_test test.cpp -L. -lamrencoder -lm $(Include)

$(static_lib): $(libObj)
	$(AR) rcs $(static_lib) $@ $^

$(shared_lib): $(libObj)
	$(CC) -fPIC -shared -o $@ $^ -lm

%.o:%.c
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(libSrc)

clean:
	rm -fv $(libObj) $(static_lib) $(shared_lib)