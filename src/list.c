/*	
 *	SPDX-License-Identifier: GPL-3.0-only
 *
 *  Virtual Orchestra - Musical Instrument Simulation
 *  Copyright (C) 2024 Garnek0 (Popa Vlad) and Contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vo/list.h>
#include <vo/debug.h>

#include <stdlib.h>
#include <string.h>

struct list* list_create() {
	struct list* newList = malloc(sizeof(struct list));
	memset((void*)newList, 0, sizeof(struct list));

	return newList;
}

void list_destroy(struct list* destroyList) {
	if (!destroyList)
		return;

	struct list_node* next;

	for (struct list_node* i = destroyList->head; i; i = next) {
		next = i->next;
		free((void*)i);
	}

	free((void*)destroyList);
}

void list_insert(struct list* insertList, void* data) {
	struct list_node* node = malloc(sizeof(struct list_node));
	node->data = data;
	node->next = NULL;

	if (insertList->head == NULL && insertList->tail == NULL) {
		insertList->head = insertList->tail = node;
	} else if (insertList->tail != NULL && insertList->head != NULL) {
		insertList->tail->next = node;
		insertList->tail = node;
	} else {
		debug_log(LOGLEVEL_WARN, "List: A linked list somehow got incredibly messed up.\n");
		return;
	}

	insertList->nodeCount++;
}

void list_remove(struct list* deleteList, void* data) {
	struct list_node* prev = NULL; 

	list_foreach(node, deleteList) {
		if (node->data == data) {
			if (node == deleteList->head)
				deleteList->head = node->next;

			if (node == deleteList->tail)
				deleteList->tail = prev;

			if (prev)
				prev->next = NULL;

			free((void*)node);

			break;
		}

		prev = node;
	}
}
