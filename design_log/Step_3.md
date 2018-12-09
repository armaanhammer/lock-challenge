# Step 3: Loop threads with condition_var

## Prelims

Initially had trouble parsing requirements. [Reference in Appendix](Step_3_Appendix.md#Initial-Prompt)

---

## Beginning codeing

### Sizing

Want variable sizing (at compile time) - add more or less threads at will and have program perform the same. Feel this is good coding practice, and can potentially reduce complexity.

### Storage of thread and mutex objects

* Chosing to continue storing thread and mutex objects either in arrays or a vector for ease of joining later.
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
      
Decided on seperate arrays that share same index. May update later.

### Indexing

Decided on 0-indexing because easier to code. Will +1 for cout statements so appears as 1-indexing to comply with display output requirements.

### asdf


---




