#include <tests.h>

#define TEST(func) \
	printf("test_%s: %s\n", #func, test_ ## func() ? "ok" : "fail")

#define TEST_ARGS(func, ...) \
	printf("test_%s: %s\n", #func, test_ ## func(__VA_ARGS__) ? "ok" : "fail")

// VECTOR

IMPL_VECTOR(int)
IMPL_VECTOR(double)
IMPL_VECTOR(vector_double)

#define VECTOR_TEST(type) \
	bool test_vector_ ## type(type def) { \
		DEF_VECTOR(type) vector = vector_ ## type ## _new(0); \
		type *get1, *get2; \
		size_t i, j; \
		for (i = 1; i < 1000; ++i) { \
			vector.push(def); \
			for (j = i - 1; j > 0; --j) { \
				get1 = vector.get(j); \
				get2 = vector.get(j+1); \
				if (get1 != NULL && get2 != NULL) \
					*get1 = *get2; \
				vector.insert(j, def); \
			} \
		} \
		for (i = 1; i < vector.size(); ++i) { \
			vector.pop(); \
			for (j = i - 1; j > 0; --j) { \
				get1 = vector.get(j); \
				get2 = vector.get(j+1); \
				if (get1 != NULL && get2 != NULL) \
					*get2 = *get1; \
				if (j % 50 == 0) \
					vector.remove(j); \
			} \
		} \
		return true; \
	}

VECTOR_TEST(int)
VECTOR_TEST(double)
VECTOR_TEST(vector_double)

void run_vector_tests() {
	TEST_ARGS(vector_int, 1122334455);
	TEST_ARGS(vector_double, 554433.221101);
	DEF_VECTOR(double) test_vector = vector_double_new(0);
	TEST_ARGS(vector_vector_double, test_vector);
}

// ITERATE

bool test_foreach_vector() {
	DEF_VECTOR(int) vector = vector_int_new(0);
	for (int i = 0; i < 1000; ++i)
		vector.push(i);
	int i = 0;
	foreach (item, vector, int)
		if (*item != i++)
			return false;
	return i == (int) vector.size();
}

bool test_foreach_reversed_vector() {
	DEF_VECTOR(int) vector = vector_int_new(0);
	for (int i = 1000; i > 0; --i)
		vector.push(i);
	int i = 0;
	foreach_reversed (item, vector, int)
		if (*item != ++i)
			return false;
	return i == (int) vector.size();
}

void run_iterate_tests() {
	TEST(foreach_vector);
	TEST(foreach_reversed_vector);
}

// MUTEX

IMPL_MUTEX(int)
IMPL_MUTEX(double)

#define MUTEX_TEST(type) \
	bool test_mutex_ ## type(type value, type new) { \
		DEF_MUTEX(type) mutex = mutex_ ## type ## _new(value); \
		{ \
			DEF_MUTEX_GUARD(type) guard = mutex.lock(); \
			if (*guard != value) \
				return false; \
			*guard = new; \
		} \
		{ \
			type *guard = mutex.lock(); \
			if (*guard != new) \
				return false; \
			*guard = value; \
			mutex.unlock(); \
		} \
		DEF_MUTEX_GUARD(type) guard = mutex.lock(); \
		return *guard == value; \
	}

MUTEX_TEST(int)
MUTEX_TEST(double)

void run_mutex_tests() {
	TEST_ARGS(mutex_int, 1234567, 123);
	TEST_ARGS(mutex_double, 76543.212, 27.25);
}

