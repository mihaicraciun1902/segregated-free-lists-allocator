//Craciun Mihai Alexandru 313CA
#ifndef sfl_H
#define sfl_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define INIT_HEAP 1
#define MALLOC 2
#define FREE 3
#define READ 4
#define WRITE 5
#define DUMP_MEMORY 6
#define DESTROY_HEAP 7

//key struct
typedef struct {
		char *name;
		int command_id;
} command_struct;

typedef struct {
	void *data;
	int dim_lista, adresa;
} a_l;//active list

typedef struct node_t {
	void *data; // Adresa de inceput a blocului de memorie liberă
	struct node_t *prev; // Pointer catre nodul precedent din lista
	struct node_t *next; // Pointer catre nodul următor din lista
} node_t;

typedef struct {
	int total_memory;
	int total_allocated_memory;
	int total_free_memory;
	int nr_of_free_blocks;
	int nr_of_allocated_blocks;

	int malloc_calls;
	int nr_fragmentations;
	int free_calls;

	int active_dim;//curent dim
	int active_size_max;//maxim dim
} dump;

typedef struct {
	node_t *head;
	unsigned int data_size;
	unsigned int size;
	int size_in_bytes;
} list_t;

// Structura de date Segregated Free Lists
typedef struct {
	list_t **v; // Vectorul de liste dublu înlănțuite(vec de pointeri)
	int base;
	int num_lists; // Numărul de liste
	int bpl; // Numărul de bytes per listă
	int rec_type; // Tipul de reconstituire
} sfl_t;

#endif
