CC = g++
AR = ar
CFLAGS  := -Wall -O3 -DHAVE_SYS_UIO_H -Wno-unused-variable -Wno-comment -Wno-unused-function
Include = -I.

Sources =   logprint.c      \
            logd_write.c    \
            event_tag_map.c \

OBJS = $(Sources:.c=.o)
$(info OBJS=$(OBJS))

static_lib=liblog.a
shared_lib=liblog.so

all: $(static_lib) $(shared_lib)

$(static_lib): $(OBJS)
	$(AR) rcs $(static_lib) $@ $^

$(shared_lib): $(OBJS)
	$(CC) -fPIC -shared -o $@ $^

%.o:%.c
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(Sources)

clean:
	rm -fv $(OBJS) $(static_lib) $(shared_lib)