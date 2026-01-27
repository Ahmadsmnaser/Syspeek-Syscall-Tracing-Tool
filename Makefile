CC      := gcc
CFLAGS  := -Wall -Wextra -O0 -g
INCLUDES:= -Iinclude

SRC := \
	src/main.c \
	src/trace.c \
	src/formatter.c \
	src/syscall_table_x86_64.c

OBJ := $(SRC:.c=.o)

TARGET := syspeek

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all
