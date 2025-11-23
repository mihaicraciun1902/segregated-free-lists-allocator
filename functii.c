//Craciun Mihai Alexandru 313CA
#include "functii.h"

int checkcommand(char *command)
{
	command_struct commands[] = {
			{"INIT_HEAP", INIT_HEAP},
			{"MALLOC", MALLOC},
			{"FREE", FREE},
			{"READ", READ},
			{"WRITE", WRITE},
			{"DUMP_MEMORY", DUMP_MEMORY},
			{"DESTROY_HEAP", DESTROY_HEAP}};

	int nr = sizeof(commands) / sizeof(command_struct);

	for (int i = 0; i < nr; i++) {
		if (strcmp(commands[i].name, command) == 0)
			return commands[i].command_id;
	}

	return -1;
}

list_t *create(unsigned int data_size)
{
	list_t *list = calloc(1, sizeof(list_t));
	list->head = NULL;
	list->data_size = data_size;
	list->size = 0;
	return list;
}

void add_nth_node(list_t *list, unsigned int n, const void *addres)
{
	unsigned int i;
	node_t *new_node = malloc(sizeof(node_t));
	new_node->data = calloc(1, sizeof(int));
	memcpy(new_node->data, addres, sizeof(int));
	new_node->next = NULL;
	new_node->prev = NULL;
	if (list->size == 0) {
		list->head = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
		list->size++;
		return;
	}
	if (n == 0) {
		new_node->next = list->head;
		new_node->prev = list->head->prev;
		list->head->prev->next = new_node;
		list->head->prev = new_node;
		list->head = new_node;
		list->size++;
		return;
	}
	if (n >= list->size) {
		node_t *ult = list->head->prev;
		ult->next = new_node;
		new_node->prev = ult;
		new_node->next = list->head;
		list->head->prev = new_node;
		list->size++;
		return;
	}
	node_t *curr = list->head;
	for (i = 0; i < n; i++)
		curr = curr->next;
	new_node->next = curr;
	new_node->prev = curr->prev;
	curr->prev->next = new_node;
	curr->prev = new_node;
	list->size++;
}

node_t *remove_nth_node(list_t *list, unsigned int n)
{
	unsigned int i;
	if (n >= list->size)
		n = list->size - 1;
	node_t *curr = list->head;
	for (i = 0; i < n; i++)
		curr = curr->next;
	if (n == 0)
		list->head = curr->next;
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	list->size--;
	return curr;
}

void free_list(list_t **pp_list)
{
	list_t *list = *pp_list;
	node_t *curr = list->head;
	node_t *urm;
	for (unsigned int i = 0; i < list->size; i++) {
		urm = curr->next;
		free(curr->data);
		free(curr);
		curr = urm;
	}
	free(list);
	*pp_list = NULL;
}

void sfl_create(int num_lists, int bpl, dump *data, sfl_t *sfl)
{
	unsigned int aux = 8, data_size = 0;
	for (int i = 0; i < num_lists; i++) {
		sfl->v[i] = create(data_size);
		sfl->v[i]->size_in_bytes = aux;

		//create nodes
		for (int j = 0; j < bpl; j = j + aux) {
			int addr = sfl->base + j + i * bpl;
			add_nth_node(sfl->v[i], j, &addr);
			data->nr_of_free_blocks++;
		}
		aux *= 2;
	}
}

void initheap(int base, int num_lists, int bpl, dump *data, sfl_t *sfl)
{
	// initiate the dump_data
	data->total_memory = num_lists * bpl;
	data->total_allocated_memory = 0;
	data->total_free_memory = data->total_memory;
	data->nr_of_free_blocks = 0;
	data->nr_of_allocated_blocks = 0;
	data->malloc_calls = 0;
	data->nr_fragmentations = 0;
	data->free_calls = 0;
	data->active_dim = 0;
	data->active_size_max = 1;
	sfl->base = base;

	// initiate the sfl data
	sfl->v = calloc(num_lists, sizeof(void *));
	sfl->num_lists = num_lists;
	sfl->bpl = bpl;
	sfl->rec_type = 0;
	sfl_create(num_lists, bpl, data, sfl);
}

void freeall(sfl_t *sfl, dump *data, a_l *al)
{
	for (int i = 0; i < sfl->num_lists; i++)
		free_list(&sfl->v[i]);
	free(sfl->v);
	for (int i = 0; i < data->active_dim; i++)
		if (al[i].data)
			free(al[i].data);
	free(al);
}

