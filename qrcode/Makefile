
CC = gcc
AR = ar

CFLAGS = -I./src \
		 -I./inc \
         -I./src/decoder \
         -I./src/qrcode \
         -Wno-pragmas

COMMON_OBJS =  src/qrcode_api.o \
				src/image.o \
				src/img_scanner.o \
				src/symbol.o \
				src/scanner.o \
				src/config.o \
				src/refcnt.o \
				src/error.o \
				src/decoder.o \
				src/decoder/qr_finder.o \
				src/qrcode/bch15_5.o \
				src/qrcode/binarize.o \
				src/qrcode/isaac.o \
				src/qrcode/qrdec.o \
				src/qrcode/qrdectxt.o \
				src/qrcode/rs.o \
				src/qrcode/util.o

EXRTC_OBJS = $(COMMON_OBJS)
SRCS = $(EXRTC_OBJS:.o=.c)

.PHONY: all clean

all: bin

$(EXRTC_OBJS):

libqrcode.a: $(EXRTC_OBJS)
	$(AR) rcs $@ $^

bin : libqrcode.a
	gcc -o qrcode_test test.c -L. -lqrcode $(CFLAGS)

clean:
	find . -name *.o | xargs rm