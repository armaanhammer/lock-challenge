#ifndef _LOCK_G_HDR  // header guard
#define _LOCK_G_HDR

#include <mutex>     // std::mutex, std::adopt_lock


namespace chal { //challenge namespace

    /** @brief A movable scoped lock type.
     *
     * This class has been kept as identical to std::lock_guard as possible
     *
     * A unique_lock controls mutex ownership within a scope. Ownership of the
     * mutex can be delayed until after construction and can be transferred
     * to another unique_lock by move construction or move assignment. If a
     * mutex lock is owned when the destructor runs ownership will be released.
     */
    template<typename _Mutex>  // returns type determined by calling function
    class LockGuard
    {
        public:
            typedef _Mutex mutex_type;

            explicit LockGuard(mutex_type& __m) : _M_device(__m)  //no implicit constructor
                { _M_device.lock(); }

            LockGuard(mutex_type& __m, std::adopt_lock_t) : _M_device(__m)
                { } // calling thread owns mutex

            ~LockGuard()
                { _M_device.unlock(); }

            LockGuard(const LockGuard&) = delete;
            LockGuard& operator=(const LockGuard&) = delete;

        private:
            mutex_type&  _M_device;
    };
}

#endif
