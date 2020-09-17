CC = g++
AR = ar
CFLAGS  := -Wall -O3 -DHAVE_SYS_UIO_H -Wno-unused-variable -Wno-comment -Wno-unused-function
Include = -I. -I./inc

Sources = src/socket.cpp

OBJS = $(Sources:.cpp=.o)
$(info OBJS=$(OBJS))

module_name=libutils
static_lib=$(module_name).a
shared_lib=$(module_name).so

all: $(static_lib) $(shared_lib)

$(static_lib): $(OBJS)
	$(AR) rcs $(static_lib) $@ $^

$(shared_lib): $(OBJS)
	$(CC) -fPIC -shared -o $@ $^

install:
	sudo cp $(shared_lib) /usr/lib

%.o:%.cpp
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) -o $@ $<

clean:
	rm -fv $(OBJS) $(static_lib) $(shared_lib)
