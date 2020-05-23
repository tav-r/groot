CC=clang
CFLAGS=-Wall -fpic
LFLAGS=-shared -nostdlib
TARGET=libgroot.so
SRC=hooks.c censor.c
OBJ=$(SRC:.c=.o)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

lib: $(OBJ)
	$(CC) -o $(TARGET) $(LFLAGS) $(OBJ) && strip -s $(TARGET)

clean:
	rm *.o *.so
