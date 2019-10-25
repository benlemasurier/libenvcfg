# libenvcfg

libenvcfg is a helper for environment driven configuration. 

# usage

```c
ENVCFG *cfg = envcfg_new("example");
envcfg_add_default(cfg, "defaults", "example");
envcfg_add_default(cfg, "integers", "12345");

...
char *addr;
envcfg_get(cfg, "listen_address", &addr);
printf("listen address: %s\n", addr);
free(addr);

char *defaults;
envcfg_get(cfg, "defaults", &defaults);
printf("defaults: %s\n", addr);
free(defaults);

int *some_int;
envcfg_get(cfg, "integers", &some_int);
printf("integers: %d\n", *some_int);
free(some_int);

envcfg_free(cfg);
```

```sh
$ EXAMPLE_LISTEN_ADDRESS=127.0.0.1:8000 ./some-app
listen_address: 127.0.0.1:8000
defaults: example
integers: 12345
```

see full example at `example/main.c`
