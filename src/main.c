#include <vector.h>

#include <stdbool.h>
#include <stdio.h>

#define VECTOR_TEST(type) \
	bool test_vector_ ## type(type def) { \
		vector_ ## type vector = vector_ ## type ## _new(0); \
		type *get1, *get2; \
		size_t i, j; \
		for (i = 1; i < 10001; ++i) { \
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
		vector.free(); \
		return true; \
	}

VECTOR_TEST(int)
VECTOR_TEST(double)
VECTOR_TEST(vector_double)

int main() {
	printf("test_vector_int: %s\n",
		test_vector_int(1122334455) ? "ok" : "fail");
	printf("test_vector_double: %s\n",
		test_vector_double(554433.221101) ? "ok" : "fail");
	vector_double test_vector = vector_double_new(0);
	printf("test_vector_vector_double: %s\n",
		test_vector_vector_double(test_vector) ? "ok" : "fail");
	test_vector.free();
	return 0;
}
