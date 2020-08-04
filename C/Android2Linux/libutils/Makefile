CC = g++
AR = ar
CFLAGS  := -Wall -O3 -Wno-unused-variable -Wno-comment -Wno-unused-function
Include = -I../

Sources =   memory.c

OBJS = $(Sources:.c=.o)
$(info OBJS=$(OBJS))

static_lib=libutils.a
shared_lib=libutils.so

all: $(static_lib) $(shared_lib)

$(static_lib): $(OBJS)
	$(AR) rcs $(static_lib) $@ $^

$(shared_lib): $(OBJS)
	$(CC) -fPIC -shared -o $@ $^

%.o:%.c
	$(CC) -g -c -Os -fPIC $(Include) $(CFLAGS) $(Sources)

clean:
	rm -fv $(OBJS) $(static_lib) $(shared_lib)