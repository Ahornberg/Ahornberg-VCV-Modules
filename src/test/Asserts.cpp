#ifndef NDEBUG
#  include "../Ahornberg.hpp"

bool assertEquals(int value1, int value2, const char* message) {
	if (value1 != value2) {
		DEBUG("%s", message);
		DEBUG("value1: %d", value1);
		DEBUG("value2: %d", value2);
		return false;
	}
	return true;
}

#endif

// assert(assertEquals(55, 54, "not equals"));

// assert(testme());

// #ifndef NDEBUG
	// bool testme() {
		// return assertEquals(55, 54, "not equals");
	// }
// #endif




// #include <chrono> 

// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// for (auto m = 0; m < 10000; ++m) {
	// // do task here
// }
// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// auto duration = duration_cast<microseconds>(stop - start);
// DEBUG("time used: %d microseconds", std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
// DEBUG("time used: %d nanoseconds", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
