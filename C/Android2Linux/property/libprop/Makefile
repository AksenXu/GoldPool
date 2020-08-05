CC = g++
AR = ar
CFLAGS  := -Wall -O3 -Wno-unused-variable -Wno-comment -Wno-unused-function -DHAVE_LIBC_SYSTEM_PROPERTIES -DHAVE_SYS_UIO_H -DHAVE_IOCTL -fpermissive

Include = -I../.. -I. -I../

Sources =   properties.c system_properties.c property_service.c

LDFLAGS := -L../../libutils -lutils

OBJS = $(Sources:.c=.o)
$(info OBJS=$(OBJS))

static_lib=libprop.a
shared_lib=libprop.so

all: $(static_lib) $(shared_lib)

$(static_lib): $(OBJS)
	$(AR) rcs $(static_lib) $@ $^

$(shared_lib): $(OBJS)
	$(CC) -fPIC -shared -o $@ $^

%.o:%.c
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(Sources)

clean:
	rm -fv $(OBJS) $(static_lib) $(shared_lib)