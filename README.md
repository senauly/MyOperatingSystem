# MyOperatingSystem
- It is homework for the Operating Systems course.

## General Info
- I aim to design new features or change the existing situations on the operating system which is created by Viktor Engelmann. He explains how to write his operating system step by step in the **wyoos** on YouTube. 
- I implemented a multithreading library for this simple OS.  
- I have 2 threads that communicate with each other in a **producer-consumer** fashion.  
- I showed that without defining **critical regions** my threads cause race conditions. 
- I used the **Petersons algorithm** between two threads to show that the race condition problems are addressed.

#### Multithreading Library
- My library consists of functions for creating, terminating, yielding, and joining the threads. 

## Setup

To compile the code:
```
$ cd ../MyOperatingSystem
$ make mykernel.iso
```
After compiling the code you should create a Virtual Machine from the mykernel.iso file.

#### Test

I created one test for creating, terminating, and joining; another test for yielding.
- **Test 1:** In my output, I printed the alphabet for regular output. 2 threads prints the alphabet in harmony there's no race condition.
- **Test 2:** Thread 1 prints A, Thread 2 prints B. If a thread is on yield, the other thread's char will be printed successively during that time. 
