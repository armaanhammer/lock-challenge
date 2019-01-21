///
/// author: armaan roshani
///


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include "memory_pool.h"




void main () 
{
    bool DEBUG = true;

    memory_pool_t * pool_1;
    size_t count_1 = 10;
    size_t block_size_1 = 1;

    printf("main: pool_1=%p, count_1=%zu, block_size_1=%zu\n", 
            pool_1, count_1, block_size_1);

    pool_1 = memory_pool_init(count_1, block_size_1);

    size_t available_1 = memory_pool_available(pool_1);

    printf("main: pool_1=%p, pool_1->available=%zu\n", pool_1, available_1);

   
    void * test;
    test = memory_pool_acquire(pool_1);


    int too_many = count_1 + 2;

    // try to pop (aquire) too many data blocks
    for( int n = 0; n < too_many; ++n ) {
        
        memory_pool_acquire(pool_1);

    }


    printf("\n\nmain: tried to aquire %d from pool_1\n\n", too_many);



    printf("test=%p\n", test);


    memory_pool_release(pool_1, test);






    

    bool worked = memory_pool_destroy(pool_1);

    printf("destroy worked: %s\n", 
           worked ? "TRUE":"FALSE");  // bool


    return;
}

