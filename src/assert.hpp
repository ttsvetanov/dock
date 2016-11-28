#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <exception>

namespace foreman {
	class AssertException : public std::exception {
	public:
		AssertException() : exception() {}
	};

	class Assert {
	public:
		static void test(std::function<bool()> fcn);
	private:
		Assert() {}
	};

	void Assert::test(std::function<bool()> fcn) {
		if (!fcn()) {
			throw AssertException();
		}
	}
}