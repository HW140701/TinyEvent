/*
Author:StubbornHuang
Data:2023.1.31
Email:stubbornhuang@qq.com
*/


#ifndef _EVENT_H_
#define _EVENT_H_

#include <functional>
#include <map>
#include <type_traits>

#ifndef EVENT_NO_THREAD_SAFETY
#define EVENT_THREAD_SAFETY
#endif // !EVENT_NO_THREAD_SAFETY

#ifdef EVENT_THREAD_SAFETY
#include <atomic>
#include <mutex>
#endif // EVENT_THREAD_SAFETY

#ifdef EVENT_THREAD_SAFETY
#define DELEGATE_ID_TYPE std::atomic_uint64_t
#else
#define DELEGATE_ID_TYPE std::uint64_t
#endif // EVENT_THREAD_SAFETY

namespace stubbornhuang
{
	static DELEGATE_ID_TYPE DELEGATE_ID = 1;

	template<typename Prototype> class Event;

	template<typename ReturnType, typename ...Args>
	class Event <ReturnType(Args...)>
	{
	private:
		using return_type = ReturnType;
		using function_type = ReturnType(Args ...);
		using std_function_type = std::function<function_type>;
		using function_pointer = ReturnType(*)(Args...);

	private:
		class Delegate
		{
		public:
			Delegate() = delete;
			Delegate(int id,std_function_type std_function_func)
				:m_Handler(nullptr),m_Id(-1)
			{
				if (std_function_func == nullptr)
					return;

				m_Id = id;
				m_Handler = std_function_func;
			}

			void Invoke(Args ...args)
			{
				if (m_Handler != nullptr)
				{
					m_Handler(args...);
				}
			}

		private:
			int m_Id;
			std_function_type m_Handler;
		};

	public:
		int AddDelegate(std_function_type std_function_func)
		{
			if (std_function_func == nullptr)
				return -1;
			
			std::shared_ptr<Delegate> pDelegate = std::make_shared<Delegate>(DELEGATE_ID, std_function_func);

#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY


			m_delegates.insert(std::pair<int, std::shared_ptr<Delegate>>(DELEGATE_ID, pDelegate));

			return DELEGATE_ID++;
		}

		bool RemoveDelegate(int delegate_id)
		{
#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY

			if (m_delegates.count(delegate_id) == 0)
				return false;

			m_delegates.erase(delegate_id);

			return true;
		}


		int operator += (std_function_type std_function_func)
		{
			return AddDelegate(std_function_func);
		}

		bool operator -= (int delegate_id)
		{
			return RemoveDelegate(delegate_id);
		}

		void Invoke(Args ...args)
		{
#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY

			for (const auto& key : m_delegates)
			{
				key.second->Invoke(args...);
			}
		}

		bool Invoke(int delegate_id, Args ...args)
		{
#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY

			if (m_delegates.count(delegate_id) == 0)
				return false;

			m_delegates[delegate_id]->Invoke(args...);


			return true;
		}

		void operator() (Args ...args)
		{
#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY

			for (const auto& key : m_delegates)
			{
				key.second->Invoke(args...);
			}
		}

		bool operator() (int delegate_id, Args ...args)
		{
#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY

			if (m_delegates.count(delegate_id) == 0)
				return false;

			m_delegates[delegate_id]->Invoke(args...);


			return true;
		}

		int GetDelegateSize()
		{
#ifdef EVENT_THREAD_SAFETY
			std::lock_guard<std::mutex> guard_mutex(m_event_mutex);
#endif // EVENT_THREAD_SAFETY

			return m_delegates.size();
		}


	private:
		std::map<int, std::shared_ptr<Delegate>> m_delegates;

#ifdef EVENT_THREAD_SAFETY

		std::mutex m_event_mutex;
#endif // EVENT_THREAD_SAFETY
	};
}


#endif // !_EVENT_H_
