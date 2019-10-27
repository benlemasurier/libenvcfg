/* Copyright (C) 2019 Ben LeMasurier
 *
 * This file is part of libenvcfg.
 *
 * Libenvcfg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Libenvcfg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libenvcfg.  If not, see <https://www.gnu.org/licenses/>.
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "defaults.h"

/**
 * _default_append appends the provided item to the provided list of defaults.
 *
 * @param list list of defaults, must be non-NULL.
 * @param item default to append to the list.
 * @return On success 0 is returned, any non-zero value indicates failure.
 */
int
_default_append(DEFAULT *list, DEFAULT *item)
{
	assert(list != NULL);

	/* fast-forward to the end of the list */
	DEFAULT *head = list;
	while(head->next)
		head = head->next;

	head->next = item;
	return 0;
}

/**
 * _default_create creates a new defaults configuration item. The returned item
 * (list) must be deallocated via _default_free().
 *
 * @param key configuration item lookup key, e.g., FOO_BAR.
 * @param val configuration item value, e.g., "BAZ".
 * @return On success a new DEFAULT item is returned. Any error encountered
 * will result in a NULL return value.
 */
DEFAULT *
_default_create(const char *key, const char *val)
{
	assert(key != NULL);
	assert(val != NULL);

	DEFAULT *item = malloc(sizeof(DEFAULT));
	if(item == NULL)
		return NULL;
	item->next = NULL;

	if(!asprintf(&item->key, key)) {
		free(item);
		return NULL;
	}

	if(!asprintf(&item->value, val)) {
		free(item->key);
		free(item);
		return NULL;
	}

	return item;
}

/**
 * _default_get finds an item in the provided list with a key matching the
 * provided key, if found.
 *
 * @param defaults list of default configuration items.
 * @return If a matching item is found, a pointer to it's location is returned.
 *	   If no match was found, NULL is returned.
 */
DEFAULT *
_default_get(DEFAULT *defaults, const char *key)
{
	if(!defaults)
		return NULL;

	DEFAULT *head = defaults;
	while(head) {
		if(strcmp(head->key, key) == 0)
			return head;
		head = head->next;
	}

	return NULL;
}

/**
 * _default_free free()s all resources assocated with the provided list
 * of defaults.
 *
 * @param defaults list of defaults.
 */
void
_default_free(DEFAULT *defaults)
{
	if(!defaults)
		return;

	DEFAULT *tmp;
	DEFAULT *head = defaults;
	while(head) {
		tmp = head;
		head = head->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
