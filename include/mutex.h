#ifndef _MUTEX_H

#define _MUTEX_H

#include <Block.h>
#include <stdbool.h>
#include <limits.h>

#define DEF_MUTEX(type) \
	__attribute__((cleanup(_mutex_ ## type ## _free))) mutex_ ## type 

#define DEF_MUTEX_GUARD(type) \
	__attribute__((cleanup(_mutex_guard_ ## type ## _unlock))) type *

#define DECL_MUTEX(type) \
	typedef struct { \
		type *(^lock)(); \
		void (^unlock)(); \
		void (^free)(); \
	} mutex_ ## type; \
	mutex_ ## type mutex_ ## type ## _new(type value); \
	void _mutex_ ## type ## _free(mutex_ ## type *self); \
	void _mutex_guard_ ## type ## _unlock(type **value);

#define IMPL_MUTEX(type) \
	mutex_ ## type mutex_ ## type ## _new(type value) { \
		struct _mutex_ ## type ## _private { \
			volatile bool lock: 1; \
			type value; \
		}; \
		__block struct _mutex_ ## type ## _private *private = \
			malloc(sizeof(struct _mutex_ ## type ## _private)); \
		private->lock = 0; \
		private->value = value; \
		__block mutex_ ## type public = { \
			.lock = Block_copy(^ { \
				while (private->lock); \
				private->lock = true; \
				return &private->value; \
			}), \
			.unlock = Block_copy(^ { \
				private->lock = false; \
			}), \
			.free = NULL \
		}; \
		public.free = Block_copy(^ { \
			free(private); \
			Block_release(public.lock); \
			Block_release(public.unlock); \
			Block_release(public.free); \
		}); \
		return public; \
	} \
	void _mutex_ ## type ## _free(mutex_ ## type *self) { \
		self->free(); \
	} \
	void _mutex_guard_ ## type ## _unlock(type **value) { \
		unsigned char *char_before = (unsigned char *) ((*value) - 1); \
		*char_before &= ((unsigned char) ((1 << CHAR_BIT) - 2)); \
	}

#endif
