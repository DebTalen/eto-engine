#include <catch/catch.hpp>
#include <core/Entity.hpp>

using namespace eto;

namespace tests 
{
struct TestComponent: public Component
{
	virtual void onUpdate() override 
	{
		++i;
	}
	int i = 0;
};

}
using namespace tests;

TEST_CASE("Entity is created", "[Entity]")
{
	Entity e;
	REQUIRE( e.isActive() == true );
	REQUIRE( e.getComponent<TestComponent>() == nullptr );
	e.setActive(false);
	REQUIRE( e.isActive() == false );
}

TEST_CASE("Components are added", "[Entity]")
{
	Entity e;
	auto c = e.addComponent<TestComponent>();
	REQUIRE( e.getComponent<TestComponent>() == c );
	auto c2 = e.addComponent<TestComponent>();
	REQUIRE( e.getComponent<TestComponent>() == c2 );
}

TEST_CASE("Components are removed", "[Entity]")
{
	Entity e;
	auto c = e.addComponent<TestComponent>();
	e.removeComponent<TestComponent>();
	REQUIRE( e.getComponent<TestComponent>() == nullptr );
	e.removeComponent<TestComponent>();
	REQUIRE( e.getComponent<TestComponent>() == nullptr );
}

TEST_CASE("Entity is updated", "[Entity]")
{
	Entity e;
	auto c = e.addComponent<TestComponent>();
	REQUIRE( c->i == 0 );
	e.update();
	REQUIRE( c->i == 1 );
	int i = 1000;
	while (i--) e.update();
	REQUIRE( c->i == 1001 );
}
