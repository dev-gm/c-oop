#include <tests.h>

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
	printf("test_vector_int: %s\n",
		test_vector_int(1122334455) ? "ok" : "fail");
	printf("test_vector_double: %s\n",
		test_vector_double(554433.221101) ? "ok" : "fail");
	DEF_VECTOR(double) test_vector = vector_double_new(0);
	printf("test_vector_vector_double: %s\n",
		test_vector_vector_double(test_vector) ? "ok" : "fail");
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
	printf("test_foreach_vector: %s\n",
		test_foreach_vector() ? "ok": "fail");
	printf("test_foreach_reversed_vector: %s\n",
		test_foreach_reversed_vector() ? "ok": "fail");
}
