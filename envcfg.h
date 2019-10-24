#ifndef _ENVCFG_H
#define _ENVCFG_H

/* FIXME: what here needs to be `extern`? */

#include "defaults.h"

struct envcfg {
    const char *program_name;
    struct _default *defaults;
};

struct envcfg envcfg_new(const char *program_name);
int envcfg_add_default(struct envcfg *cfg,
		       const char *key, const char *val);
int envcfg_get(struct envcfg  *cfg, const char *key, char **dest);
int envcfg_free(struct envcfg *cfg);

#endif /* _ENVCFG_H */
