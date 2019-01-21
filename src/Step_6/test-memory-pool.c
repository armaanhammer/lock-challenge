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

    printf("main: pool_1=%p, \n", pool_1);


    //printf("main: pool_1=%p, count=%zu, available=%zu, block_size=%zu\n", 
    //       pool_1, pool_1->count, pool_1->available, pool_1->block_size);



    

    

    return;
}

