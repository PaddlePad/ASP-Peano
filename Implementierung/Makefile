# Add additional compiler flags here
CFLAGS=-O0 -Wall -Wextra -DNDEBUG -g
## 
all: peano
peano: peano.c peano.S #testAssembler.c mirror.S reverse.S
	$(CC) $(CFLAGS) -o $@ $^ -lm -fbounds-check
clean:
	rm -f peano
