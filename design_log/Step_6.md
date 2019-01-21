# Step 6: Memory Pool

To duplicate
---

1. In the [/src/Step_6/build/ directory](../src/Step_6/build) run `cmake ../`. The build environment will be set up in that directory using the [CMakeLists.txt file](../src/Step_6/CMakeLists.txt) in the parent directory.
1. In the [/src/Step_6/build/ directory](../src/Step_6/build/) run `make`. Executable will build as `test.out`. 
1. To run executable, run `./test.out`.


Files
---

* [memory_pool.c](../src/Step_6/memory_pool.c) 
* [test-memory_pool.c](../src/Step_6/test-memory_pool.c) (testbench)
* [CMakeLists.txt file](../src/Step_6/CMakeLists.txt) (slightly modified from [prompt file](../prompt/CMakeLists_1.txt))


Documentation
---

Doxygen documentation is available for this step of the project. 

| [Step 6 main page](https://armaanhammer.github.io/lock-challenge/Step_6/html/index.html) | | |
| --- | --- | --- |
| memory_pool.c | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_6/html/test-memory-pool_8c.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_6/html/memory__pool_8c_source.html) |
| test-memory_pool.c | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_6/html/test-memory-pool_8c.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_6/html/test-memory-pool_8c_source.html) |


Results
---

Running the program runs all three tests in series.

### Test 1

Test 1 is a basic create and destroy. A pool with 5 blocks of size 1 are created and destroyed. The destroy function correctly identifies them as non-aquired.

```bash
	****************** TEST 1 *****************


main: pool_1=0x1, count=5, block_size=1
MEMORY_POOL: i=0, data=0x560c030926a0, header=0x560c030926a1, block_size=1, next=(nil)
MEMORY_POOL: i=1, data=0x560c030926d0, header=0x560c030926d1, block_size=1, next=0x560c030926a1
MEMORY_POOL: i=2, data=0x560c03092700, header=0x560c03092701, block_size=1, next=0x560c030926d1
MEMORY_POOL: i=3, data=0x560c03092730, header=0x560c03092731, block_size=1, next=0x560c03092701
MEMORY_POOL: i=4, data=0x560c03092760, header=0x560c03092761, block_size=1, next=0x560c03092731
memory_pool_init: mp=0x560c03092670, count=5, block_size=1
memory_pool_destroy: mp=0x560c03092670, pool=0x560c03092761, count=5, available=5, block_size=1
memory_pool_destroy: freeing non-aquired data block # 1
memory_pool_destroy: freeing non-aquired data block # 2
memory_pool_destroy: freeing non-aquired data block # 3
memory_pool_destroy: freeing non-aquired data block # 4
memory_pool_destroy: freeing non-aquired data block # 5
main: destroy returned: TRUE
```

### Test 2

Test 2 is more advanced. A pool with 5 blocks of size 4 are created. Acquire is attempted 8 times and fails (correctly) 3 times. The first block is then read correctly as containing 0. The first block is released back to the pool. Then the pool is destroyed. The destroy function correctly identifies 1 block as non-aquired, and all the rest of the blocks as aquired.

```bash
	****************** TEST 2 *****************


main: pool_1=0x560c01d8731d, count=5, block_size=4
MEMORY_POOL: i=0, data=0x560c030926a0, header=0x560c030926a4, block_size=4, next=(nil)
MEMORY_POOL: i=1, data=0x560c030926d0, header=0x560c030926d4, block_size=4, next=0x560c030926a4
MEMORY_POOL: i=2, data=0x560c03092700, header=0x560c03092704, block_size=4, next=0x560c030926d4
MEMORY_POOL: i=3, data=0x560c03092730, header=0x560c03092734, block_size=4, next=0x560c03092704
MEMORY_POOL: i=4, data=0x560c03092760, header=0x560c03092764, block_size=4, next=0x560c03092734
memory_pool_init: mp=0x560c03092670, count=5, block_size=4
main: pool_2=0x560c03092670, pool_2->available=5
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092760
main: pointer aquired=0x560c03092760
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092730
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092700
memory_pool_acquire: mp=0x560c03092670, data=0x560c030926d0
memory_pool_acquire: mp=0x560c03092670, data=0x560c030926a0
ERROR: memory_pool_acquire: nothing to aquire.
ERROR: memory_pool_acquire: nothing to aquire.
ERROR: memory_pool_acquire: nothing to aquire.
main: tried to aquire 7 more from pool_1
main: test_block=0x560c03092760, dereferenced=0
memory_pool_release: data=0x560c03092760, header=0x560c03092764, block_size=4, next=(nil)
memory_pool_destroy: mp=0x560c03092670, pool=0x560c03092764, count=5, available=1, block_size=4
memory_pool_destroy: freeing non-aquired data block # 1
memory_pool_destroy: freeing aquired data block # 1
memory_pool_destroy: freeing aquired data block # 2
memory_pool_destroy: freeing aquired data block # 3
memory_pool_destroy: freeing aquired data block # 4
main: destroy returned: TRUE
```

### Test 3

