#include <bits/stdc++.h>
using namespace std;

struct memory_block {
    memory_block *next ;
    size_t size;
    bool isFree;
};

#define M_BLOCK_SIZE (sizeof(memory_block))

static memory_block* first = nullptr;


memory_block* request_memory(size_t size){
    memory_block *block = (memory_block*) sbrk(0);
    void *new_block = sbrk(M_BLOCK_SIZE + size);
    if (new_block == (void *)-1) {
        // memory not available
        return NULL;
    }
    
    block->size = size;
    block->isFree = false; // now it's occupied
    block->next = nullptr;
    if(!first) {
        first = block; // if it's first time request
    } else {
        memory_block *temp = first;
        while(temp->next != nullptr){
            temp = temp->next;
        }
        
        temp->next = block;  /// adding last entry
    }
    
    return block;
}



memory_block* find_memory(size_t size) {
    memory_block* current = first;
    while(current != NULL){
        if(current->isFree && current->size >= size){
            return current;
        }
        current = current->next;
    }
    
    return current;
}

void split_memory(memory_block* block, size_t size){
    if (block->size > size + M_BLOCK_SIZE) {
        memory_block *new_block = (memory_block*)((uint8_t *)block + size + M_BLOCK_SIZE);
        new_block->isFree = true;
        new_block->size = block->size - size - M_BLOCK_SIZE;
        
        new_block->next = block->next;
        block->next = new_block;
    }
}


void consolidate_memory() {
    memory_block *current = first;
    while(current != NULL && current->next != NULL) {
        if (current->isFree &&current->next->isFree){
            current->size += current->next->size + M_BLOCK_SIZE;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
        
    }
}



void *my_maloc(size_t size){
    if (size <= 0) return NULL;
    
    memory_block* block;
    
    if(!first){
        block = request_memory(size);
        return (void *)((uint8_t*)block + M_BLOCK_SIZE);
    }
    
    block = find_memory(size);
    if (!block) {
        block = request_memory(size);
        return (void *)((uint8_t*)block + M_BLOCK_SIZE);
    }
    
    // here you can define split memory function to make more fragments
    // the bigger memory to get use in another funtion
    split_memory(block,size);
    return (void *)((uint8_t*)block + M_BLOCK_SIZE);
    
}



void my_free(void *s){
    if (s!= NULL){
        memory_block *obj = (memory_block*)((uint8_t *)s-M_BLOCK_SIZE);
        obj->isFree = true;
        consolidate_memory();
        // You can create function to consolidate the memories 
        // so that bigger memory requirement can be surve
    }
    
}
/// for defing the custom calloc 
// just update the function parameter , like size , number of block requrired
// then call the my malloc and after that 
/// set each addresh values as 0 by default (that is main deiffernt in malloc vs calloc 



int main()
{
    int *p = (int *)my_maloc(4*sizeof(int));
    
    p[0] = 1;
    p[1] = 2;
    p[2] = 3;
    p[3] = 4;
    
    cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3];
    
    my_free(p);
   
  
   
    return 0;
}
