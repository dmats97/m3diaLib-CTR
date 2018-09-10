#include "m3d/core/thread.hpp"

namespace m3d {
    Thread::Thread() :
                m_running(false),
                m_started(false) { /* do nothing */ }

    Thread::Thread(std::function<void(m3d::Parameter)> t_function, m3d::Parameter t_parameter, bool t_autostart, bool t_detached, unsigned long long int t_stackSize) :
                m_running(false),
                m_started(false) {
        initialize(t_function, t_parameter, t_autostart, t_detached, t_stackSize);
    }

    Thread::~Thread() {
        threadJoin(m_thread, U64_MAX);
        threadFree(m_thread);
    }

    void Thread::initialize(std::function<void(m3d::Parameter)> t_function, m3d::Parameter t_parameter, bool t_autostart, bool t_detached, unsigned long long int t_stackSize) {
        m_stackSize = t_stackSize;
        m_data.m_parameter = t_parameter;
        m_data.m_function = t_function;

        if (t_autostart) {
            start(t_detached);
        }
    }

    void Thread::setStackSize(unsigned long long int t_stackSize) {
        m_stackSize = t_stackSize;
    }

    void Thread::start(bool t_detached) {
        if (!m_started && !m_running) {
            s32 prio;
            svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
            m_thread = threadCreate(threadFunction, &m_data, m_stackSize, prio - 1, -2, t_detached);
            m_started = true;
            m_running = true;
        }
    }

    void Thread::detach() {
        threadDetach(m_thread);
        m_running = false;
        m_started = false;
    }

    void Thread::join(long long unsigned int t_timeout) {
        if (m_started) {
            threadJoin(m_thread, t_timeout);
            threadFree(m_thread);
            m_running = false;
            m_started = false;
        }
    }

   void Thread::sleep(int t_milliseconds) {
       svcSleepThread(1000000 * t_milliseconds);
   }

   // private methods
   void Thread::threadFunction(void* arg) {
       m3d::Thread::ThreadData data = *static_cast<m3d::Thread::ThreadData*>(arg);
       data.m_function(data.m_parameter);
   }
} /* m3d */
