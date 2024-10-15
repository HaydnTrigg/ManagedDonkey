#include "memory/memory_pool.hpp"

struct s_memory_pool_block_header
{
	long __unknown0;
	long __unknown4;
	const char* __unknown8;
	long __unknownC;
};
static_assert(sizeof(s_memory_pool_block_header) == 0x10);

struct s_memory_pool_block
{
	dword size;
	byte __data4[0x4];
	long next_block_handle;
	long previous_block_handle;
};
static_assert(sizeof(s_memory_pool_block) == 0x10);

dword memory_pool_handle_from_address(s_memory_pool const* pool, void const* pointer)
{
	ASSERT(pool);
	ASSERT(pointer);

	return pointer_distance(pool, pointer);
}

long __cdecl sub_969DB0(long a1)
{
	return INVOKE(0x00969DB0, sub_969DB0, a1);
}

void* __cdecl memory_pool_get_address(s_memory_pool const* memory_pool, long payload_handle)
{
	ASSERT(memory_pool);

	return payload_handle ? (void*)offset_pointer(memory_pool, payload_handle) : NULL;
}

s_memory_pool_block* __cdecl memory_pool_get_block(s_memory_pool const* pool, long payload_handle)
{
	ASSERT(pool);

	return (s_memory_pool_block*)memory_pool_get_address(pool, payload_handle);
}

s_memory_pool_block* __cdecl memory_pool_block_get(s_memory_pool const* pool, long payload_handle)
{
	ASSERT(payload_handle);

	//memory_pool_verify(pool);
	s_memory_pool_block* block = memory_pool_get_block(pool, sub_969DB0(payload_handle));
	//ASSERT(memory_pool_block_valid(pool, block));

	//s_memory_pool_block* other_block = NULL;
	//for (other_block = memory_pool_get_block(pool, pool->first_block_handle);
	//	other_block && block != other_block;
	//	other_block = memory_pool_get_block(pool, other_block->next_block_handle))
	//{
	//	;
	//}
	//
	//ASSERT(other_block);

	return block;
}

void __cdecl memory_pool_block_free_handle(s_memory_pool* pool, dword payload_handle)
{
	//INVOKE(0x00969C80, memory_pool_block_free_handle, pool, payload_handle);

	s_memory_pool_block* block = memory_pool_block_get(pool, payload_handle);
	//memory_pool_verify(pool);
	//ASSERT(memory_pool_block_valid(pool, block));

	pool->free_size += block->size;
	//ASSERT(pool->free_size <= pool->size);

	if (block->previous_block_handle)
		memory_pool_get_block(pool, block->previous_block_handle)->next_block_handle = block->next_block_handle;
	else
		pool->first_block_handle = block->next_block_handle;

	if (block->next_block_handle)
		memory_pool_get_block(pool, block->next_block_handle)->previous_block_handle = block->previous_block_handle;
	else
		pool->last_block_handle = block->previous_block_handle;

	//csmemset(memory_pool_block_get_base(pool, block), 0xDD, block->size);
	//memory_pool_verify(pool);
}

void memory_pool_block_free(s_memory_pool* pool, void const** payload_data)
{
	//INVOKE(0x00969CD0, memory_pool_block_free, pool, payload_data);

	ASSERT(payload_data);

	memory_pool_block_free_handle(pool, memory_pool_handle_from_address(pool, *payload_data));
}

long __cdecl memory_pool_get_block_size(s_memory_pool* memory_pool, void const** payload_handle)
{
	return INVOKE(0x0096A4E0, memory_pool_get_block_size, memory_pool, payload_handle);
}
