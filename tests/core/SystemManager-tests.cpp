#include <catch.hpp>
#include <core/System.hpp>
#include <core/SystemManager.hpp>
#include <core/EntityManager.hpp>

using namespace eto::core;


struct TestSystem1 : public System 
{
	TestSystem1(int &num) 
		: i(num) 
	{ } 
	void update(EntityManager &em) override { 
		--i;
	}
	int &i;
};

TEST_CASE("Systems added", "[SystemManager]")
{
	int i = 100;
	EntityManager em;
	SystemManager sm(em);
	sm.add<TestSystem1>(i);

	REQUIRE(i == 100);

	for (int i = 0; i < 50; ++i)
		sm.update_all();
	REQUIRE(i == 50);

	sm.remove<TestSystem1>();

	for (int i = 0; i < 50; ++i)
		sm.update_all();
	REQUIRE(i == 50);

	sm.add<TestSystem1>(i);
	for (int i = 0; i < 50; ++i)
		sm.update_all();
	REQUIRE(i == 0);
}


