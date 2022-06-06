#ifndef VECTOR_H

#define VECTOR_H

#include <stddef.h>
#include <Block.h>

#define DECL_VECTOR(type) \
	typedef struct { \
		size_t (^size)(void); \
		type * (^get)(size_t index); \
		void (^push)(type item); \
		type * (^pop)(); \
		void (^insert)(size_t index, type value); \
		void (^remove)(size_t index); \
		void (^free)(void); \
	} vector_ ## type; \
	vector_ ## type vector_ ## type ## _new(size_t capacity_increase);

DECL_VECTOR(int)
DECL_VECTOR(double)
DECL_VECTOR(vector_double)

#endif
