#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "envcfg.h"

void __toupper(const char *s, char *dest);
struct _default *_default_get(struct _default *defaults, const char *key);

int
envcfg_add_default(struct envcfg *cfg,
			   const char *key, const char *val)
{
	if(cfg == NULL)
		return -1;

	/* FIXME: overwrite duplicates */
	struct _default *item = malloc(sizeof(struct _default));
	item->key   = malloc(sizeof(char) * strlen(key) + 1);
	item->value = malloc(sizeof(char) * strlen(val) + 1);
	item->next = NULL;

	strcpy(item->key, key);
	strcpy(item->value, val);

	if(!cfg->defaults) {
		cfg->defaults = item;
		return 0;
	}

	/* append to the end of the list of defaults */
	struct _default *head = cfg->defaults;
	while(head->next)
		head = head->next;

	head->next = item;

	return 0;
}

struct envcfg
envcfg_new(const char *program_name) {
	struct envcfg cfg = {
		.program_name = program_name,
		.defaults = NULL,
	};

	return cfg;
}

int
envcfg_free(struct envcfg *cfg)
{
	_default_free(cfg->defaults);

	return 0;
}

/**
 * envcfg_get retrieves a configuration value from the program's environment.
 *
 * @param cfg envcfg state struct
 * @param key configuration lookup key, if cfg.program_name is "foo", and the
 * 	      config item is bar, the corresponding environment variable would
 * 	      be "FOO_BAR".
 * @param dest memory storage location for the requested configuration value.
 * 	       On successful return, it is the responsibility of the caller to
 * 	       free() this value.
 * @return A return value of 0 indicates success, otherwise failure.
 */
int
envcfg_get(struct envcfg *cfg, const char *key, char **dest)
{
	/* create lookup key */
	size_t plen = strlen(cfg->program_name);
	size_t namelen = strlen(key);
	size_t size = plen + namelen + 2; /* '_' seperator + null-terminator */

	char buf[size];
	int ret = snprintf(buf, size, "%s_%s", cfg->program_name, key);
	if(ret == -1) {
		/* output error */
		return -1;
	}
	if(ret > size) {
		/* result was truncated */
		return -1;
	}

	/* uppercase the search key */
	char *envvar = malloc(size);
	if(envvar == NULL) {
		/* could not allocate memory */
		return -1;
	}
	__toupper(buf, envvar);	

	/* extract value from environment */
	char *val = getenv(envvar);
	free(envvar);
	if(val == NULL) {
		/* not found, check for default */
		struct _default *def = _default_get(cfg->defaults, key);
		if(def)
			val = def->value;
		else
			return -1;
	}

	/* copy result into destination buffer */
	*dest = malloc(strlen(val)+1);
	if(dest == NULL)
		return -1;

	strcpy(*dest, val);

	return 0;
}

void
__toupper(const char *s, char *dest)
{
	for(int i = 0; i < strlen(s); i++)
		dest[i] = toupper(s[i]);
}
