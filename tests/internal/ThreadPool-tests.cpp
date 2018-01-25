#include <catch.hpp>
#include <cmath>
#include <internal/ThreadPool.hpp>

using namespace eto::internal;

void test_sleep(size_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); } 

// I dont rally know how to test it ...
TEST_CASE("ThreadPool tests", "[ThreadPool]")
{
	ThreadPool tp(3);
	auto power = tp.push([] (int i) { return 1 << i; }, 6 );
	tp.push(test_sleep, 1);
	REQUIRE(power.get() == 64);
	tp.wait_all();
	REQUIRE(tp.is_done());
}
