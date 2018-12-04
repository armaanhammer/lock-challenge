#include <mutex>          // std::mutex, std::lock_guard, std::adopt_lock


  /** @brief A movable scoped lock type.
   *
   * A unique_lock controls mutex ownership within a scope. Ownership of the
   * mutex can be delayed until after construction and can be transferred
   * to another unique_lock by move construction or move assignment. If a
   * mutex lock is owned when the destructor runs ownership will be released.
   */
  template<typename _Mutex>
    class lock_guard
    {
    public:
      typedef _Mutex mutex_type;

      explicit lock_guard(mutex_type& __m) : _M_device(__m)
      { _M_device.lock(); }

      lock_guard(mutex_type& __m, std::adopt_lock_t) : _M_device(__m)
      { } // calling thread owns mutex

      ~lock_guard()
      { _M_device.unlock(); }

      lock_guard(const lock_guard&) = delete;
      lock_guard& operator=(const lock_guard&) = delete;

    private:
      mutex_type&  _M_device;
    };

