///
/// author: armaan roshani
///
/// Testbench for memory_pool.c
///


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include <time.h>
#include "memory_pool.h"



// magic value to check for data corruption
#define NODE_MAGIC 0xBAADA555





void main () 
{
    size_t count;
    size_t block_size;
    size_t available;
    bool returned_bool;


    //////////////////////////////////////////////////////
    // TEST 1
    //
    // basic create and destruct 
    //////////////////////////////////////////////////////

    printf("\n\n\t****************** TEST 1 *****************\n\n\n");
    
    memory_pool_t * pool_1;
    count = 5;
    block_size = 1;

    printf("main: pool_1=%p, count=%zu, block_size=%zu\n", 
            pool_1, count, block_size);

    // initialze
    pool_1 = memory_pool_init(count, block_size);

    // destruct
    returned_bool = memory_pool_destroy(pool_1);

    printf("main: destroy returned: %s\n", 
           returned_bool ? "TRUE":"FALSE");


    //////////////////////////////////////////////////////
    // TEST 2
    //
    // basic tests of aquisition and release
    //////////////////////////////////////////////////////
    
    printf("\n\n\t****************** TEST 2 *****************\n\n\n");

    memory_pool_t * pool_2;
    count = 5;
    block_size = 4;

    printf("main: pool_1=%p, count=%zu, block_size=%zu\n", 
            pool_2, count, block_size);

    // initialze
    pool_2 = memory_pool_init(count, block_size);

    // check available size
    available = memory_pool_available(pool_2);
    printf("main: pool_2=%p, pool_2->available=%zu\n", pool_2, available);

    // acquire 1 data block
    void * test_block;
    test_block = memory_pool_acquire(pool_2);
    printf("main: pointer aquired=%p\n", test_block);
    
    // try to pop (aquire) too many data blocks
    int too_many = count + 2;
    for( int n = 0; n < too_many; ++n ) {
        memory_pool_acquire(pool_2);
    }

    printf("main: tried to aquire %d more from pool_1\n", too_many);

    // dereference memory pointed to by test_block
    unsigned int u_data = *(unsigned int*) test_block;

    printf("main: test_block=%p, dereferenced=%u\n", test_block, u_data);

    // release back the first acquired block to test destruction
    memory_pool_release(pool_2, test_block);
    
    // destruct
    returned_bool = memory_pool_destroy(pool_2);

    printf("main: destroy returned: %s\n", 
           returned_bool ? "TRUE":"FALSE");  // bool


    //////////////////////////////////////////////////////
    // TEST 3
    //
    // 1. create random size memory pool
    // 2. acquire all data blocks
    // 3. fill all data blocks using magic value
    // 4. relese all data blocks back into pool
    // 5. dump all data before destruct
    //////////////////////////////////////////////////////

    printf("\n\n\t****************** TEST 3 *****************\n"
           "\t* 1. create random size memory pool\n"
           "\t* 2. acquire all data blocks\n"
           "\t* 3. fill all data blocks using magic value\n"
           "\t* 4. release all data blocks back into pool\n"
           "\t* 5. dump all data before destruct\n"
           "\t*******************************************\n\n\n");
    
    // seed random number generator
    srand(time(0));

    // between 1 and 10 data blocks
    count = (rand() % 9) +1;

    memory_pool_t * pool_3;
    block_size = sizeof(NODE_MAGIC);

    printf("main: pool_1=%p, count=%zu, block_size=%zu\n", 
            pool_3, count, block_size);

    // initialze
    pool_3 = memory_pool_init(count, block_size);

    // create an array of pointers to store all data blocks
    void ** magic_array = (void **) calloc(count, sizeof(0xBAADA555));

    // aquire all blocks in pool
    for( int n = 0; n < count; ++n ) {
        magic_array[n] = memory_pool_acquire(pool_3);
    }

    // print data in all aquired blocks (should all be zero)
    for( int n = 0; n < count; ++n ) {
        printf("main: reading: magic_array[%d]=%p, data=%X\n", 
                n, magic_array[n], *(unsigned int*) (magic_array[n]));
    }

    // change value of each data block to magic value
    for( int n = 0; n < count; ++n ) {
        *(unsigned int*) magic_array[n] = 0xBAADA555;
        printf("main: writing: magic_array[%d]=%p, data=%X\n", 
                n, magic_array[n], *(unsigned int*) (magic_array[n]));
    }

    // release all blocks back into pool
    for( int n = 0; n < count; ++n ) {
        returned_bool = memory_pool_release(pool_3, magic_array[n]);
    }

    // re-aquire all blocks in pool
    for( int n = 0; n < count; ++n ) {
        magic_array[n] = memory_pool_acquire(pool_3);
    }

    // print data in all re-aquired blocks
    for( int n = 0; n < count; ++n ) {
        printf("main: reading: magic_array[%d]=%p, data=%X\n", 
                n, magic_array[n], *(unsigned int*) (magic_array[n]));
    }

    // dump pool prior to destructing
    memory_pool_dump(pool_3);

    // destruct
    returned_bool = memory_pool_destroy(pool_3);

    printf("main: destroy returned: %s\n", 
           returned_bool ? "TRUE":"FALSE");  // bool

    return;
}

