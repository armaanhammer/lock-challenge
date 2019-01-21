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




void main () 
{
    bool DEBUG = true;

    size_t count;
    size_t block_size;
    size_t available;
    bool returned_bool;

    //////////////////////////////////////////////////////
    // TEST 1
    //
    // basic create and destruct 
    //////////////////////////////////////////////////////

    memory_pool_t * pool_1;
    count = 10;
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

    memory_pool_t * pool_2;
    count = 10;
    block_size = 1;

    printf("main: pool_1=%p, count=%zu, block_size=%zu\n", 
            pool_2, count, block_size);

    // initialze
    pool_1 = memory_pool_init(count, block_size);

    // check available size
    available = memory_pool_available(pool_2);
    printf("main: pool_1=%p, pool_1->available=%zu\n", pool_2, available);

    // acquire 1 data block
    void * test_block;
    test_block = memory_pool_acquire(pool_2);
    printf("main: pointer aquired=%p\n", test_block);
    
    // try to pop (aquire) two too many data blocks
    int too_many = count + 2;
    for( int n = 0; n < too_many; ++n ) {
        memory_pool_acquire(pool_2);
    }

    printf("\n\nmain: tried to aquire %d from pool_1\n\n", too_many);

    // release back the first acquired block to test destruction
    memory_pool_release(pool_2, test);
    
    // destruct
    returned_bool = memory_pool_destroy(pool_2);

    printf("main: destroy returned: %s\n", 
           returned_bool ? "TRUE":"FALSE");  // bool


    //////////////////////////////////////////////////////
    // TEST 3
    //
    // 1. create random size memory pool
    // 2. fill all data blocks using magic value
    // 3. hammer in and out randomly
    // 4. dump all data before destruct
    //////////////////////////////////////////////////////
    
    



    return;
}

