#include <stdio.h>
#include <stddef.h>
//#include "modifiedmalloc.h"

static char myblock[5000];

struct block
{
	size_t blocksize;
	int isFree; //Should only be 0 if not free and 1 if it is free
    struct block *next;
    //struct block *prev;
};
typedef struct block Block;

Block *List = (void *) myblock; //Points to the first block 


void separate(Block *fitblock, size_t size){
	Block *temp = (void *)((void *)fitblock + size + sizeof(Block));
	temp->isFree = 1;
	temp->blocksize = (fitblock->blocksize) - size - sizeof(Block);
	temp->next = fitblock->next;
	temp->prev = fitblock->prev;
	fitblock->isFree = 0;
	fitblock->blocksize = size;
	fitblock->next = temp;
}

//Uses the First-fit algo
void *mymalloc(size_t size){
	Block *prev, *current;
	void *end;
	//Checks if the List is empty or not and starts it if it is empty
	if(!(List->blocksize)){
		List->blocksize = 5000 - sizeof(Block);
		List->next = NULL;
		List->prev = NULL;
		List->isFree = 1;
	}
	current = List;
	//Traverse through the blocks and found the 1st one
	while( ( ( (current->blocksize) < size) || ( (current->isFree) == 0) ) && (current->next != NULL) ){
		prev=current;
		current=current->next;
	}
	//The block fits perfectly
	if(current->blocksize == size){
		current->isFree = 0;
		end = (void*)(++current);
		return end;
	}
	//Needed to split the block in order to allocate
	else if((current->blocksize) > (size + sizeof(Block))){
		separate(current, size);
		end = (void *)(++current);
		return end;
	}else{
		end = NULL;
		printf("Not enough space is available \n Suck a Cock \n");
		return end;
	}
	
}

void coalesce(){
	block *current, *prev;
	current = List;
	while((current->next) != NULL){
		if((current->isFree) && (current->next->isFree)){
			current->blocksize +=(current->next->blocksize) + sizeof(Block); //Could possible be minus the sizeof(Block) instead of plus
			current->next = current->next->next;
		}
		prev = current;
		current = current->next;
	}
}

void free(void* ptr){
	
}

//testing purposes
int main(){
	int *p=(int *)mymalloc(sizeof(int));
	*p = 10;
	printf("%d \n", &p);
	printf("%d  also suck a cock sahil \n", *p);
	return 0;
}