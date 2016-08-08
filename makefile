all:
	gcc -o test *.c *.h -std=c89 -Wall -Wextra -pedantic -g
clean:
	rm -f test
