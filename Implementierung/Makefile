# Add additional compiler flags here
CFLAGS=-O0 -Wall -Wextra -DNDEBUG -g
## peano.c peano.S
all: peano
peano: testAssembler.c mirror.S reverse.S
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f peano
