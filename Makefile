CC=c99

libenvcfg.so: defaults.o libenvcfg.o
	$(CC) -shared -Wl,-soname,libenvcfg.so.1 -o libenvcfg.so.1.0.1 libenvcfg.o defaults.o
	ln -s libenvcfg.so.1.0.1 libenvcfg.so.1
	ln -s libenvcfg.so.1.0.1 libenvcfg.so


libenvcfg.o: defaults.o envcfg.[ch]
	$(CC) -fPIC -g -c -Wall -o libenvcfg.o envcfg.c

defaults.o: defaults.[ch]
	$(CC) -fPIC -g -c -Wall -o defaults.o defaults.c

.PHONY: clean
clean:
	$(RM) libenvcfg.o
	$(RM) defaults.o
	$(RM) libenvcfg.so*
