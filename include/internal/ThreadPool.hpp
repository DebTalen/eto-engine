/*
 * Slightly modificated version of the https://github.com/progschj/ThreadPool
 */
#ifndef ETO_THREAD_POOL_HPP
#define ETO_THREAD_POOL_HPP

#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <internal/Assert.hpp>

namespace eto {
namespace internal {


/** Simple implementation of the thread pool */
class ThreadPool
{
public:
	/**
	 *  @brief  Constructor
	 *
	 *  @param  pool_size The number of threads to use, must be >= 1
	 */
	ThreadPool(size_t pool_size);

	~ThreadPool();

	/**
	 *  @brief  Adds a task to the task queue
	 *
	 *  @param  f Executable function 
	 *  @param  args Arguments for the f
	 *  @return std::future with the result of f 
	 */
	template <typename F, typename ...Args>
	auto push(F&& f, Args&& ...args);

	/**
	 *  @brief  Indicates whether all tasks were completed or not 
	 *
	 *  @return true if all tasks were completed, otherwise false
	 */
	bool is_done() const;

	/**
	 *  @brief  Blocks caller's thread until all tasks are done
	 *
	 *  @param  nanosec Waiting time in nanoseconds. If set to 0, waiting time is not limited.
	 */
	void wait_all(size_t nanosec = 0);


	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
private:
	std::vector<std::thread> 	  m_pool;
	std::queue<std::function<void()>> m_tasks;

	std::mutex m_queue_mutex;
	std::mutex m_wait_mutex;
	
	std::condition_variable m_new_task_var;
	std::condition_variable m_wait_var;

	std::atomic_int  m_pending;
	std::atomic_bool m_stop;
};

template <typename F, typename ...Args>
auto ThreadPool::push(F&& f, Args&& ...args)
{
	using return_t = typename std::result_of<F(Args...)>::type;
	++m_pending;

	auto task = std::make_shared< std::packaged_task<return_t()> >(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
	);
	std::future<return_t> res = task->get_future();	
	{
		std::unique_lock<std::mutex> lock(m_queue_mutex);
		// don't allow enqueueing after stopping the pool
		ETO_ASSERT( m_stop.load(std::memory_order_relaxed) == false );
		m_tasks.emplace([task](){ (*task)(); });
	}
	m_new_task_var.notify_one();
	return res;
}

} // namespace internal
} // namespace eto

#endif
