#ifndef _TESTS_H

#define _TESTS_H

#include <stdio.h>
#include <stdbool.h>

#include <vector.h>

DECL_VECTOR(int)
DECL_VECTOR(double)
DECL_VECTOR(vector_double)

void run_vector_tests();

#include <iterate.h>

void run_iterate_tests();

#include <mutex.h>

DECL_MUTEX(int)
DECL_MUTEX(double)

void run_mutex_tests();

inline void run_tests() {
	run_vector_tests();
	run_iterate_tests();
	run_mutex_tests();
}

#endif