void my_free(int free_addr, sfl_t *sfl, dump *data, a_l *al)
{
	data->free_calls++;
	if (free_addr == 0) {
		data->free_calls--; return;
	}

	for (int i = 0; i < data->active_dim; i++)
		if (al[i].adresa == free_addr) { // search for requested addres
			for (int k = 0; k < sfl->num_lists; k++)
				if (al[i].dim_lista == sfl->v[k]->size_in_bytes) {
					add_nth_node(sfl->v[k], 0, &free_addr);
					data->nr_of_free_blocks++;
					data->nr_of_allocated_blocks--;
					data->total_allocated_memory -= al[i].dim_lista;
					data->total_free_memory += al[i].dim_lista;
					if (al[i].data)
						free(al[i].data);
					for (int j = i; j < data->active_dim - 1; j++)
						al[j] = al[j + 1];
					data->active_dim--;
					return;
				}
			//if a list with the size doesnt exist create it
				sfl->v = realloc(sfl->v, sfl->num_lists * sizeof(sfl_t));
				sfl->v[sfl->num_lists] = create(al[i].dim_lista);
				add_nth_node(sfl->v[sfl->num_lists], 0, &free_addr);
				sfl->v[sfl->num_lists]->size = 1;
				sfl->v[sfl->num_lists]->size_in_bytes = al[i].dim_lista;
				sfl->num_lists++;
				data->nr_of_free_blocks++;
				data->nr_of_allocated_blocks--;
				data->total_allocated_memory -= al[i].dim_lista;
				data->total_free_memory += al[i].dim_lista;
				if (al[i].data)
					free(al[i].data);
				for (int j = i; j < data->active_dim - 1; j++)
					al[j] = al[j + 1];
				data->active_dim--;
				return;
		}
	printf("Invalid free\n");
	data->free_calls--;
}

void my_malloc(int malloc_bytes, sfl_t *sfl, dump *data, a_l *al)
{
for (int i = 0; i < sfl->num_lists; i++) {
	if (sfl->v[i]->size >= 1)
		bubblesort(sfl->v[i]->head, sfl->v[i]->size);
}

	int done = 0;
	data->malloc_calls++;
	int new_list = 1;

// Try to find a block with exact size
for (int i = 0; i < sfl->num_lists; i++) {
	if (malloc_bytes == sfl->v[i]->size_in_bytes && sfl->v[i]->size > 0) {
		// Remove the block from the list
		node_t *node = remove_nth_node(sfl->v[i], 0);
		// Update statistics
		data->total_allocated_memory += malloc_bytes;
		data->total_free_memory -= malloc_bytes;
		data->nr_of_allocated_blocks++;
		data->nr_of_free_blocks--;
		// Store information about the allocated block
		al[data->active_dim].adresa = (*(int *)node->data);
		al[data->active_dim].dim_lista = malloc_bytes;
		al[data->active_dim].data = NULL;
		data->active_dim++;
		free(node->data);
		free(node); //Free the memory allocated for the removed node
		done = 1;
		return;
	}
}

// If no exact size list is found, find a larger ;ost to fragment
if (!done) {
	for (int i = 0; i < sfl->num_lists; i++) {
		if (malloc_bytes < sfl->v[i]->size_in_bytes && sfl->v[i]->size > 0) {
			// Remove the block from the list
			node_t *node = remove_nth_node(sfl->v[i], 0);
			// Update statistics
			data->total_allocated_memory += malloc_bytes;
			data->total_free_memory -= malloc_bytes;
			data->nr_of_allocated_blocks++;
			data->nr_fragmentations++;
			// Store information about the allocated block
			al[data->active_dim].adresa =  (*(int *)node->data);
			al[data->active_dim].dim_lista = malloc_bytes;
			al[data->active_dim].data = NULL;
			data->active_dim++;
			for (int j = 0; j < sfl->num_lists; j++) {
				int m = malloc_bytes;
				int addr =  (*(int *)node->data) + m;
				if (sfl->v[i]->size_in_bytes - m == sfl->v[j]->size_in_bytes) {
					add_nth_node(sfl->v[j], 0,  &addr);
					new_list = 0;
				}
			}
			int remaining = sfl->v[i]->size_in_bytes - malloc_bytes;
			// Create a new list for the remaining block
			if (new_list) {
				sfl->v = realloc(sfl->v, sfl->num_lists * sizeof(sfl_t));
				sfl->v[sfl->num_lists] = create(remaining);
				sfl->v[sfl->num_lists]->size = 0;
				sfl->v[sfl->num_lists]->size_in_bytes = remaining;
				int addr =  (*(int *)node->data) + malloc_bytes;
				add_nth_node(sfl->v[sfl->num_lists], 0, &(addr));
				sfl->num_lists++;
			}
			free(node->data);
			free(node); //Free the memory allocated for the removed node
			for (int a = 0; a < sfl->num_lists - 1; a++) {
				for (int b = a + 1; b < sfl->num_lists; b++) {
					if (sfl->v[a]->size_in_bytes > sfl->v[b]->size_in_bytes) {
						list_t *aux = sfl->v[a];
						sfl->v[a] = sfl->v[b];
						sfl->v[b] = aux;
					}
				}
			}
			done = 1;
			return;
		}
	}
}

// If neither exact size nor larger blocks are found
	if (!done) {
		data->malloc_calls--;
		printf("Out of memory\n");
	}
}

