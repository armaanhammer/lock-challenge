# Step 6 Appendix

## CMAKE

### Parsing

Parsing the [included CMakeLists_1.txt](../prompt/CMakeLists_1.txt):

```
cmake_minimum_required(VERSION 3.8)

project (test-memory-pool
        VERSION 1.0.0
        DESCRIPTION "Test Memory Pool")

include_directories(.)
link_directories(../ ./)

set(CMAKE_CXX_FLAGS "-Wall -std=c99")

# create executable
add_executable (test test-memory-pool.c memory_pool.c)
set_property(TARGET test PROPERTY C_STANDARD 99)
```

Ok, so this is going to be straight C, rather than C++. Going to copy [memory_pool.c prompt](../prompt/memory_pool.c) and [memory_pool.h prompt](../prompt/memory_pool.h) to this project's src.

Also going to add debug symbols. Trying adding this line: `set(CMAKE_BUILD_TYPE Debug)`, and will also add flag `-g` to this line: `set(CMAKE_CXX_FLAGS "-Wall -std=c99")` to pass in explicitly. 


<br>


Analysis
---

### Initial

#### CMAKE

The presence of `test-memory-pool.c` in the create executable directives indicates that I am to create a testbench on my own.

#### memory_pool.h

I am given a list of objectives:

```
 * Objective: speed operations of malloc/free and adapt idiomatically and separate memory
             management from other data storage patterns such as linked lists, stacks,
             double buffering
   Limitations: Fixed sized memory blocks. Due to the O(1) requirement only fixed sized
                memory allocation can be performed. Memory fragmentation and
                collection/collating operations are not desired due to performance demands
  Support O(1) operation in acquire and release operations
  Strategy:
    stack object to manage memory blocks
       acquire = pop_front  (acquire block off the front/top of stack)
       release = push_back  (release block by putting on back/bottom of stack)
```

##### Stack idiom

I am experiencing some confusion about the stack I am to implement. I see two interpretations, and associated problems with both:

* The memory pool itself is implemented as a stack
  * If this is correct, I do not immediately see the purpose of the `* next` pointers in `memory_pool_block_header`. Could not the stack simply be one contiguous block of memory?
* The memory pool and the stack are two discrete structures; the stack is present only to test the memory pool.
  * This makes more sense immediately, but I still have reservations. This is discussed in [Deeper Analysis](#deeper-analysis) below.



#### memory_pool.c

```c
typedef struct memory_pool_block_header
{
    uint32_t magic;      // NODE_MAGIC = 0xBAADA555. error checking
    size_t size;
    bool inuse;      // true = currently allocated. Used for error checking

    struct memory_pool_block_header * next;

} memory_pool_block_header_t;
```

This is an individual block within the larger memory pool. 

The line `struct memory_pool_block_header * next;` looks very much like the next pointer in a linked list block. 

The `typedef { } memory_pool_block_header_t;` decleration avoids the need to qualify with struct when creating instances of memory_pool_block_header

```c
struct memory_pool {
    size_t count;         // total elements
    size_t block_size;   // size of each block
    size_t available;

    struct memory_pool_block_header * pool;
    void ** shadow; // shadow copy of nodes to free on destroy even if caller/user still has them in acquired state
};
```

## Deeper analysis

I find the `void ** shadow;` line fascinating. It's a pointer to a pointer (or to an array of pointers?). Presumably, I need to populate the array of pointers either with all blocks, or only with blocks that are in "aquired state". I am not immediately clear what that means. Perhaps a user function needs to lock out access to individual blocks under certain conditions? On first blush, it does **not** seem to be related to `bool inuse;` inside of struct `memory_pool_block_header`.

Comparing this with these lines in the header:
```
    stack object to manage memory blocks
       acquire = pop_front  (acquire block off the front/top of stack)
       release = push_back  (release block by putting on back/bottom of stack)
```

Seems to indicate that I am to implement a stack as well, and move intems back and forth between the memory pool and the stack.

To check my logic, I will suppose a scenario:



### example scenario

Three items in memory pool, empty stack

#### initial condition


|     | memory pool | inuse | status |     | stack |     |
| --- | ---         | ---   | ---    | --- | ---   | --- |
| pool-> | pA       | yes   |        |     |       |     |
|     | pB          | yes   |        |     |       |     |
|     | pC          | yes   |        |     |       |     |
|     |             | no    |        |     |       | <-top |


**OR**

#### intial condition

|        | memory pool | inuse | status |     | status | stack |       |
| ---    | ---         | ---   | ---    | --- | ---    | ---   | ---   |
| pool-> |             | no    |        |     |        |       |       |
|        |             | no    |        |     |        | pC    | <-top |
|        |             | no    |        |     |        | pB    |       |
|        |             | no    |        |     |        | pA    |       |

#### first aquire operation

|        | memory pool | inuse | status |     | status  | stack |       |
| ---    | ---         | ---   | ---    | --- | ---     | ---   | ---   |
| pool-> | pC          | yes   |        |     |         |       |       |
|        |             | no    |        |     | aquired | pC    |       |
|        |             | no    |        |     |         | pB    | <-top |
|        |             | no    |        |     |         | pA    |       |

#### second aquire operation

|        | memory pool | inuse | status |     | status  | stack |       |
| ---    | ---         | ---   | ---    | --- | ---     | ---   | ---   |
| pool-> | pC          | yes   |        |     |         |       |       |
|        | pB          | yes   |        |     | aquired | pC    |       |
|        |             | no    |        |     | aquired | pB    |       |
|        |             | no    |        |     |         | pA    | <-top |



## Macros

```c
//---
// MACROS
//

// HTODB = header to data block
//     converts header pointer to container data block
//
#define MEMORY_POOL_HTODB(_header_, _block_size_) ((void *)_header_ - _block_size_)

// DBTOH = data block to header
//     convert data block pointer to point to embedded header information block
//
#define MEMORY_POOL_DBTOH(_data_block_, _block_size_) ((memory_pool_block_header_t *)(_data_block_ + _block_size_))
```       

Question: why do we need to go back and forth between header and data block? I think I am missing something. Drawing hierarchy to flesh out: &darr; &#x25BC;


```
memory_pool  
--> memory_pool_block_header * pool;  
    --> memory_pool_block_header * next;  
        --> memory_pool_block_header * next;  
            --> etc...
```
               
DBTOH contains `memory_pool_block_header_t *` which is the type alias for memory_pool_block_header in the global namespace. This seems relevant, but I am not sure how yet.
         
        

## Memory Pool Initializer









<br>

```c
 header->inuse ? "TRUE":"FALSE",
```

This is clever! I like it.

<br><br>




Abstraction
---

### Design Constraints

* create pool (memory_pool)
  * composed of many blocks (memory_pool_block_header_t)
    * each block points to either:
      * next block
      * or NULL (presumably)
      
      

  
