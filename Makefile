cache : cache.o ds.o
			gcc -W -o cache cache.o ds.o
ds.o : ds.c ds.h
			gcc -c ds.c ds.h
cache.o : cache.c cache.h
			gcc -c cache.c cache.h
