#include "weightlist.h"

WList initListW(V data)
{
	WList list;
	list = (WList) malloc(sizeof(struct wlist));
	list->data = data;
	list->next = NULL;
	list->prev = NULL;
	return list;
}

WList addFirstW(WList list, V data)
{
	WList new;
	if (list == NULL)
		return initListW(data);
	new = initListW(data);
	new->next = list;
	list->prev = new;
	return new;
}

WList addLastW(WList list, V data)
{
	WList new, tmp;
	if (list == NULL)
		return initListW(data);
	new = initListW(data);
	tmp = list;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
	return list;
}

WList freeListW(WList list)
{
	WList tmp;
	if (list == NULL)
		return list;
	while (list != NULL) {
		tmp = list;
		list = list->next;
		free(tmp);
	}
	return NULL;
}
