#include "mem.h"

#define DYNAMIC_MEM_TOTAL_SIZE 4*1024
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamic_mem_node_t) 

typedef struct dynamic_mem_node 
{
    u32 size;
    bool used;
    struct dynamic_mem_node *next;
    struct dynamic_mem_node *prev;
} dynamic_mem_node_t;


static u8 dynamic_mem_area[DYNAMIC_MEM_TOTAL_SIZE];
static dynamic_mem_node_t *dynamic_mem_start;

void init_dynamic_mem(){
    dynamic_mem_start = (dynamic_mem_node_t*) dynamic_mem_area;
    dynamic_mem_start->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
    dynamic_mem_start->next = NULL_POINTER;
    dynamic_mem_start->prev = NULL_POINTER;
}

void *find_best_mem_block(dynamic_mem_node_t *dynamic_mem, size_t size){
    dynamic_mem_node_t *best_mem_block = (dynamic_mem_node_t *) NULL_POINTER;
    u32 best_mem_block_size = DYNAMIC_MEM_TOTAL_SIZE + 1;

    dynamic_mem_node_t *current_mem_block = dynamic_mem;
    while(current_mem_block){
        if(!(current_mem_block->used) 
        && (current_mem_block->size >= (size + DYNAMIC_MEM_NODE_SIZE)) 
        && (current_mem_block->size <= (best_mem_block))){
            best_mem_block = current_mem_block;
            best_mem_block_size = current_mem_block->size;
        }
        current_mem_block = current_mem_block->next;
    }
    return best_mem_block;
}

void *malloc(size_t size){
    dynamic_mem_node_t *best_mem_block = (dynamic_mem_node_t*) find_best_mem_block(dynamic_mem_start, size);
    if(best_mem_block != NULL_POINTER){
        best_mem_block->size = best_mem_block->size - size - DYNAMIC_MEM_NODE_SIZE;
        dynamic_mem_node_t *mem_node_allocate = (dynamic_mem_node_t *)(((u8 *) best_mem_block) + DYNAMIC_MEM_NODE_SIZE + best_mem_block->size);

        mem_node_allocate->size = size;
        mem_node_allocate->used = true;
        mem_node_allocate->next = best_mem_block->next;
        mem_node_allocate->prev = best_mem_block;

        if(best_mem_block->next != NULL_POINTER){
            best_mem_block->next->prev = mem_node_allocate;
        }
        best_mem_block->next = mem_node_allocate;

        return (void *)((u8 *)mem_node_allocate + DYNAMIC_MEM_NODE_SIZE);
    }
    return NULL_POINTER;
}

void *merge_next_node_into_current(dynamic_mem_node_t *current_mem_node) {
    dynamic_mem_node_t *next_mem_node = current_mem_node->next;
    if (next_mem_node != NULL_POINTER && !next_mem_node->used) {
        current_mem_node->size += current_mem_node->next->size;
        current_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        current_mem_node->next = current_mem_node->next->next;
        if (current_mem_node->next != NULL_POINTER) {
            current_mem_node->next->prev = current_mem_node;
        }
    }
    return current_mem_node;
}

void *merge_current_node_into_previous(dynamic_mem_node_t *current_mem_node) {
    dynamic_mem_node_t *prev_mem_node = current_mem_node->prev;
    if (prev_mem_node != NULL_POINTER && !prev_mem_node->used) {
        prev_mem_node->size += current_mem_node->size;
        prev_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        prev_mem_node->next = current_mem_node->next;
        if (current_mem_node->next != NULL_POINTER) {
            current_mem_node->next->prev = prev_mem_node;
        }
    }
}

void free(void *p){
    if(p == NULL_POINTER){
        return;
    }

    dynamic_mem_node_t *current_mem_node = (dynamic_mem_node_t *) ((u8 *) p - DYNAMIC_MEM_NODE_SIZE);
    if (current_mem_node == NULL_POINTER) {
        return;
    }

    current_mem_node->used = false;

    current_mem_node = merge_next_node_into_current(current_mem_node);
    merge_current_node_into_previous(current_mem_node);

}


void memory_copy(u8 *source, u8 *dest, int no_bytes){
    for(int i = 0; i<no_bytes; i++){
        *(dest + i) = *(source + i);
    }    
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}