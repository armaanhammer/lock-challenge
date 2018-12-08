# Step 3: Loop threads with condition_var

## Prelims

Initially had trouble parsing requirements. [Reference in Appendix](Step_3_Appendix.md#Initial-Prompt)

---

## Beginning codeing

* Want dynamic sizing - add more or less threads at will and have program perform the same.
* Chosing to continue storing thread objects in an array for ease of joining later.
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





