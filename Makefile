CC = gcc
CFLAGS = -Ofast
TARGET = window
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -luser32 -lgdi32
	./window.exe

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	cmd.exe /C del *.o $(TARGET).exe
