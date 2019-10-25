#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../envcfg.h"

#define PROGRAM_NAME "example"

int
main() {
	ENVCFG *cfg = envcfg_new(PROGRAM_NAME);

	if(envcfg_add_default(cfg, "foo", "bar") != 0) {
		printf("something went wrong adding default (foo)\n");
		exit(EXIT_FAILURE);
	}
	if(envcfg_add_default(cfg, "foo", "bar-overwritten") != 0) {
		printf("something went wrong adding default (bar-overwritten)\n");
		exit(EXIT_FAILURE);
	}

	if(envcfg_add_default(cfg, "test_integer", "1") != 0) {
		printf("something went wrong adding default (test_integer)\n");
		exit(EXIT_FAILURE);
	}

	char *addr;
	if(envcfg_get(cfg, "listen_address", &addr) != 0) {
		printf("something went wrong obtaining listen address value\n");
		exit(EXIT_FAILURE);
	}
	printf("listen address: %s\n", addr);
	free(addr);

	char *foo;
	if(envcfg_get(cfg, "foo", &foo) != 0) {
		printf("something went wrong obtaining foo (default) value\n");
		exit(EXIT_FAILURE);
	}
	printf("foo (default): %s\n", foo);
	free(foo);

	/* integer values */
	int *some_int;
	if(envcfg_get_int(cfg, "integer_test", &some_int) != 0) {
		printf("something went wrong obtaining test integer\n");
		exit(EXIT_FAILURE);
	}
	printf("test integer: %d\n", *some_int);
	free(some_int);

	/* cleanup */
	if(envcfg_free(cfg) != 0) {
		printf("something went wrong freeing config\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
