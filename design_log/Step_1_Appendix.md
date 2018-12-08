# Step 1 Appendix

The purpose of this document is to capture ancillary research I have done that only loosely applies to the objectives I am working toward.

## Initial Prompt

> 1) Reimplement c++11 lock_guard (NOT use std::lock_guard). Use namespace
      to allow you to call your class impl "lock_guard" and not conflict with
      std::lock_guard
>
>      LockGuard.h          // header file only impl



### Parsing Prompt
Having difficulty parsing prompt. I took "LockGuard.h // header file only impl" in the prompt to indicate that I am to build a declaration for the class based off of a .h file that was provided.

I see that no LockGuard.h was attached to the email. Perhaps I misunderstand what I am intended to do?

### Additional Prompt

> You need to create LockGuard.h. I did not attach it. You need to re-create the functionality of std::lock_guard.  You can put all the code in a header file. It’s a common technique for STL and other utilities and is often referred to as “header only” so the user of the utility knows they can just include the header and not have to link a library to their project.
>
> You will need to fully implement the functions. In the header file.
>
> LockGuard.h
>
> Set namespace to avoid naming collisions with std::lock_guard. std:: being a name space.
>
> class LockGuard {
>
>   … implementation …
>
> }

---


## Template
```C++
template <class Mutex>
```

##### Reference:
http://www.cplusplus.com/doc/oldtutorial/templates/

----

## Mutex

https://www.youtube.com/watch?v=I-hZkUa9mIs  <<< Interesting that this video says calling the mutex's own lock&unlock function is _not_ recommended. This seems to imply that in general, it is best to use a lock_guard function. Also notes that lock_guard does not release the lock until out of scope, unlike unique_lock, which releases the lock upon `locker.unlock();`. This provides a constraint on my code later.

https://www.youtube.com/watch?v=LL8wkskDlbs  <<< Initial video of C++ Threading series. Realizing it would be useful to watch them all to brush up. Diverting to doing that now.

Points of note:

> Once a parent thread calls a child thread, it is important to encapsulate any further work the parent thread does before re-joining the child thread in a try, catch block to prevent an exception in the parent from destroying the child before it is done. Ex:
>
> ```C++
> int main(){
>     std::thread t1(function_1);  // t1 starts running
>
>     try {
>           do_something_dangerous();
>     } catch (...) {
>           t1.join();
>           throw;
>
>     ti.join();
>     return 0;
> }
> ```

---

## Typedef
```C++
    typedef Mutex mutex_type;
```



---

## Operator Overload
```C++
lock_guard& operator=(lock_guard const&) = delete;
```

https://www.youtube.com/watch?v=PgGhEovFhd0

https://www.youtube.com/watch?v=mS9755gF66w  <<< Says best practice: only use when usage is intuitive. Avoid if ppl would need to go to your function to understand what it does.

###### Note:
"Passing by const reference to avoid copying". Any function that doesn't modify the class should be const.
> ```C++
> Vector2 Add(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
> ```

The operator overload analog:
> ```C++
> Vector2 operator+(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
> ```

An alternative approach is:

> ```C++
> Vector2 operator+(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
>
> Vector2 Add(const Vector2& other) const {
>     return *this + other;  // deref because 'this' is const pntr in current scope
> }
> ```

Yet another alternative approach is:

> ```C++
> Vector2 operator+(const Vector2& other) const {
>     return Vector2(x + other.x, y + other.y);
> }
>
> Vector2 Add(const Vector2& other) const {
>     return operator+(other);  // address operator+ like a function
> }
> ```


###### Overloading the << operator
Overloading the << operator to include Vector2 containing two floats
> ```C++
> std::ostream& operator<<(std::ostream& stream, const Vector2& other) {
>     stream << other.x << ", " << other.y;
>     return stream;
> }
> ```


###### Question from above youtube video:
What does this do? Says it is a constructor definition. Occurs inside of a struct.
> ```C++
> Vector2(float x, float y)
>     : x(x), y(y) {}
> ```

---

## Resource Acquisition is Initialization

https://www.youtube.com/watch?v=ojOUIg13g3I

Don't combine the operation of storing objects in a shared pointer with any other statement.

---


## Range based for loop

```C++
    // for each element th in array threads, join th
    // use auto& to guarantee sharing variable by reference 
    for (auto& th : threads) th.join();
```

https://stackoverflow.com/questions/29859796/c-auto-vs-auto
https://www.youtube.com/watch?v=2yR8dcKT-Ck

syntax:

```C++
for ( element : collection ) {
      //do something
}
```
