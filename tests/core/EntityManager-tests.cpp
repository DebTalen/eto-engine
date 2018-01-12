#include <catch.hpp>
#include <core/EntityManager.hpp>
#include <core/Entity.hpp>
#include <string>
#include <algorithm>

using namespace eto;

TEST_CASE("Entities are created and destroyed", "[EntityManager]")
{
	EntityManager w;
	Entity e1 = w.create();
	REQUIRE ( e1.is_valid() );

	Entity e2 = w.create();
	REQUIRE( e1 != e2 );

	e2.destroy();
	REQUIRE ( e2.is_valid() == 0 );

	auto entities = w.create(100);
	REQUIRE( entities.size() == 100 );

	for (size_t i = entities.size() / 2; i < entities.size(); ++i)
	{
		entities[i].destroy();
		REQUIRE(entities[i].is_valid() == 0);
	}
	for (size_t i = 0; i < entities.size() / 2; ++i)
	{
		w.destroy(entities[i]);
		REQUIRE(entities[i].is_valid() == 0);
	}
}

struct TestPod{
	int i;
	float f;
	std::string s;
};

bool operator==(const TestPod &lhs, const TestPod &rhs) {
	return ( lhs.i == rhs.i &&
		 lhs.f == rhs.f &&
		 lhs.s == rhs.s );
}

class TestClass
{
public:
	TestClass(float val = 0.0)
		: num(val)
	{ }
	float num;
};

TEST_CASE("Components are added and removed", "[EntityManger]")
{
 	EntityManager w;
	Entity e1 = w.create();

	SECTION("Basic type")
	{
		char &c = e1.add<char>('a');
		REQUIRE( c == e1.get<char>() );
		REQUIRE( e1  == *(w.with<char>().begin()) );
		REQUIRE( e1.has<char>() == true );

		e1.remove<char>();
		REQUIRE( w.with<char>().count() == 0);
		REQUIRE( e1.has<char>() == false );
	}
	SECTION("Pod structure")
	{
		TestPod &pod = e1.add<TestPod>(10, 10.0f, "teststring");
		REQUIRE( pod == e1.get<TestPod>() );
		REQUIRE( e1  == *(w.with<TestPod>().begin()) );
		REQUIRE( e1.has<TestPod>() == true );

		e1.remove<TestPod>();
		REQUIRE( w.with<TestPod>().count() == 0);
		REQUIRE( e1.has<TestPod>() == false );
	}
	SECTION("Class with constructor")
	{
		TestClass &classs = e1.add<TestClass>(-0.0001f);
		REQUIRE( classs.num == e1.get<TestClass>().num );
		REQUIRE( e1  == *(w.with<TestClass>().begin()) );
		REQUIRE( e1.has<TestClass>() == true );

		e1.remove<TestClass>();
		REQUIRE( w.with<TestClass>().count() == 0);
		REQUIRE( e1.has<TestClass>() == false );
	}
}

TEST_CASE("Range test", "[EntityManager]")
{
	EntityManager w;
	auto v = w.create(50);
	for (auto &i : v)
		i.add<int>(0x33);
	auto vv = w.create(50);
	for (auto &i : vv)
	{
		i.add<int>(0x33);
		i.add<char>('a' + 1);
	}

	Range r = w.with<int>();
	REQUIRE( r.count() == 100 );
	std::for_each(r.begin(), r.end(),
		      [](const Entity &e) {
		      		REQUIRE (e.get<int>() == 0x33 );
		      });
	for (auto &i : vv)
		i.destroy();
	Range rr = w.with<char>();
	REQUIRE( rr.count() == 0 );
}
