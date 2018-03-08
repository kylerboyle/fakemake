
hmap_int.o: hmap.c hmap.h
	gcc -DIVAL -c -g hmap.c -o hmap_int.o

hmap_dbl.o: hmap.c hmap.h
	gcc -DDVAL -c -g hmap.c -o hmap_dbl.o

hmap_str.o: hmap.c hmap.h
	gcc -DSVAL -c -g hmap.c -o hmap_str.o

hmap_ptr.o: hmap.c hmap.h
	gcc -DPVAL -c -g hmap.c -o hmap_ptr.o

hmap.o: hmap.c hmap.h
	gcc -DPVAL -c -g hmap.c

test: test.c hmap.h hmap.o
	gcc -g test.c hmap.o -o test

freq: freq.c hmap.h hmap_int.o
	gcc -DIVAL -g freq.c hmap_int.o -o freq

dval_demo: dval_demo.c hmap.h hmap_dbl.o
	gcc -DDVAL -g dval_demo.c hmap_dbl.o -o dval_demo

pval_demo: pval_demo.c hmap.h hmap_ptr.o
	gcc -DPVAL -g pval_demo.c hmap_ptr.o -o pval_demo

fakemake: fakemake.c hmap.h hmap_int.o
	gcc -DIVAL -g fakemake.c hmap_int.o -o fakemake

clean:
	rm -f *.o test freq pval_demo dval_demo

