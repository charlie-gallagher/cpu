# Virtual CPU
This is a quick, informal project emulating a processor. At this point, I
haven't implemented some important features, like the status register or
allowing the user to pass in an input file. I'm manually initializing the memory
array in the `main` function, and I've only written a few opcodes. Still, it's
fun. 

### Compile
This program depends on the `unistd.h` header file, so it can't be compiled on
Windows. 


```
gcc -I include -o cpu src/*.c
```

### Example
Although I haven't implemented an input file, the program expects an argument
when you call it. 

```
./cpu file
```

`file` doesn't have to exist (yet). 

---

Charlie Gallagher, August 2021
