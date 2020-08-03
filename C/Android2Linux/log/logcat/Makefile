CC = g++
AR = ar
CFLAGS  := -Wall -O3 -DHAVE_SYS_UIO_H -Wno-unused-variable -Wno-comment -Wno-unused-function -DHAVE_IOCTL
Include = -I../liblog

LDFLAGS :=  -L../liblog -llog

Sources = logcat.cpp

OBJS = $(Sources:.cpp=.o)
#$(info OBJS=$(OBJS))

BIN=logcat

all: $(OBJS) 
	$(CC) -o $(BIN) $^ $(LDFLAGS)

%.o:%.cpp
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(Sources)

clean:
	rm -fv $(OBJS) $(BIN)