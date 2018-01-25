#include <chrono>
#include <internal/ThreadPool.hpp>

using namespace eto::internal;

ThreadPool::ThreadPool(size_t pool_size)
	: m_pending(0), m_stop(0)
{
	ETO_ASSERT( pool_size >= 1 );
	m_pool.reserve(pool_size);
	for (size_t i = 0; i < pool_size; ++i)
	{
		m_pool.emplace_back([this] {
			while(true)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lk(this->m_queue_mutex);
					this->m_new_task_var.wait(lk, [this] {
						return this->m_stop || !this->m_tasks.empty();
					});
					if (this->m_stop && m_tasks.empty())
						return;
					task = std::move(this->m_tasks.front());
					m_tasks.pop();
				}
				task();
				--m_pending;
				m_wait_var.notify_all();
			}
		});
	}
}

ThreadPool::~ThreadPool()
{
	m_stop.store(true, std::memory_order_release);
	m_new_task_var.notify_all();
	for (std::thread &t : m_pool)
		t.join();
}

bool ThreadPool::is_done() const
{
	return m_pending.load(std::memory_order_relaxed);
}

void ThreadPool::wait_all(size_t nanosec) 
{
	std::unique_lock<std::mutex> lk(m_wait_mutex);
	if (nanosec)
		m_wait_var.wait_for(lk, std::chrono::nanoseconds(nanosec), 
				    [this] { return this->is_done();} 
				   );
	else 
		m_wait_var.wait(lk, [this] { return this->is_done(); });
}
