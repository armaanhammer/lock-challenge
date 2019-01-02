/// 
/// \file LockGuard.h
/// \brief contains namespace chal and class LockGuard
/// 
/// \note set DEBUG to false within chal namespace to disable debug output.
/// \note needs to be compiled with flag -pthread.
///
/// \author Armaan Roshani
/// \bug no known bugs
/// 

#ifndef _LOCK_G_H  // header guard
#define _LOCK_G_H

#include <mutex>     // std::mutex, std::adopt_lock




namespace chal { // challenge namespace

    bool DEBUG = true;

    /// \brief A movable scoped lock type.
    ///
    /// \note This class has been kept as identical to std::lock_guard as possible
    ///
    /// A unique_lock controls mutex ownership within a scope. Ownership of the
    /// mutex can be delayed until after construction and can be transferred
    /// to another unique_lock by move construction or move assignment. If a
    /// mutex lock is owned when the destructor runs ownership will be released.
    ///
    template<typename _Mutex>  ///< returns type determined by calling function
    class LockGuard
    {
        public:
            typedef _Mutex mutex_type;


            // no implicit constructor
            explicit LockGuard(mutex_type& __m) : _M_device(__m) { 
                _M_device.lock(); 
                
                if(DEBUG) {
                    std::cerr << "LockGuard locked" << std::endl;
                }
            }


            // calling thread owns mutex
            LockGuard(mutex_type& __m, std::adopt_lock_t) : _M_device(__m) {  

                if(DEBUG) {
                    std::cerr << "LockGuard adopted" << std::endl;
                }
            }


            ~LockGuard() { 
                _M_device.unlock();
                
                if(DEBUG) {
                    std::cerr << "LockGuard unlocked" << std::endl;
                }
            }

            // generate compile error if copy attempted
            // (supposed to be un-copyable)
            LockGuard(const LockGuard&) = delete;  // copy constructor
            LockGuard& operator=(const LockGuard&) = delete;  // copy assignment operator

        private:
            mutex_type&  _M_device;
    };
}

#endif
