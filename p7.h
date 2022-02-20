// llist.h

#ifndef P7_H
#define P7_H

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

void * myalloc(int allo);
void print_data(void);
void myfree(void *p);
#endif