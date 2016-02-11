CFLAGS= -std=c11 -Wall -Werror -Wextra -Wpedantic -Wwrite-strings -Wno-deprecated -Wstack-usage=1024 -Wfloat-equal  -Waggregate-return -Winline

CC = gcc

TARGET=intersect

OBJS1=intersect.o 

.PHONY: clean debug profile 

$(TARGET): $(OBJS1) -lm

debug: CFLAGS+=-g
debug: $(TARGET)


clean:
	-$(RM) $(TARGET) $(OBJS1) 

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(TARGET)