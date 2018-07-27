
CC=gcc
CFLAGS=-I$(IDIR)

IDIR =./
LDIR =./lib
SDIR =./
ODIR =.

LIBS=-lm

_DEPS = uart_api.h log.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = uart_api.o uart_tool.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


uartool: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#obj/uart_api.o: uart_api.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

#obj/uart_tool.o: uart_tool.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
