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

#pragma once

struct list_node {
	void* data;
	struct list_node* next;
};

struct list {
	struct list_node* head;
	struct list_node* tail;
	int nodeCount;
};

#define list_foreach(i, list) for (struct list_node* i = (list)->head; i != NULL; i = i->next)

struct list* list_create();
void list_destroy(struct list* destroyList);
void list_insert(struct list* insertList, void* data);
void list_remove(struct list* deleteList, void* data);
