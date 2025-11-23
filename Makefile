build:
	gcc sfl.c functii.c functii.h sfl.h -o sfl -Wall -Wextra -std=c99
build1:
	gcc test.c -o sfl -lm
run_sfl:
	./sfl
clean:
	rm sfl