void my_read(int r_add, int nb, sfl_t *sfl, dump *data, a_l *al)
{
		int addr_exist = 0, poz = -1, currmem, offset = 0;
		for (int i = 0; i < data->active_dim; i++) {
			if (al[i].adresa == r_add) {
				poz = i;
				addr_exist = 1;
				break;
			}
		}
		if (poz == -1)
			for (int i = 0; i < data->active_dim; i++) {
				if (al[i].adresa == r_add - nb) {
					poz = i;
					addr_exist = 1;
					offset = r_add - al[i].adresa;
					break;
				}
			}
		if (!addr_exist) {
			printf("Segmentation fault (core dumped)\n");
			dumpmemory(data, sfl, al);
			freeall(sfl, data, al);
			exit(0);
			return;
		}
	int nol = 1;
	int i = poz + 1, need = al[poz].dim_lista;
	while (need < nb && i < data->active_dim) {
		if (al[i].dim_lista == al[poz].dim_lista) {
			need += al[poz].dim_lista;
			nol++;
		}
		i++;
	}

		char *data_ptr = (char *)al[poz].data;
		if (nol == 1) {
			int addr = al[poz].adresa;
			if (offset == 0) {
				for (int i = addr; i < addr + nb; i++)
					printf("%c", data_ptr[i - addr]);

			} else {
				for (int i = addr + offset; i < addr + offset + nb; i++)
					printf("%c", data_ptr[i - addr]);
			}
		} else {
			int ies = 0;
			currmem = al[poz].dim_lista;
			for (int i = 1; i < nol; i++) {
				if (nb <= currmem)
					break;
				currmem += al[poz + i].dim_lista;
				if (!(al[poz].adresa + al[poz].dim_lista == al[poz + i].adresa))
					ies = 1;
			}
			if (ies) {
				printf("Segmentation fault (core dumped)\n");
				dumpmemory(data, sfl, al);
				freeall(sfl, data, al);
				exit(0);
				return;
			}
			currmem = nb;
			for (int i = 0; i < nol; i++) {
				if (i >= data->active_dim)
					break;
				data_ptr = (char *)al[poz + i].data;
				int addr = al[poz + i].adresa;
				int d = al[i].dim_lista;
				for (int j = addr; j < addr + (currmem < d ? currmem : d); j++)
					printf("%c", data_ptr[j - addr]);
				currmem -= d;
			}
		}
	printf("\n");
}

