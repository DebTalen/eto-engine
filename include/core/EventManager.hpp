#ifndef ETO_EVENT_MANAGER_HPP
#define ETO_EVENT_MANAGER_HPP

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <internal/Assert.hpp>

using std::function;

namespace eto  {
namespace core {

namespace details {

size_t& get_next_event_id()  {
	static size_t next_event_id = 0;
	return next_event_id;
}

template <typename E>
size_t get_event_type_id() {
	static size_t event_id = get_next_event_id()++;
	return event_id;
}

}

struct Token;
struct SubscriptionsBase;
template <typename E> struct Subscription;
template <typename E> struct Subscriptions;

/** Provides the interface for subscribing to events and notifing of events */
class EventManager 
{
public:
	~EventManager();

	/**
	 *  @brief  Subscribes to events of type E
	 *
	 *  A callback function must take constant reference to 
	 *  the event of type E and have return type void.
	 *  @param  f A callback function. It will be called on each event of type E.
	 *  @return Subscription token that represents subscription. It automatically unsubscribes when destroyed.
	 */
	template <typename E>
	Token subscribe(function<void(const E&)> f);

	/**
	 *  @brief  Notifis all subscribtions to event of type E
	 *
	 *  @param  event The const reference to the event. It is passed to all subscribers.
	 */
	template <typename E>
	void notify(E const& event);

private:
	friend struct Token;

	template <typename E>
	Subscriptions<E> * get_subscriptions();

	std::unordered_map<size_t, SubscriptionsBase*> m_subs;
	std::mutex m_mutex;
};

/**  A subscription to events of type E*/
template <typename E>
struct Subscription 
{
	std::weak_ptr<size_t> subject; // Weak pointer to a subscription token
	function<void(const E&)> callback;
};

/** Helper base class. */
struct SubscriptionsBase 
{
	virtual ~SubscriptionsBase() {}
};

/** Subscriptions to events of type E */
template <typename E>
struct Subscriptions : public SubscriptionsBase
{
	std::vector<Subscription<E>> subs;
};

/**
 *  @brief  A subscription token
 *
 *  It automatically unsubscribes, when goes out of scope.
 */
struct Token 
{
	inline void unsubscribe() {
		object.reset();
	}
private:
	friend class EventManager;
	std::shared_ptr<size_t> object;
};


template <typename E>
inline Subscriptions<E>* EventManager::get_subscriptions() 
{
	size_t id = details::get_event_type_id<E>();
	return static_cast<Subscriptions<E>* >(m_subs[id]);
}

template <typename E>
Token EventManager::subscribe(function<void(const E&)> f)
{
	size_t event_id = details::get_event_type_id<E>();
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		auto it = m_subs.find(event_id);
		if (it == m_subs.end())
			m_subs[event_id] = new Subscriptions<E>();
	}

	Subscriptions<E> *s = this->get_subscriptions<E>();

	Token ret;
	ret.object = std::make_shared<size_t>(s->subs.size());
	Subscription<E> obj = { ret.object, f };
	s->subs.push_back(std::move(obj));
	return ret;
}

template <typename E>
void EventManager::notify(E const& event)
{
	auto it = m_subs.find(details::get_event_type_id<E>());
	if (it != m_subs.end())
	{
		Subscriptions<E> *s = this->get_subscriptions<E>();
		for (size_t i = 0; i < s->subs.size(); ++i)
		{
			if (!s->subs[i].subject.expired())
				s->subs[i].callback(event);
			//else 
			//	s->subs.erase(s->subs.begin() + i);
			//	This is not thread-safe and also ruins all > i indexes.
			//	Last can be fixed with use of map in Subscriptions instead of vector, 
			//	still not thread-safe tho.
			//	For now it just skips dead subscriptions and never removes them.
		}
	}
}

EventManager::~EventManager()
{
	for (auto &it : m_subs)
		if (it.second) 
			delete it.second;
}


} // namespace core 
} // namespace eot 

#endif 
