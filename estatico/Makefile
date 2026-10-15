CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS =
LDLIBS =

TARGET = ranking.exe
OBJECTS = main.o ventas.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: main.c ventas.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c main.c -o $@

ventas.o: ventas.c ventas.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ventas.c -o $@

clean:
ifeq ($(OS),Windows_NT)
	powershell.exe -NoProfile -NonInteractive -Command "foreach ($$archivo in @('$(TARGET)', 'main.o', 'ventas.o')) { if (Test-Path -LiteralPath $$archivo) { Remove-Item -LiteralPath $$archivo -Force -ErrorAction Stop } }"
else
	$(RM) $(TARGET) $(OBJECTS)
endif