void my_write(int w_ad, int nb, sfl_t *sfl, dump *data, a_l *al, char *w)
{
	int len, ok = 0, addr_exist = 0, poz, currmem;
	int nol = 1;
	len = strlen(w) + 1;
	if (nb > len)
		nb = len;

	if (nb < len)
		nb = len;

	for (int i = 0; i < data->active_dim; i++) {
		if (al[i].adresa == w_ad) {
			addr_exist = 1;
			poz = i;
			break;
		}
	}

	if (!addr_exist) {
		printf("Segmentation fault (core dumped)\n");
			dumpmemory(data, sfl, al);
			freeall(sfl, data, al);
			exit(0);
			return;
	}

	int i = poz + 1, need = al[poz].dim_lista;
	while (need < nb && i < data->active_dim) {
		if (al[i].dim_lista == al[poz].dim_lista) {
			need += al[poz].dim_lista;
			nol++;
		}
		i++;
	}

	if (nb <= need)
		ok = 1;

	if (!ok) {
		printf("Segmentation fault (core dumped)\n");
		dumpmemory(data, sfl, al);
		freeall(sfl, data, al);
		exit(0);
		return;
	}

	if (nol == 1) {
		for (int i = 0; i < data->active_dim; i++) {
			if (al[i].dim_lista >= nb && al[i].adresa == w_ad) {
				if (!al[i].data) {
					al[i].data = calloc(1, al[i].dim_lista);
					memcpy(al[i].data, w, nb);
				}
			memcpy(al[i].data, w, nb - 1);
			break;
			}
		}
	} else {
		currmem = nb;
		char *p = w;
		for (int i = poz; i < nol; i++) {
			int d = al[i].dim_lista;
			if (!al[i].data) {
				al[i].data = calloc(1, d);
				memcpy(al[i].data, p, d < currmem ? d : currmem);
			}
			memcpy(al[i].data, p, d < currmem ? d - 1 : currmem - 1);
			currmem -= al[i].dim_lista;
			p += al[i].dim_lista;
		}
	}
}

void bubblesort(node_t *start, unsigned int size)
{
	int swapped;
	node_t *ptr1;

	/* Checking for empty list */
	if (!start)
		return;

	do {
		swapped = 0;
		ptr1 = start;

		for (unsigned int i = 0; i < size - 1; i++) {
			if ((*(int *)ptr1->data) > (*(int *)ptr1->next->data)) {
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
	} while (swapped);
}

void swap(node_t *a, node_t *b)
{
	unsigned int temp =  (*(int *)a->data);
	(*(int *)a->data) = (*(int *)b->data);
	(*(int *)b->data) = temp;
}

void dumpmemory(dump *data, sfl_t *sfl, a_l *al)
{
	printf("+++++DUMP+++++\n");
	printf("Total memory: %d bytes\n", data->total_memory);
	printf("Total allocated memory: %d bytes\n", data->total_allocated_memory);
	printf("Total free memory: %d bytes\n", data->total_free_memory);
	printf("Free blocks: %d\n", data->nr_of_free_blocks);
	printf("Number of allocated blocks: %d\n", data->nr_of_allocated_blocks);
	printf("Number of malloc calls: %d\n", data->malloc_calls);
	printf("Number of fragmentations: %d\n", data->nr_fragmentations);
	printf("Number of free calls: %d\n", data->free_calls);
	for (int i = 0; i < sfl->num_lists - 1; i++) {
		for (int j = 0; j < sfl->num_lists - i - 1; j++) {
			if (!sfl->v[j] || !sfl->v[j + 1])
				continue;
			if (sfl->v[j]->size_in_bytes > sfl->v[j + 1]->size_in_bytes) {
				// Swap the pointers
				list_t *temp = sfl->v[j];
				sfl->v[j] = sfl->v[j + 1];
				sfl->v[j + 1] = temp;
			}
		}
	}
	for (int i = 0; i < sfl->num_lists; i++) {
		if (sfl->v[i]->size >= 1) {
			int x = sfl->v[i]->size_in_bytes;
			int y = sfl->v[i]->size;
			printf("Blocks with %d bytes - %d free block(s) : ", x, y);
			bubblesort(sfl->v[i]->head, sfl->v[i]->size);
			node_t *curr = sfl->v[i]->head;
			for (unsigned int j = 0; j < sfl->v[i]->size; j++) {
				if (j == sfl->v[i]->size - 1)
					printf("0x%x",  (*(int *)curr->data));
				else
					printf("0x%x ",  (*(int *)curr->data));
				curr = curr->next;
			}
			printf("\n");
		}
	}

	a_l aux;
	for (int a = 0; a < data->active_dim - 1; a++) {
		for (int b = a + 1; b < data->active_dim; b++) {
			if (al[a].adresa > al[b].adresa) {
				aux = al[a];
				al[a] = al[b];
				al[b] = aux;
			}
		}
	}

	if (data->active_dim <= 0) {
		printf("Allocated blocks :");
	} else {
		printf("Allocated blocks : ");
		for (int i = 0; i < data->active_dim; i++) { // active memory
			if (i == data->active_dim - 1)
				printf("(0x%x - %d)", al[i].adresa, al[i].dim_lista);
			else
				printf("(0x%x - %d) ", al[i].adresa, al[i].dim_lista);
		}
	}
	printf("\n-----DUMP-----\n");
}
