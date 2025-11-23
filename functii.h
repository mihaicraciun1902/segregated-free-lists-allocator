//Craciun Mihai Alexandru 313CA
#ifndef FUNCTII_H
#define FUNCTII_h
#include "sfl.h"

int checkcommand(char *command);
void initheap(int base, int num_lists, int bpl, dump *data, sfl_t *sfl);
void sfl_create(int num_lists, int bpl, dump *data, sfl_t *sfl);
void freeall(sfl_t *sfl, dump *data, a_l *al);
void my_free(int free_addr, sfl_t *sfl, dump *data, a_l *al);
void my_malloc(int malloc_bytes, sfl_t *sfl, dump *data, a_l *al);
void bubblesort(node_t *start, unsigned int size);
void my_read(int r_addr, int nb, sfl_t *sfl, dump *data, a_l *al);
void my_write(int w_ad, int nb, sfl_t *sfl, dump *data, a_l *al, char *w);
void dumpmemory(dump *data, sfl_t *sfl, a_l *al);
void free_list(list_t **pp_list);
list_t *create(unsigned int data_size);
void add_nth_node(list_t *list, unsigned int n, const void *data);
node_t *remove_nth_node(list_t *list, unsigned int n);
void swap(node_t *a, node_t *b);
#endif
