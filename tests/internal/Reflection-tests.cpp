#include <catch.hpp>
#include <string>
#include <tuple>
#include <internal/Reflection.hpp>

using std::string;
using namespace eto::internal::reflect;

template <typename T>
struct Dummy {
	T val;
};

struct ReflStruct
{
public:
	const int8_t &cref_get1() const { return cref_get_cref_set; }
	const float &cref_get2() const { return cref_get_val_set; }

	int val_get1() const { return val_get_cref_set; }
	unsigned val_get2() const { return val_get_val_set; }

	double &ref_get1() { return ref_get_cref_set; }
	long &ref_get2() { return ref_get_val_set; }

	void cref_set1(const int8_t &cref) { cref_get_cref_set = cref; }
	void cref_set2(const int &cref) { val_get_cref_set = cref; }
	void cref_set3(const double &cref) { ref_get_cref_set = cref; }

	void val_set1(float val) { cref_get_val_set = val; }
	void val_set2(unsigned val) { val_get_val_set = val; }
	void val_set3(long val) { ref_get_val_set = val; }

	int direct_ptr = 1;
private:
	int8_t    cref_get_cref_set = 1;
	float     cref_get_val_set = 1.0f;
	int       val_get_cref_set = 1;
	unsigned  val_get_val_set = 1;
	double    ref_get_cref_set = 1.0;
	long      ref_get_val_set = 1;
};

namespace eto {

template <>
inline auto MetaInfo<ReflStruct>::register_members() {
	return members(
		member("direct_ptr", &ReflStruct::direct_ptr),
		member("cref_get_cref_set", &ReflStruct::cref_get1, &ReflStruct::cref_set1),
		member("cref_get_val_set",  &ReflStruct::cref_get2, &ReflStruct::val_set1),
		member("val_get_cref_set",  &ReflStruct::val_get1, &ReflStruct::cref_set2),
		member("val_get_val_set",   &ReflStruct::val_get2, &ReflStruct::val_set2),
		member("ref_get_cref_set",  &ReflStruct::ref_get1, &ReflStruct::cref_set3),
		member("ref_get_val_set",   &ReflStruct::ref_get2, &ReflStruct::val_set3) 
	);
}

template <typename T>
struct MetaInfo<Dummy<T>> {
	static auto register_members() {
		return members ( member("val", &Dummy<T>::val) );
	}
};

}


TEST_CASE("Reflecting all possible getter/setter combination", "[Reflection]")
{
	ReflStruct r;
	REQUIRE (is_registered<ReflStruct>() == true);
	REQUIRE (is_registered<Dummy<int>>() == true);
	REQUIRE (is_registered<Dummy<ReflStruct>>() == true);
	REQUIRE (is_registered<Dummy<Dummy<int>>>() == true);

	auto m = get_members<ReflStruct>();

	REQUIRE( std::tuple_size<decltype(m)>::value == 7 );
	REQUIRE( std::get<0>(m).get_name() == "direct_ptr" );
 	REQUIRE( std::get<1>(m).get_name() == "cref_get_cref_set" );
	REQUIRE( std::get<2>(m).get_name() == "cref_get_val_set" );
	REQUIRE( std::get<3>(m).get_name() == "val_get_cref_set" );
	REQUIRE( std::get<4>(m).get_name() == "val_get_val_set" );
	REQUIRE( std::get<5>(m).get_name() == "ref_get_cref_set" );
	REQUIRE( std::get<6>(m).get_name() == "ref_get_val_set" );

	REQUIRE( std::get<0>(m).can_get_ref() == true );
	REQUIRE( std::get<1>(m).can_get_ref() == false );
	REQUIRE( std::get<2>(m).can_get_ref() == false );
	REQUIRE( std::get<3>(m).can_get_ref() == false );
	REQUIRE( std::get<4>(m).can_get_ref() == false );
	REQUIRE( std::get<5>(m).can_get_ref() == true );
	REQUIRE( std::get<6>(m).can_get_ref() == true );

	auto check_getters = [&m, &r] (int num) {
 		for_each_member(r, [&num] (const auto &member) {
			 REQUIRE( member == static_cast<decltype(member)>(num) );
		}); 
		REQUIRE( std::get<0>(m).get(r) == r.direct_ptr );
		REQUIRE( std::get<1>(m).get(r) == r.cref_get1() );
		REQUIRE( std::get<2>(m).get(r) == r.cref_get2() );
		REQUIRE( std::get<3>(m).get(r) == r.val_get1() );
		REQUIRE( std::get<4>(m).get(r) == r.val_get2() );
		REQUIRE( std::get<5>(m).get(r) == r.ref_get1() );
		REQUIRE( std::get<6>(m).get(r) == r.ref_get2() );
 	};

	check_getters(1);

 	for_each_in_tuple(m, [&r] (const auto &member) {
		member.set(r, static_cast<get_member_type<decltype(member)>>(10) );
	});

	check_getters(10);
}


