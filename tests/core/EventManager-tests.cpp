#include <catch.hpp>
#include <core/EventManager.hpp>

using namespace eto::core;

struct TestEvent1 
{
	bool boolean;
	float floatt;
};
struct TestEvent2 {
	TestEvent2(TestEvent1 &l, const TestEvent1 &r, int d)
		: left(l), right(r), distance(d)
	{
	}
	TestEvent1 left;
	TestEvent1 right;
	int distance;
};

int TEST_EVENT_VAR { 0};

void test_callback1(const TestEvent1 &e) {
	if (e.boolean) 
		++TEST_EVENT_VAR;
}

class TestClassEvent
{
public:
	static void test_callback2(const TestEvent2 &e)
	{
		if (e.distance < 0)
			--TEST_EVENT_VAR;
	}
};

TEST_CASE("Event manager", "[EventManager]")
{
	TEST_EVENT_VAR = 0;
	EventManager em;
	TestEvent1 e11 = { 1, 1.0 };
	TestEvent1 e12 = { 1, 1.0 };
	TestEvent2 e21 = { e11, e12, -1 };
	em.notify(e11);
	REQUIRE( TEST_EVENT_VAR == 0 );

	SECTION("Automatic unsubscription")
	{
		{
			auto sub = em.subscribe(std::function<void(const TestEvent1&)>(test_callback1));
			em.notify(e11);
			REQUIRE( TEST_EVENT_VAR == 1 );
		}
		em.notify(e12);
		REQUIRE( TEST_EVENT_VAR == 1 );

	}
	SECTION("Manual unsubscriprion")
	{
		auto sub = em.subscribe<TestEvent1>(test_callback1);
		em.notify(TestEvent1{1, 1.0});
		REQUIRE( TEST_EVENT_VAR == 1 );
		sub.unsubscribe();
		em.notify(e12);
		REQUIRE( TEST_EVENT_VAR == 1 );
	}
	SECTION("Class method callback")
	{
		auto sub = em.subscribe<TestEvent2>(&TestClassEvent::test_callback2);
		em.notify(e21);
		REQUIRE( TEST_EVENT_VAR == -1 );
	}
	SECTION("Lambda callback")
	{
		auto sub = em.subscribe<TestEvent2>([](const TestEvent2 &e) {
						if (e.left.floatt == e.right.floatt)
							++TEST_EVENT_VAR;
					});
		for (int i = 0; i < 50; ++i)
			em.notify(e21);
		REQUIRE( TEST_EVENT_VAR == 50 );
	}
}

