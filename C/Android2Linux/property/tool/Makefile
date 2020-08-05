CC = g++
AR = ar
CFLAGS  := -Wall -O3 -DHAVE_SYS_UIO_H -Wno-unused-variable -Wno-comment -Wno-unused-function -DHAVE_IOCTL -fpermissive
Include = -I../ -I../../

LDFLAGS :=  -L../libprop -lprop -L../../libutils -lutils

Sources = setprop.c getprop.c

OBJS = $(Sources:.c=.o)

SETPROP=setprop
GETPROP=getprop

all: $(SETPROP) $(GETPROP)

setprop :
	$(CC) -o setprop setprop.c $(CFLAGS) $(LDFLAGS) $(Include)
    
getprop :
	$(CC) -o getprop getprop.c $(CFLAGS) $(LDFLAGS) $(Include)

clean:
	rm -fv $(GETPROP) $(SETPROP) *.o