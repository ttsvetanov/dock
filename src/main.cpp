#include <cstdio>
#include <cstdint>
#include <windows.h>

#include "assert.hpp"

int32_t main() {
	try {
		foreman::Assert::isEquals<const char*>(u8"some", u8"some");
		printf(u8"\tTest passed!\n");
	} catch (foreman::AssertException& e) {
		printf(u8"\tMessage: %s\n", e.what());
	}
	
	fflush(stdin);
	getchar();

	return 0;
}