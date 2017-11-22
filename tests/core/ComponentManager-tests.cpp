#include <catch/catch.hpp>
#include <core/ComponentManager.hpp>
#include <vector>

using namespace eto;

TEST_CASE("Component manager with basic type", "[ComponentManager]")
{
	ComponentManager<int> m;
	for (int i = 0; i < 100; ++i)
		m.add(i, i);

	for (int i = 0; i < 100; ++i)
		REQUIRE( m.get(i) == i );

	for (int i = 0; i < 40; ++i)
		m.remove(i);

	for (int i = 20; i < 40; ++i)
		m.add(i - 20, i);

	for (int i = 0; i < 20; ++i)
		REQUIRE( m.get(i) == i + 20 );
}

struct TestPOD {
	int  integer;
	char character;
	std::vector<float> vfloats;
};

bool operator==(const TestPOD &lhs, const TestPOD &rhs){
	return ( lhs.integer == rhs.integer && 
		 lhs.character == rhs.character &&
		 lhs.vfloats == rhs.vfloats );
}

TEST_CASE("Componen manager with POD type", "[ComponentManager]")
{
	ComponentManager<TestPOD> m(100);
	TestPOD sample1 = { ~1, '&', { 0.11, 1, -13.3, 5.666 } };
	TestPOD sample2 = { 444, 'w', { } };
	
	for (int i = 0; i < 100; ++i)
		m.add(i, ~1, '&', std::initializer_list<float>{ 0.11, 1, -13.3, 5.666 } );

	for (int i = 0; i < 100; ++i)
		REQUIRE( m.get(i) == sample1 );

	for (int i = 0; i < 40; ++i)
		m.remove(i);
	for (int i = 20; i < 40; ++i)
		m.add(i - 20, 444, 'w' );

	for (int i = 0; i < 20; ++i)
		REQUIRE( m.get(i) == sample2 ); 
}

class TestClassCompManager
{
public:
	TestClassCompManager(float val = 0.0)
		: num(val)
	{ }
	float num;
};

TEST_CASE("Component manager with contructors", "[ComponentManager]")
{
 	ComponentManager<TestClassCompManager> m(100);
	float sample1 = 0.000000013;
	float sample2 = -3;

	for (int i = 0; i < 100; ++i)
		m.add(i, sample1);

	for (int i = 0; i < 100; ++i)
		REQUIRE( m.get(i).num == sample1 );

	for (int i = 0; i < 40; ++i)
		m.remove(i);

	for (int i = 20; i < 40; ++i)
		m.add(i - 20, sample2);

	for (int i = 0; i < 20; ++i)
		REQUIRE( m.get(i).num == sample2 );

}
