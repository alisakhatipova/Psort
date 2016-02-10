build: psort
psort: manager.o partitioner.o sorter.o utilites.o merger.o
	gcc manager.o partitioner.o sorter.o utilites.o merger.o -o psort
partitioner.o: partitioner.c
	gcc -c partitioner.c -o partitioner.o
merger.o: merger.c
	gcc -c merger.c -o merger.o
utilites.o: utilites.c
	gcc -c utilites.c -o utilites.o
sorter.o: sorter.c
	gcc -c sorter.c -o sorter.o
manager.o: manager.c headers.h
	gcc -c manager.c -o manager.o

build_release: psort34
psort34: manager.o partitioner.o sorter.o utilites.o merger.o
	gcc -O3 manager.o partitioner.o sorter.o utilites.o merger.o -o psort34
partitioner.o: partitioner.c
	gcc -O3 -c partitioner.c -o partitioner.o
merger.o: merger.c
	gcc -O3 -c merger.c -o merger.o
utilites.o: utilites.c
	gcc -O3 -c utilites.c -o utilites.o
sorter.o: sorter.c
	gcc -O3 -c sorter.c -o sorter.o
manager.o: manager.c headers.h
	gcc -O3 -c manager.c -o manager.o

tester: test
test: test.o
	gcc test.o -o test
test.o: test.c
	gcc -c test.c -o test.o

comparing: compare
compare: compare.o
	gcc compare.o -o compare
compare.o: compare.c
	gcc -c compare.c -o compare.o

clean:
	rm *.o psort
