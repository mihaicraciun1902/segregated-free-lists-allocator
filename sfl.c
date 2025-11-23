//Craciun Mihai Alexandru 313CA
#include "sfl.h"
#include "functii.h"

int main(void)
{
	sfl_t sfl; // head
	a_l *al;// active list
	al = (a_l *)malloc(sizeof(a_l));
	dump data;
	char command[256];
	int base; // heap start addres
	int num_lists; // number of lists
	int bpl; // bytes per list
	int rec_type; // recon type
	int nb; // nr of bytes
	while (1) {
		scanf(" %s", command); // reads input command
		int op = checkcommand(command); // gets the command id

	switch (op) {
	case INIT_HEAP: {
		scanf("%x%d%d%d", &base, &num_lists, &bpl, &rec_type);
		if (rec_type == 1) // no bonus
			return 0;

		initheap(base, num_lists, bpl, &data, &sfl);
		break;
	}
	case MALLOC: {
		int malloc_bytes;
		scanf("%d", &malloc_bytes);

		if (data.active_dim == data.active_size_max) {
			int r = 2 * data.active_dim;
			al = (a_l *)realloc(al, r * sizeof(a_l));
			data.active_size_max = 2 * data.active_dim;
		}

		my_malloc(malloc_bytes, &sfl, &data, al);
		break;
	}
	case FREE: {
		int free_addr;
		scanf("%x", &free_addr);
		my_free(free_addr, &sfl, &data, al);
		break;
	}
	case READ: {
		int r_add; // read addres
		scanf("%x %d", &r_add, &nb);
		my_read(r_add, nb, &sfl, &data, al);
		break;
	}
	case WRITE: {
		int w_ad; // write address
		char w[100];
		scanf("%x \"%[^\"]\" %d", &w_ad, w, &nb);
		my_write(w_ad, nb, &sfl, &data, al, w);
		break;
	}
	case DUMP_MEMORY: {
		dumpmemory(&data, &sfl, al);
		break;
	}
	case DESTROY_HEAP: {
		freeall(&sfl, &data, al);
		return 0;
	}
	default: {
		printf("Invalid command\n");
		break;
	}
	}
}

	return 0;
}
