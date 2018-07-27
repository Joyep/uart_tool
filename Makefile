TARGET = uartool

_OBJ = uart_device.o \
	   uart_tool.o

_DEPS = uart_device.h log.h

IDIR = ./
LDIR = ./lib
SDIR = ./
ODIR = .
LIBS = -lm

CC=gcc
CFLAGS=-I$(IDIR)

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(TARGET)
