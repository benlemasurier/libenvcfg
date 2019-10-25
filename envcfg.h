#ifndef _ENVCFG_H
#define _ENVCFG_H

#include "defaults.h"

typedef struct envcfg_t {
    char *program_name;
    DEFAULT *defaults;
} ENVCFG;

ENVCFG *envcfg_new(const char *program_name);
int envcfg_add_default(ENVCFG *cfg, const char *key, const char *val);
int envcfg_get(ENVCFG *cfg, const char *key, char **dest);
int envcfg_get_int(ENVCFG *cfg, const char *key, int **dest);
int envcfg_free(ENVCFG *cfg);

#endif /* _ENVCFG_H */
