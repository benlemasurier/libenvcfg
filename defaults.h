#ifndef _ENVCFG_DEFAULT_H
#define _ENVCFG_DEFAULT_H

struct _default {
	char *key;
	char *value;

	struct _default *next;
};

void _default_free(struct _default *defaults);
struct _default *_default_get(struct _default *defaults, const char *key);

#endif /* _ENVCFG_DEFAULT_H */