Test 3 is the most advanced. A pool with between 1 and 10 blocks of size 4 are created. All the blocks are aquired and then read correctly as 0. The magic value `0xBAADA555` is written to them, and they are released back to the pool. Then, all the blocks are aquired again and re-read. Then the pool is destroyed. The destroy function correctly identifies all the blocks as aquired.


```bash
	****************** TEST 3 *****************
	* 1. create random size memory pool
	* 2. acquire all data blocks
	* 3. fill all data blocks using magic value
	* 4. release all data blocks back into pool
	* 5. dump all data before destruct
	*******************************************


main: pool_1=0x560c01d872d0, count=6, block_size=4
MEMORY_POOL: i=0, data=0x560c03092760, header=0x560c03092764, block_size=4, next=(nil)
MEMORY_POOL: i=1, data=0x560c030927f0, header=0x560c030927f4, block_size=4, next=0x560c03092764
MEMORY_POOL: i=2, data=0x560c03092820, header=0x560c03092824, block_size=4, next=0x560c030927f4
MEMORY_POOL: i=3, data=0x560c03092850, header=0x560c03092854, block_size=4, next=0x560c03092824
MEMORY_POOL: i=4, data=0x560c03092880, header=0x560c03092884, block_size=4, next=0x560c03092854
MEMORY_POOL: i=5, data=0x560c030928b0, header=0x560c030928b4, block_size=4, next=0x560c03092884
memory_pool_init: mp=0x560c03092670, count=6, block_size=4
memory_pool_acquire: mp=0x560c03092670, data=0x560c030928b0
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092880
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092850
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092820
memory_pool_acquire: mp=0x560c03092670, data=0x560c030927f0
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092760
main: reading: magic_array[0]=0x560c030928b0, data=0
main: reading: magic_array[1]=0x560c03092880, data=0
main: reading: magic_array[2]=0x560c03092850, data=0
main: reading: magic_array[3]=0x560c03092820, data=0
main: reading: magic_array[4]=0x560c030927f0, data=0
main: reading: magic_array[5]=0x560c03092760, data=0
main: writing: magic_array[0]=0x560c030928b0, data=BAADA555
main: writing: magic_array[1]=0x560c03092880, data=BAADA555
main: writing: magic_array[2]=0x560c03092850, data=BAADA555
main: writing: magic_array[3]=0x560c03092820, data=BAADA555
main: writing: magic_array[4]=0x560c030927f0, data=BAADA555
main: writing: magic_array[5]=0x560c03092760, data=BAADA555
memory_pool_release: data=0x560c030928b0, header=0x560c030928b4, block_size=4, next=(nil)
memory_pool_release: data=0x560c03092880, header=0x560c03092884, block_size=4, next=(nil)
memory_pool_release: data=0x560c03092850, header=0x560c03092854, block_size=4, next=(nil)
memory_pool_release: data=0x560c03092820, header=0x560c03092824, block_size=4, next=(nil)
memory_pool_release: data=0x560c030927f0, header=0x560c030927f4, block_size=4, next=(nil)
memory_pool_release: data=0x560c03092760, header=0x560c03092764, block_size=4, next=(nil)
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092760
memory_pool_acquire: mp=0x560c03092670, data=0x560c030927f0
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092820
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092850
memory_pool_acquire: mp=0x560c03092670, data=0x560c03092880
memory_pool_acquire: mp=0x560c03092670, data=0x560c030928b0
main: reading: magic_array[0]=0x560c03092760, data=BAADA555
main: reading: magic_array[1]=0x560c030927f0, data=BAADA555
main: reading: magic_array[2]=0x560c03092820, data=BAADA555
main: reading: magic_array[3]=0x560c03092850, data=BAADA555
main: reading: magic_array[4]=0x560c03092880, data=BAADA555
main: reading: magic_array[5]=0x560c030928b0, data=BAADA555
memory_pool_dump: mp = 0x560c03092670, count=6, available=0, block_size=4
memory_pool_destroy: mp=0x560c03092670, pool=(nil), count=6, available=0, block_size=4
memory_pool_destroy: freeing aquired data block # 1
memory_pool_destroy: freeing aquired data block # 2
memory_pool_destroy: freeing aquired data block # 3
memory_pool_destroy: freeing aquired data block # 4
memory_pool_destroy: freeing aquired data block # 5
memory_pool_destroy: freeing aquired data block # 6
main: destroy returned: TRUE
```

Requirements
---

> 6)  Memory Pool
> See attached architecture and files

The prompt c and h files are available here: [/prompt/memory_pool.c](../prompt/memory_pool.c) and here: [/prompt/memory_pool.h](../prompt/memory_pool.h)

Prelims
---

### Environment

The general environment remains the same as the previous steps.

#### CMAKE

I set up my CMAKE environment based on the prompt CMakeLists_1.txt available here: [/prompt/CMakeLists_1.txt](../prompt/CMakeLists_1.txt). My final CMakeLists.txt file has some changes. Commentary is available in the [Reference in Appendix](Step_6_Appendix.md#cmake).

### Doxygen

Considerations for creating documentation remain the same as for Step 3: [Reference in Step 3 Appendix](Step_3_Appendix.md#doxygen-creation)
