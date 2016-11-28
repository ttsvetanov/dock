#include <cstdio>
#include <cstdint>

#include "assert.hpp"

int32_t main() {
	try {
		foreman::Assert::test([]() -> bool { return false; });
	} catch (foreman::AssertException& e) {
		printf("Assert failed!\n");
	}
	
	fflush(stdin);
	getchar();

	return 0;
}