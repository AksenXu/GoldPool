CC = g++
AR = ar
CFLAGS  := -Wall -O3 -DHAVE_SYS_UIO_H -Wno-unused-variable -Wno-comment -Wno-unused-function -DHAVE_IOCTL -fpermissive
Include = -I.. -I../libprop -I../..

LDFLAGS := -L../libprop -lprop -L../../libutils -lutils

Sources = main.c

OBJS = $(Sources:.c=.o)

BIN=property_service

all: $(OBJS) 
	$(CC) -o $(BIN) $^ $(LDFLAGS)

%.o:%.c
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(Sources)

clean:
	rm -fv $(OBJS) $(BIN)