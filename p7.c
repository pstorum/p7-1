#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "p7.h"

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block *head = NULL;

int main(){
	
	myalloc(10); print_data();
    myalloc(20); print_data();
    myalloc(30); print_data();
    myalloc(40); print_data();
    myalloc(50); print_data();
}

void * myalloc(int allo){
	int padded_block_size;
	int alloed = allo + GET_PAD(allo);
	
	if (head == NULL) {
	    head = sbrk(1024);
	    head->next = NULL;
	    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
	    head->in_use = 0;
	}
	struct block *holding = head;

	while(holding != NULL){
		if(holding->size >= alloed){
			padded_block_size = PADDED_SIZE(sizeof(struct block));
			int required_space = alloed + padded_block_size + 16;

			if (holding->size >= required_space){
				struct block *new_block = sbrk(sizeof(holding));
				new_block->size = holding->size - (alloed + padded_block_size);
				new_block->in_use = 0;
				new_block->next = NULL;
				
				holding->in_use = 1;
				holding->size = allo;
				holding->next = new_block;
			}
			return PTR_OFFSET(holding, padded_block_size);
		}
		holding = holding->next;
	}
	return NULL;
}

void print_data(void){
	struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

void myfree(void *p){
	struct block *temp = p;
	temp->in_use = 0;
}

