
.PHONY: clean defult

defult: solution.c
	gcc -o solution solution.c -pthread

clean:
	rm -f solution
