# Step 4 Appendix

Coding
---

When I read that I needed to protect a resource, I immediately thought that the easiest thing to do would be to protect printing to standard out because I already had the print function. I thought about it though, and realized it would be a bit silly to do that because I would want to use my DEBUG statements to verify that my resource was being protected, and I initially coded all of my DEBUG statements printing to standard out. 

So, by verifying that my resource was being protected, I was implicitly leaving the resource unprotected. Instead, I decided to protect a file output stream as a shared resource. I started to write a class to open a file and write to it, similar to the example found here: https://stackoverflow.com/a/33597665 Before I got far though, I realized that I just needed to change all of my debug statements to print to standard error, and the problem was avoided entirely. I knew that Step 5 was going to take effort already, so I decided to devote my effort to that, rather than fixing something in this step that was not broken.
