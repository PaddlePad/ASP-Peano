# Add additional compiler flags here
CFLAGS=-O0 -Wall -Wextra -DNDEBUG -g

all: peano
peano: peano.c peano.S
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f peano
