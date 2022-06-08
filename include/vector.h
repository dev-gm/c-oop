#ifndef _VECTOR_H

#define _VECTOR_H

#include <Block.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DEF_VECTOR(type) \
	__attribute__((cleanup(_vector_ ## type ## _free))) vector_ ## type

#define DECL_VECTOR(type) \
	typedef struct { \
		size_t (^size)(void); \
		type * (^get)(size_t index); \
		void (^push)(type item); \
		type * (^pop)(void); \
		void (^insert)(size_t index, type value); \
		void (^remove)(size_t index); \
		type *(^begin)(void); \
		type *(^end)(void); \
		void (^free)(void); \
	} vector_ ## type; \
	vector_ ## type vector_ ## type ## _new(size_t capacity_increase); \
	void _vector_ ## type ## _free(vector_ ## type *self);

#define IMPL_VECTOR(type) \
	vector_ ## type vector_ ## type ## _new(size_t capacity_increase) { \
		struct _vector_ ## type ## _private { \
			size_t size, capacity, capacity_increase; \
			type *items; \
		}; \
		struct _vector_ ## type ## _private * __block private = \
			malloc(sizeof(struct _vector_ ## type ## _private)); \
		private->size = 0; \
		private->capacity_increase = capacity_increase > 0 \
			? capacity_increase \
			: 4; \
		private->capacity = private->capacity_increase; \
		private->items = malloc(sizeof(type) * private->capacity); \
		__block vector_ ## type public = { \
			.size = Block_copy(^ size_t (void) { \
				return private->size; \
			}), \
			.get = Block_copy(^ type * (size_t index) { \
				return index < private->size \
					? private->items + index \
					: NULL; \
			}), \
			.push = Block_copy(^ (type item) { \
				if (private->size >= private->capacity) { \
					private->capacity += private->capacity_increase; \
					private->items = realloc(private->items, private->capacity * sizeof(type)); \
				} \
				private->items[private->size] = item; \
				++private->size; \
			}), \
			.pop = Block_copy(^ type * (void) { \
				return private->size > 0 \
					? &private->items[(private->size--) - 1] \
					: NULL; \
			}), \
			.insert = Block_copy(^ (size_t index, type value) { \
				if (index < private->size) \
					private->items[index] = value; \
			}), \
			.remove = Block_copy(^ (size_t index) { \
				if (index < private->size - 1) \
					memmove( \
						&private->items[index], \
						&private->items[index+1], \
						(private->size - index - 1) * sizeof(type) \
					); \
				--private->size; \
			}), \
			.begin = Block_copy(^ type * (void) { \
				return private->size > 0 \
					? &private->items[0] \
					: NULL; \
			}), \
			.end = Block_copy(^ type * (void) { \
				 return private->size > 0 \
					? &private->items[private->size-1] \
					: NULL; \
			}), \
			.free = NULL \
		}; \
		public.free = Block_copy(^ { \
			free(private->items); \
			free(private); \
			Block_release(public.size); \
			Block_release(public.get); \
			Block_release(public.push); \
			Block_release(public.pop); \
			Block_release(public.insert); \
			Block_release(public.remove); \
			Block_release(public.free); \
		}); \
		return public; \
	} \
	void _vector_ ## type ## _free(vector_ ## type *self) { \
		self->free(); \
	}

#endif
