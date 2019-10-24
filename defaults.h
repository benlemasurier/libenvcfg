#ifndef _ENVCFG_DEFAULT_H
#define _ENVCFG_DEFAULT_H

typedef struct default_t {
	char *key;
	char *value;

	struct default_t *next;
} DEFAULT;

int _default_append(DEFAULT *list, DEFAULT *item);
DEFAULT *_default_create(const char *key, const char *val);
void _default_free(DEFAULT *defaults);
DEFAULT *_default_get(DEFAULT *defaults, const char *key);

#endif /* _ENVCFG_DEFAULT_H */
