/*
 * author: iancain
 *
 * additions: armaan roshani
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include "memory_pool.h"



bool DEBUG = true;


///
/// \brief DEBUG PRINTER function
///
///  \param id an integer that defines thread number (0 for main)
///  \param msg a string containing message to be printed
///
/// Prints to standard error
/// \warning NOT thread-safe; must be called within a thread-safe scope
///
void debugPrinter(std::string id, std::string msg) {
    
    //std::cerr << "***\tDEBUG from " << id << ": " 
    //          << msg << " \t***" << std::endl;
}



// PRIVATE: declared inside *.c file
typedef struct memory_pool_block_header
{
    uint32_t magic;     // NODE_MAGIC = 0xBAADA555. error checking
    size_t size;	///< \todo answer where size is defined	
    bool inuse;      	// true = currently allocated. Used for error checking

    struct memory_pool_block_header * next; ///< looks like a linked list

} memory_pool_block_header_t; ///< type alias for global namespace
                                


struct memory_pool {
    size_t count;         // total elements
    size_t block_size;    // size of each block
    size_t available;	  // (total elements) - (elements in use)

    struct memory_pool_block_header * pool; // looks like a pointer to a linked list

    void ** shadow; // shadow copy of nodes to free on destroy even if caller/user still has them in acquired state
};  // memory_pool_t (typedef in header)



//---
// MACROS
//

// HTODB = header to data block
//     converts header pointer to container data block
//
//     pointer arithmetic:
//         takes in:
//             * header pointer as number
//             * block size as number
//         outputs:
//             header memory address - block size
//
//     This seems to imply that:
//         * pointers point to lowest address of relevant block of memory
//         * the datablock exists "below" the header in address space
//
#define MEMORY_POOL_HTODB(_header_, _block_size_) ((void *)_header_ - _block_size_)

// DBTOH = data block to header
//     convert data block pointer to point to embedded header information block
//
#define MEMORY_POOL_DBTOH(_data_block_, _block_size_) ((memory_pool_block_header_t *)(_data_block_ + _block_size_))

// magic value to check for data corruption
#define NODE_MAGIC 0xBAADA555



/// \brief memory pool initializer
///
/// \param count number of nodes (headers) in pool
/// \param block_size size of each data block pointed to by nodes
///
/// \returns NULL if OOM, or if for loop not fully traversed
/// \returns memory address of mp if successful
///
/// \todo make sure this does not leak memory if for loop not fully traversed (case n != count)
///
memory_pool_t * memory_pool_init(size_t count, size_t block_size)
{
    memory_pool_t *mp = NULL; 		         // pointer to allocate 
    //memory_pool_block_header_t * last;  // pointer to header
    memory_pool_block_header_t * header = NULL;  // pointer to header
    memory_pool_block_header_t * temp;           // pointer to header
    void * block = NULL;		         // pointer to data block
    int n = 0;

    // allocate memory pool struct. give ownership back to caller
    mp = (memory_pool_t*) malloc (sizeof(memory_pool_t)); // create memory pool object
    if( mp == NULL ) {
        printf("ERROR: memory_pool_init: unable to malloc memory_pool_t. OOM\n"); /// out of memory
        return NULL;
    }

    for( n = 0; n < count; ++n ) {
        if(DEBUG) {
	    printf("*** DEBUG:\tinside for loop, count is: %d\t***\n", n);
	}
	
        // allocate data block: data block size + header size
        //
        size_t total_size = block_size + sizeof(memory_pool_block_header_t);
        block = (void *) malloc (total_size);

        // move to end of data block to create header
        //
	temp = header;
        header = MEMORY_POOL_DBOTH(block, block_size);
	header->next = temp;

	

        // add to stack (just a simple stack)

        printf("MEMORY_POOL: i=%d, data=%p, header=%p, block_size=%zu, next=%p\n",
               n,             // int
	       block,         // pointer
	       header,        // pointer
	       header->size,  // size_t
	       header->next); // pointer
    }

    printf("memory_pool_init(mp=%p, count=%zu, block_size=%zu)\n", mp, count, block_size);

    // populate variables in new memory_pool object
    mp->count = count;		  // total elements
    mp->block_size = block_size;  // size of each block
    mp->available = count;


    /// \todo answer why memory_pool_block_header * pool is not populated with an 
    ///       address at this point


    // error check: if for loop traversed count times, return mp memory address
    // 	            else, return NULL
    return n == count ? mp : NULL;
}



bool memory_pool_destroy(memory_pool_t *mp)
{

    printf("memory_pool_destroy(mp = %p, count=%zu, block_size=%zu)\n", mp, mp->count, mp->block_size);

    for(int n = 0; n < mp->count; ++n ) {
        // free all data blocks from pool
    }

    // free memory pool itself

    return true;
}



void * memory_pool_acquire(memory_pool_t * mp)
{
    // pop stack

    // get data block from header
    void * data = MEMORY_POOL_HTODB(header, mp->block_size);

    printf("memory_pool_acquire: mp=%p, data=%p\n", mp, data);
    return data;  // return to caller
}



bool memory_pool_release(memory_pool_t *mp, void * data)
{
    // move to header inside memory block using MEMORY_POOL_DBTOH(data, mp->block_size);

    printf("memory_pool_release: data=%p, header=%p, block_size=%zu, next=%p\n",
           data, header, header->size, header->next);

    // push on stack
    return true;
}



size_t memory_pool_available(memory_pool_t *mp)
    memory_pool_block_header_t * header = mp->pool;
{
    if( mp == NULL ) {
        printf("ERROR: memory_pool_available: memory pool invalid\n");
        return 0;
    }
    return mp->available;
}



void memory_pool_dump(memory_pool_t *mp)
{
    if( mp == NULL ) {
        printf("ERROR: memory_pool_dump: memory pool invalid\n");
        return;
    }

    printf("memory_pool_dump(mp = %p, count=%zu, available=%zu, block_size=%zu)\n",
            mp, 	     // pointer
	    mp->count, 	     // size_t
	    mp->available,   // size_t
	    mp->block_size); // size_t

    // point to initial header
    memory_pool_block_header_t * header = mp->pool;

    for(int n = 0; n < mp->available; ++n ) {

	// use header-to-data-block macro to create pointer
	// pointer points to 
	//
        void * data_block = MEMORY_POOL_HTODB(header, mp->block_size);

        printf(" + block: i=%d, data=%p, header=%p, inuse=%s, block_size=%zu, next=%p\n",
               n,                               // int
	       data_block,                      // pointer
	       header, 				// pointer
	       header->inuse ? "TRUE":"FALSE",  // bool
	       header->size, 			// size_t
	       header->next);			// pointer

        header = header->next;
    }
}
