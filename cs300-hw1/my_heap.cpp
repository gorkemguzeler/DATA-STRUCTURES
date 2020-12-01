//
//  my_heap.cpp
//  cs300-hw1
//
//  Created by gorkemg on 8.11.2020.
//

#include <stdio.h>
#include "my_heap.h"
#include <iostream>

using namespace std;

int memory_leak = 0 ;

My_heap::My_heap(){
    heap_begin = nullptr;
    blk = nullptr;
    used_bytes = 0;
}

My_heap::~My_heap(){
    memory_block * ptr = heap_begin;
    
    cout << "At destruction, the heap had a memory leak of " << used_bytes << " bytes" << endl;
    
    while (ptr != nullptr) {
        heap_begin = heap_begin->right;
        delete ptr;
        ptr = heap_begin;
    }
    blk = nullptr;
}

memory_block * My_heap::bump_allocate(int num_bytes){
    
    if(num_bytes <= (MAX_CAPACITY - used_bytes)){
        
        if(heap_begin == nullptr){ //add to first position
            memory_block* ptr = new memory_block(nullptr , nullptr ,true, num_bytes, 0 );
            heap_begin = ptr;
            blk = ptr;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
        else{ //add to the end
            memory_block* ptr = new memory_block(blk , nullptr ,true, num_bytes, (blk->starting_address + blk->size) );
            blk->right = ptr;
            blk = blk->right;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
    }
    else //no capacity
        return nullptr;
}

memory_block * My_heap::first_fit_allocate(int num_bytes){
    
    if(num_bytes <= (MAX_CAPACITY- used_bytes)){
        
        memory_block * temp = heap_begin ;
        
        while(temp != nullptr){ //starting from beginning check if any block is suitable.
            if(temp->used == false && temp->size >= num_bytes ){
                temp->used = true;
                used_bytes = used_bytes + temp->size;
                return temp;
            }
            temp = temp->right;
        }
        
        if(heap_begin == nullptr){ //add to first position
            memory_block* ptr = new memory_block(nullptr , nullptr ,true, num_bytes, 0 );
            heap_begin = ptr;
            blk = ptr;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
        else{ //add to the end
            memory_block* ptr = new memory_block(blk , nullptr ,true, num_bytes , (blk->starting_address + blk->size) );
            blk->right = ptr;
            blk = blk->right;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
        
    }
    else //no capacity
        return nullptr;
    
}

memory_block * My_heap::best_fit_allocate(int num_bytes){
    
    if(num_bytes <= (MAX_CAPACITY - used_bytes)){
        
        memory_block * temp = heap_begin ;
        memory_block * smallest = nullptr ;
        int count = 0;
        
        while(temp != nullptr){ //starting from beginning check if any block is suitable.
            if(temp->used == false && temp->size >= num_bytes ){
                
                if(count == 0){ //only use for the first time
                    smallest = temp;
                    count = 1;
                }
                
                if(temp->size  < smallest->size){
                    smallest = temp;
                }
            }
            temp = temp->right;
        }
        
        if(count == 1){ //there is any empty slot
            used_bytes = used_bytes + smallest->size;
            smallest->used = true;
            return smallest;
        }
        
        if(heap_begin == nullptr){ //add to first position
            memory_block* ptr = new memory_block(nullptr , nullptr ,true, num_bytes, 0 );
            heap_begin = ptr;
            blk = ptr;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
        else{ //add to the end
            memory_block* ptr = new memory_block(blk , nullptr ,true, num_bytes, (blk->starting_address + blk->size));
            blk->right = ptr;
            blk = blk->right;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
    }
    else //no capacity
        return nullptr;
}

memory_block * My_heap::first_fit_split_allocate(int num_bytes){
    
    if(num_bytes <= (MAX_CAPACITY - used_bytes)){
        
        memory_block * temp = heap_begin ;
        
         while(temp != nullptr){ //starting from beginning check if any block is suitable.
            if(temp->used == false && temp->size == num_bytes ){
                temp->used = true;
                used_bytes = used_bytes + num_bytes;
                return temp;
            }
            else if(temp->used == false && temp->size > num_bytes ){ //split the block
                
                temp->right = new memory_block(temp, temp->right, false, (temp->size - num_bytes), (temp->starting_address + num_bytes) ) ;
                if(temp->right->right != nullptr){
                    temp->right->right->left = temp->right;
                }
                used_bytes = used_bytes + num_bytes;
                temp->size = num_bytes;
                temp->used = true;
                
                return temp;
            }
            temp = temp->right;
        }
        
        if(heap_begin == nullptr){ //add to first position
            memory_block* ptr = new memory_block(nullptr , nullptr ,true, num_bytes, 0 );
            heap_begin = ptr;
            blk = ptr;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
        else{ //add to the end
            memory_block* ptr = new memory_block(blk , nullptr ,true, num_bytes, (blk->starting_address + blk->size) );
            blk->right = ptr;
            blk = blk->right;
            used_bytes = used_bytes + num_bytes;
            ptr->used = true;
            return ptr;
        }
        
    }
    else //no capacity
        return nullptr;
}

void My_heap::deallocate(memory_block* block_address){
    
    if(block_address != nullptr){
        block_address->used = false;
        used_bytes = used_bytes - block_address->size;
        
        if(block_address->right != nullptr){
            if(block_address->right->used == false){ //delete right node
                memory_block* to_be_deleted = block_address->right;
                block_address->size = block_address->size + block_address->right->size;
                
                if(block_address->right == blk){
                    blk = blk->left;
                }
                block_address->right = block_address->right->right;
                
                if(block_address->right != nullptr){
                    block_address->right->left = block_address;
                }
              delete to_be_deleted;
            }
        }
        
        if(block_address->left != nullptr){
            if(block_address->left->used == false){ //delete current node
                memory_block* to_be_deleted2 = block_address;
                block_address->left->size = block_address->left->size + block_address->size;
                
                if(block_address == blk){
                    blk = blk->left;
                }
                block_address->left->right = block_address->right;
            
                if(block_address->right != nullptr){
                    block_address->right->left = block_address->left;
                }
               delete to_be_deleted2;
            }
        }
    }
}

void My_heap::print_heap(){
    
    int total_memory_blocks= 0;
    int used_memory_blocks= 0;
    
    memory_block * ptr = heap_begin;
    memory_block * ptr1 = heap_begin;
    memory_block * ptr2 = heap_begin;
    
    while(ptr!= nullptr){
        total_memory_blocks++;
        ptr = ptr->right;
    }
    
    while(ptr1!= nullptr){
        if(ptr1->used == true){
            used_memory_blocks++;
        }
        ptr1=ptr1->right;
    }
    
    int free_memory_blocks= total_memory_blocks - used_memory_blocks ;
    
    cout << "Maximum capacity of heap: 512B" << endl;
    
    if(used_bytes < 0){
        used_bytes = 0;
    }

    cout << "Currently used memory (B): " << used_bytes << endl;
    
    cout << "Total memory blocks: " << total_memory_blocks << endl;
    
    cout << "Total used memory blocks: " << used_memory_blocks << endl;
    
    cout << "Total free memory blocks: " << free_memory_blocks << endl;
    
    cout << "Fragmentation: " << get_fragmantation() << "%" << endl;
    
    cout << "------------------------------" << endl;
    
    int block_num = 0;
    
    while(ptr2 != nullptr){
        
        cout << "Block ​" << block_num << "\t\tUsed: ";
        
        if(ptr2->used == false){
            cout << "False" ;
        }
        else
            cout << "True" ;
        
        cout << "\t" << "Size (B): "<< ptr2->size << "\t" ;
        
        cout << hex << "Starting address: 0x" << ptr2->starting_address << dec <<  "\n" ;
        
        block_num++;
        ptr2 = ptr2->right;
    }
    
    cout << "------------------------------" << endl;
    
    cout << "------------------------------" << endl;
    
}


float My_heap::get_fragmantation(){
    float fragmentation = 0, free_memory , biggest_free_block = 0;
    free_memory = MAX_CAPACITY - used_bytes;
    int free_mem_blocks = 0;
    
    //to find biggest_free_block, we will go over each block end try to find the maximum one the compare with the free space after the last full node:
    memory_block * temp = heap_begin ;
    
    while(temp != nullptr){ //starting from beginning check if any block is suitable.
        
        if(temp->used == false){
        free_mem_blocks = free_mem_blocks + temp->size;
        }
        
        if(temp->used == false && temp->size > biggest_free_block){
            biggest_free_block = temp->size;
        }
        
        temp = temp->right;
    }
    
    if(blk!=nullptr){
        if(biggest_free_block < MAX_CAPACITY -(free_mem_blocks+ used_bytes ) )
        { //if biggest_free_block is lower than the the memory after the last node
        biggest_free_block = MAX_CAPACITY -(free_mem_blocks+ used_bytes) ; 
    }
    }
    fragmentation = (free_memory - biggest_free_block)/free_memory*100;
    
    return fragmentation;
}
