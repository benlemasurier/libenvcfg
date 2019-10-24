#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defaults.h"

struct _default *
_default_get(struct _default *defaults, const char *key)
{
	if(!defaults)
		return NULL;

	struct _default *head = defaults;
	while(head) {
		if(strcmp(head->key, key) == 0)
			return head;
		head = head->next;
	}

	return NULL;
}

void
_default_free(struct _default *defaults)
{
	if(!defaults)
		return;

	struct _default *tmp;
	struct _default *head = defaults;
	while(head) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
