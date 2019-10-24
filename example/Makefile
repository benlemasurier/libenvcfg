CC=c99

main: main.c
	$(CC) -Wall -Wextra -pedantic -g $< -o $@ -L../ -lenvcfg

.PHONY: clean
clean:
	$(RM) main
