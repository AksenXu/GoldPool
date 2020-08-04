CC = g++
AR = ar
CFLAGS  := -Wall -O3 -DHAVE_SYS_UIO_H -Wno-unused-variable -Wno-comment -Wno-unused-function -DHAVE_IOCTL -fpermissive
Include = -I.. -I../include -I../..

LDFLAGS := -L../../libutils -lutils

Sources = system_properties.c property_service.c main.c

OBJS = $(Sources:.c=.o)

BIN=property_service

all: $(OBJS) 
	$(CC) -o $(BIN) $^ $(LDFLAGS)

%.o:%.c
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(Sources)

clean:
	rm -fv $(OBJS) $(BIN)