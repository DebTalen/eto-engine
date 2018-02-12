#ifndef ETO_REFLECTION_HPP
#define ETO_REFLECTION_HPP
/*
 * This file provides the basic reflection for class members. It supports access via getter/setter.
 * See Reflection-tests.cpp for usage reference.
 *
 * This code is highly inspired by Elias Daler's MetaStuff (https://github.com/eliasdaler).
 * Unfortunately, at the time I wrote this, MetaStuff did not support template classes, and some other things that I needed.
 */
#include <tuple>
#include <string>
#include <internal/Assert.hpp>

using std::string;

namespace eto {
/*  
 * You need to specialize this structure for your type
 * so that register_members() retruns all needed members
*/
template <typename C>
struct MetaInfo {
	static auto register_members() {
		return std::tuple<>();
	}
};

namespace internal {
namespace reflect {

template <typename C, typename T>
using member_ptr = T C::*;

template <typename C, typename T>
using cref_get =  const T& (C::*)() const;

template <typename C, typename T>
using val_get =  T (C::*)() const;

template <typename C, typename T>
using ref_get = T& (C::*)() ;

template <typename C, typename T>
using cref_set =  void (C::*)(const T&);

// It's kinda pointless and troublesome
//template <typename C, typename T>
//using ref_set =  void (C::*)(T&);

template <typename C, typename T>
using val_set =  void (C::*)(T);

// Use it like so: get_member_type<decltype(member)>
template <typename C>
using get_member_type = typename std::decay_t<C>::member_type;

/** A member of class C of type T */
template <typename C, typename T>
class Member {
public:
	using member_type = T;
	Member(const Member&) = default;
	Member& operator=(const Member&) = default;
	
	/**
	 *  @brief  Retrieves a member from an object
	 *
	 *  @return Either a const reference to member or a member copy 
	 */
	auto get(const C &obj) const;

	/**
	 *  @brief  Retrieves a non-const reference to a member
	 *
	 *  This function should be called only when can_get_ref() == true
	 *  @return A non-const reference to a member
	 */
	T& get_ref(C &obj) const;

	/** Sets the value for the object's member */
	void set(C &obj, const T &val) const;

	/** Returns the member name */
	string get_name() const { return m_name; }

	/** Indicates whether it is possible to get a non-const reference to the member	 */
	bool can_get_ref() const { return (m_ptr || m_ref_get); }
private:
	string m_name;

	member_ptr<C,T> m_ptr = nullptr;
	
	cref_get<C,T> m_cref_get = nullptr;
	cref_set<C,T> m_cref_set = nullptr;

	ref_get<C,T> m_ref_get = nullptr;

	val_get<C,T> m_val_get = nullptr;
	val_set<C,T> m_val_set = nullptr;

	/** Create Members only via member<C,T> function*/
	Member(const string &name);

	template <typename U, typename V> friend Member<U,V> member(const string&, member_ptr<U,V>);
	template <typename U, typename V> friend Member<U,V> member(const string&, cref_get<U,V>, cref_set<U,V>);
	template <typename U, typename V> friend Member<U,V> member(const string&, cref_get<U,V>, val_set<U,V>);
	template <typename U, typename V> friend Member<U,V> member(const string&, ref_get<U,V>, cref_set<U,V>);
	template <typename U, typename V> friend Member<U,V> member(const string&, ref_get<U,V>, val_set<U,V>);
	template <typename U, typename V> friend Member<U,V> member(const string&, val_get<U,V>, cref_set<U,V>);
	template <typename U, typename V> friend Member<U,V> member(const string&, val_get<U,V>, val_set<U,V>);
};

/** Function to retrieve members of type C */
template <typename C>
inline auto get_members() {
	static auto members = MetaInfo<C>::register_members();
	return members;
}

/** Indicates that the type was registered or not */
template <typename C>
constexpr bool is_registered() {
	return not std::is_same<std::tuple<>, decltype(get_members<C>())>::value;
}

template <typename ...Args>
constexpr auto members(Args&& ...args)
{
	return std::make_tuple(std::forward<Args>(args)...);
}

template <typename C, typename T>
Member<C,T> member(const string &name, member_ptr<C,T> ptr) {
	Member<C,T> m(name);
	m.m_ptr = ptr;
	return m;
}

template <typename C, typename T>
Member<C,T> member(const string &name, cref_get<C,T> getter, cref_set<C,T> setter) {
	Member<C,T> m(name);
	m.m_cref_get = getter;
	m.m_cref_set = setter;
	return m;
}

template <typename C, typename T>
Member<C,T> member(const string &name, cref_get<C,T> getter, val_set<C,T> setter) {
	Member<C,T> m(name);
	m.m_cref_get = getter;
	m.m_val_set = setter;
	return m;
}

template <typename C, typename T>
Member<C,T> member(const string &name, ref_get<C,T> getter, cref_set<C,T> setter) {
	Member<C,T> m(name);
	m.m_ref_get = getter;
	m.m_cref_set = setter;
	return m;
}

template <typename C, typename T>
Member<C,T> member(const string &name, ref_get<C,T> getter, val_set<C,T> setter) {
	Member<C,T> m(name);
	m.m_ref_get = getter;
	m.m_val_set = setter;
	return m;
}

template <typename C, typename T>
Member<C,T> member(const string &name, val_get<C,T> getter, cref_set<C,T> setter) {
	Member<C,T> m(name);
	m.m_val_get = getter;
	m.m_cref_set = setter; return m;
}

template <typename C, typename T>
Member<C,T> member(const string &name, val_get<C,T> getter, val_set<C,T> setter) {
	Member<C,T> m(name);
	m.m_val_get = getter;
	m.m_val_set = setter;
	return m;
}

// Helper functions
template<size_t... Is, typename ...Ts, typename F>
void for_each_in_tuple(const std::tuple<Ts...> & t, const F &f, std::index_sequence<Is...>){
	using expander = int[];
	(void)expander { 0, ((void)f(std::get<Is>(t)), 0)... };
}

template<typename ...Ts, typename F>
void for_each_in_tuple(const std::tuple<Ts...> & t, const F &f){
	for_each_in_tuple(t, f, std::make_index_sequence<sizeof...(Ts)>());
}

template <typename C, typename F>
void for_each_member(C &obj, const F &f)
{
	for_each_in_tuple(get_members<C>(), 
		[&] (const auto& m) {
			f(m.get(obj));
	});
}


// Implementaion 
template <typename C, typename T>
Member<C,T>::Member(const string &s) 
	: m_name(s)
{
}

template <typename C, typename T>
auto Member<C,T>::get(const C &obj) const 
{
	if (m_cref_get)
		return (obj.*m_cref_get)();
	if (m_val_get) 
		return (obj.*m_val_get)();
	if (m_ptr)
		return static_cast<const T&>(obj.*m_ptr);
	ETO_ASSERT (m_ref_get);
	// meh...
	return (obj.*(reinterpret_cast<const T& (C::*)() const>(m_ref_get)))();
}

template <typename C, typename T>
T& Member<C,T>::get_ref(C &obj) const
{
	ETO_ASSERT (this->can_get_ref());
	if (m_ref_get)
		return (obj.*m_ref_get)();
	//else 
	return obj.*m_ptr;
}

template <typename C, typename T>
void Member<C,T>::set(C &obj, const T &val) const
{
	if(m_cref_set)
		(obj.*m_cref_set)(val);
	else if(m_val_set)
		(obj.*m_val_set)(val);
	else 
		this->get_ref(obj) = val;
}

} // namespace reflect 

} // namespace internal
} // namespace eto 

#endif 
