//
//  my_heap.h
//  cs300-hw1
//
//  Created by gorkemg on 8.11.2020.
//

#ifndef my_heap_h
#define my_heap_h

#define MAX_CAPACITY 512

struct memory_block{
        
    memory_block* left; // node to the left
    memory_block* right; // node to the right
    bool used; // if this memory block is reserved or not
    int size;// the number of bytes reserved in Img_heap
    int starting_address; // the starting address in Img_heap of the memory reserved by this block

    memory_block () {}

    memory_block (memory_block* l, memory_block*r ,bool u , int s , int s_a): left(l),right(r),used(u),size(s),starting_address(s_a)  {}
    
};

class My_heap{
    
private:
    
    memory_block* heap_begin; //​a pointer at the ​first​ ​memory_block​ in ​My_heap​.
    memory_block* blk; // a pointer at the ​last ​memory_block​ in ​My_heap​.
    int used_bytes; //the total number of allocated bytes in ​Img_heap​.
    
public:
    
    My_heap(); //creates an empty ​My_heap​.
    ~My_heap();
    memory_block* bump_allocate(int num_bytes); //allocate a new region in ​Img_heap, details in document
    memory_block* first_fit_allocate(int num_bytes);
    memory_block* best_fit_allocate(int num_bytes);
    memory_block* first_fit_split_allocate(int num_bytes);
    void deallocate(memory_block* block_address);
    void print_heap();
    float get_fragmantation();
};

#endif /* my_heap_h */
