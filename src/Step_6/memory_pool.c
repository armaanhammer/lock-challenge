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



bool DEBUG = false;




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
/// Allocates one memory pool struct and `count` number of memory pool block and header
/// combinations. Also allocates memory for `count` number of pointers in `shadow` pointer
/// array.
///
memory_pool_t * memory_pool_init(size_t count, size_t block_size)
{
    memory_pool_t *mp = NULL; 		            // pointer to allocate 
    memory_pool_block_header_t * header = NULL; // pointer to header
    memory_pool_block_header_t * temp;          // pointer to header
    void * block = NULL;		                // pointer to data block
    int n = 0;

    // allocate memory pool struct. give ownership back to caller
    mp = (memory_pool_t*) malloc (sizeof(memory_pool_t)); // create memory pool object
    if( mp == NULL ) {
        printf("ERROR: memory_pool_init: unable to malloc memory_pool_t. OOM\n"); /// out of memory
        return NULL;
    }

    // allocate memory pool block and header combos
    for( n = 0; n < count; ++n ) {
        
        if(DEBUG) 
	        printf("*** DEBUG:\tinside for loop, count is: %d\t***\n", n);
	
        // allocate data block: data block size + header size
        //
        size_t total_size = block_size + sizeof(memory_pool_block_header_t);
        block = (void *) malloc (total_size);

        // move to end of data block to create header
        //
	    temp = header;
        header = MEMORY_POOL_DBTOH(block, block_size);

        // add to stack (just a simple stack)
	    // implementing as a linked list
	    header->next = temp;
	    header->size = block_size;

        printf("MEMORY_POOL: i=%d, data=%p, header=%p, block_size=%zu, next=%p\n",
               n,             // int
	           block,         // pointer
	           header,        // pointer
	           header->size,  // size_t
	           header->next); // pointer
    }

    // allocate shadow array of pointers
    mp->shadow = (void **) calloc(count, sizeof(memory_pool_block_header_t *));    

    printf("memory_pool_init: mp=%p, count=%zu, block_size=%zu\n", mp, count, block_size);

    // populate variables in new memory_pool object
    mp->count = count;		      // total elements
    mp->block_size = block_size;  // size of each block
    mp->available = count;

    // attach pool 
    mp->pool = header;

    // error check: if for loop traversed count times, return mp memory address
    // 	            else, return NULL
    return n == count ? mp : NULL;
}



bool memory_pool_destroy(memory_pool_t *mp)
{
    if(mp == NULL) {  // don't want to deference NULL
        printf("ERROR: memory_pool_destroy: nothing to destroy.\n");
        return false;
    }
    
    printf("memory_pool_destroy: mp=%p, pool=%p, count=%zu, available=%zu, block_size=%zu\n", 
           mp,
           mp->pool,
	       mp->count, 
	       mp->available, 
	       mp->block_size);

    memory_pool_block_header_t * header = mp->pool;
    memory_pool_block_header_t * next;

    // free all non-aquired data blocks
    //for(int n = 0; n < mp->count; ++n ) {
    for(int n = 0; n < mp->available; ++n ) {

        printf("memory_pool_destroy: freeing non-aquired data block # %d\n", n+1);
        
	    next = header->next;
	
	    void * data_block = MEMORY_POOL_HTODB(header, mp->block_size);
	    free(data_block);

	    /// \todo make sure that header gets freed as well with datablock

	    header = next;
    }

    // free all aquired data blocks
    for(int n = 0; n < (mp->count - mp->available ); ++n ) {

        printf("memory_pool_destroy: freeing aquired data block # %d\n", n+1);
        
        if(mp->shadow[n] != NULL) { //don't want to free(NULL), causing coredump
            
            void * data_block = MEMORY_POOL_HTODB(header, mp->block_size);
	        free(data_block);
	    }
    }

    // free memory pool itself
    free(mp);

    return true;
}



/// \brief memory pool pop function
///
/// \param mp a pointer to a memory pool object
///
/// \returns NULL if nothing to pop
/// \returns data a pointer to top data block
///
void * memory_pool_acquire(memory_pool_t * mp)
{
    if(mp->pool == NULL) {  // don't want to deference NULL
        printf("ERROR: memory_pool_acquire: nothing to aquire.\n");
        return NULL;
    }
        
    // grab pointer
    memory_pool_block_header_t * header = mp->pool;

    // pop stack
    mp->pool = mp->pool->next;

    // pool housekeeping
    --(mp->available);

    // header housekeeping
    header->inuse = true;
    header->next = NULL;

    // shadow housekeeping
    int slot = mp->count - mp->available;
    mp->shadow[slot] == header;

    // get data block from header
    void * data = MEMORY_POOL_HTODB(header, mp->block_size);

    printf("memory_pool_acquire: mp=%p, data=%p\n", mp, data);
    return data;  // return to caller
}


/// \brief memory pool push function
///
/// \param mp a pointer to a memory pool object
/// \param data a pointer to a data block with attached header
///
/// \returns true if successful
///
bool memory_pool_release(memory_pool_t *mp, void * data)
{
    // move to header inside memory block using MEMORY_POOL_DBTOH(data, mp->block_size);
    memory_pool_block_header_t * header = MEMORY_POOL_DBTOH(data, mp->block_size);

    printf("memory_pool_release: data=%p, header=%p, block_size=%zu, next=%p\n",
           data, header, header->size, header->next);

    // grab pointer
    memory_pool_block_header_t * next = mp->pool;

    // push on stack
    mp->pool = header;
    header->next = next;

    // header housekeeping
    header->inuse = false;

    // shadow housekeeping
    int slot = mp->count - mp->available;
    mp->shadow[slot] == NULL;
    
    // pool housekeeping
    //
    // do this after shadow housekeeping to allow (count - available)
    // to include correct slot
    ++(mp->available);

    return true;
}



/// \brief memory pool availability function
///
/// \param mp a pointer to a memory pool object
///
/// \returns number of available data blocks
/// \returns 0 if no data blocks available or mp invalid
///
size_t memory_pool_available(memory_pool_t *mp)
{
    if( mp == NULL ) {
        printf("ERROR: memory_pool_available: memory pool invalid\n");
        return 0;
    }
    return mp->available;
}



/// \brief memory pool dump function
///
/// \param mp a pointer to a memory pool object
///
/// Iterates through all free data blocks in memory pool, printing metadata
/// about each to user.
void memory_pool_dump(memory_pool_t *mp)
{
    if( mp == NULL ) {
        printf("ERROR: memory_pool_dump: memory pool invalid\n");
        return;
    }

    printf("memory_pool_dump: mp = %p, count=%zu, available=%zu, block_size=%zu\n",
           mp, 	            // pointer
	       mp->count, 	    // size_t
	       mp->available,   // size_t
	       mp->block_size); // size_t

    // point to initial header
    memory_pool_block_header_t * header = mp->pool;

    for(int n = 0; n < mp->available; ++n ) {

        // use header-to-data-block macro to create pointer
        void * data_block = MEMORY_POOL_HTODB(header, mp->block_size);

        printf(" + block: i=%d, data=%p, header=%p, inuse=%s, block_size=%zu, next=%p\n",
               n,                               // int
	           data_block,                      // pointer
	           header, 				            // pointer
	           header->inuse ? "TRUE":"FALSE",  // bool
	           header->size, 			        // size_t
	           header->next);			        // pointer

        header = header->next;
    }

    return;
}
