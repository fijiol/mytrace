

all:
	gcc -fPIC -shared mylib.c -o mylib.so

test:
	MYTRACE_FORK_CMD='echo %pid' LD_PRELOAD=./mylib.so bash ./test.sh

clean:
	rm -f mylib.so

