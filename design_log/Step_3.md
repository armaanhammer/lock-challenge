# Step 3: Loop threads with condition_var

## Prelims

Initially had trouble parsing requirements. [Reference in Appendix](Step_3_Appendix.md#Initial-Prompt)

---

## Coding Decisions

### Sizing

Want variable sizing (at compile time) - add more or less threads at will and have program perform the same. Feel this is good coding practice, and can potentially reduce complexity.

### How to guarantee ordered thread execution

Thinking of two ways to guarantee ordered thread execution:
* One mutex per thread-to-thread handoff
  * Each possible transition (eg. 1-2 , 2-3 , 3-1) has its own mutex that the 'receiving' thread blocks on.
  * main() is responsible for assigning mutexes to correct threads (eg. thread 3 must be assigned mutex 1 as mutex to unlock)
* One mutex total, with an int passed by reference to maintain count of which thread is next. 
  * each thread must have logic to increment in (eg. thread 3 must 'increment' int to 1)
  
Decided on single mutex and shared int by reference. Reasoning being: each thread already needs to know its ID to print it out. Might as well capitalize on that.

### Storage of thread and objects

* Chosing to continue storing thread objects either in arrays or a vector for ease of joining later.
  * Ran into question about whether to use array vs. vector. 
    * Pros for array:
      * easy
      * quick
    * Cons for array:
      * harder to count size of
      * less expandible
    * Pros for vector:
      * far more expandable
      * possible to easily size
    * Cons for vector:
      * needless complexity
      * slower
      
Decided on array. May update later.

### Indexing

Decided on 0-indexing because easier to code. Will +1 for cout statements so appears as 1-indexing to comply with display output requirements.

---

## Doxygen

Considerations for creating documentation are listed in: [Reference in Appendix](Step_3_Appendix.md#Doxygen-Creation)






