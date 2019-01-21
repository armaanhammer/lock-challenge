
```bash
armaan@big-black-box:build$ ./test.out 


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


	****************** TEST 3 *****************
	* // 1. create random size memory pool
	* // 2. acquire all data blocks
	* // 3. fill all data blocks using magic value
	* // 4. release all data blocks back into pool
	* // 5. dump all data before destruct
	******************************************


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
