#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "envcfg.h"

static void _ucase(char *s);

/**
 * envcfg_add_default adds a new default value to the provided config.
 *
 * @param cfg envcfg state
 * @param key configuration lookup key, e.g., "listen_address".
 * @param val default value, e.g., "0.0.0.0:8000".
 *
 * @return On success 0 is returned, any non-zero value indicates an error.
 */
int
envcfg_add_default(ENVCFG *cfg, const char *key, const char *val)
{
	assert(cfg != NULL);
	assert(key != NULL);
	assert(val != NULL);

	/* overwrite duplicates */
	DEFAULT *existing = _default_get(cfg->defaults, key);
	if(existing) {
		free(existing->value);
		existing->value = malloc(sizeof(char) * strlen(val) + 1);
		strcpy(existing->value, val);
		return 0;
	}

	DEFAULT *item = _default_create(key, val);
	if(item == NULL)
		return -1;

	if(cfg->defaults == NULL) {
		cfg->defaults = item;
		return 0;
	}

	if(_default_append(cfg->defaults, item) != 0)
		return -1;

	return 0;
}

/**
 * envcfg_new creates a new environment configuration. The returned ENVCFG
 * must be deallocated via `envcfg_free()`.
 *
 * @param program_name the program's name. This represents the prefix to all
 *	  environment variables, i.e., if the program is named "foo", all
 *	  environment configuration variables should be in the form FOO_<item>.
 *
 * @return On success, a pointer to ENVCFG is returned. Any error encountered
 *	   will result in a return value of NULL.
 */
ENVCFG *
envcfg_new(const char *program_name) {
	ENVCFG *cfg = malloc(sizeof(ENVCFG));
	if(cfg == NULL)
		return NULL;

	cfg->program_name = malloc(strlen(program_name) + 1);
	if(cfg->program_name == NULL) {
		free(cfg);
		return NULL;
	}
	strcpy(cfg->program_name, program_name);

	cfg->defaults = NULL;

	return cfg;
}

/**
 * envcfg_free free()s all memory associated with the provided EVNCFG. This
 *	       must be called when a configuration is no longer needed or
 *	       memory will be leaked.
 *
 * @param cfg The configuration to deallocate.
 *
 * @return On success 0 is returned, otherwise a non-zero value.
 */
int
envcfg_free(ENVCFG *cfg)
{
	if(cfg == NULL)
		return 0;

	_default_free(cfg->defaults);
	free(cfg->program_name);
	free(cfg);

	return 0;
}

/**
 * envcfg_get retrieves a configuration value from the program's environment.
 * If the key is not found within the environment a configured default will be
 * used, if one exists.
 *
 * @param cfg envcfg state
 * @param key configuration lookup key, if cfg.program_name is "foo", and the
 * 	      config item is bar, the corresponding environment variable would
 * 	      be "FOO_BAR".
 * @param dest memory storage location for the requested configuration value.
 * 	       On successful return, it is the responsibility of the caller to
 * 	       free() this value.
 * @return A return value of 0 indicates success, otherwise failure.
 */
int
envcfg_get(ENVCFG *cfg, const char *key, char **dest)
{
	/* create environment lookup key */
	size_t plen = strlen(cfg->program_name);
	size_t namelen = strlen(key);
	size_t size = plen + namelen + 2; /* '_' seperator + null-terminator */

	char buf[size];
	int ret = snprintf(buf, size, "%s_%s", cfg->program_name, key);
	if(ret == -1) {
		/* output error */
		return -1;
	}
	if((size_t) ret > size) {
		/* result was truncated */
		return -1;
	}

	/* uppercase the lookup key */
	_ucase(buf);

	/* extract value from environment */
	char *val = getenv(buf);
	if(val == NULL) {
		/* not found, check for default */
		DEFAULT *def = _default_get(cfg->defaults, key);
		if(def)
			val = def->value;
		else
			return -1;
	}

	/* copy result into destination buffer */
	*dest = malloc(strlen(val)+1);
	if(*dest == NULL)
		return -1;

	strcpy(*dest, val);

	return 0;
}

static void
_ucase(char *s)
{
	for(size_t i = 0; i < strlen(s); i++)
		s[i] = toupper(s[i]);
}
