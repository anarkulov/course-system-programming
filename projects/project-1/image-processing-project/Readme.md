### COM 391-451, System Programming
# Project #1

![Sample Source Image](https://i.imgur.com/40Bvuur.png)

![Brightness and Contrast Sample](https://i.imgur.com/ezN6oDV.png)

## Part 1-1, Brightness and Contrast

In this part of the project, you will have to implement a simple brightness and
contrast adjustment filter for a 2D image with SIMD intrinsics. The brightness
adjustment requires using a simple addition operation. The contrast adjustment
requires using multiplication. The final calculated value should be converted
back to an integer color channel value.

The overall formula is

```C
// ...
channel_value = (unsigned char) (channel_value * contrast + brightness);
```

The image is represented as an array of pixel color values. Every pixel is
represented as three-byte integers with channel values ranging from 0 to 255.
Your test image is stored in the BMP/DIB format. Your code template provides a
simple library to read, decode, and write some variants of bitmap images. You
can refer to the following image (courtesy to [Verpies](https://commons.wikimedia.org/wiki/File:BMPfileFormat.png))
to understand how to work with bitmap data.

![BMP Image Structure](https://i.imgur.com/CKrcD9u.png)

You are given the assembly code and all the SIMD instructions. This code is not
portable between compilers. You have to convert it into SIMD intrinsics
instruction by instruction.

### Tasks

1. Open the `brightness.c` file.

2. Find the `TODO` comment.

3. Write the intrinsics.

4. Test the code on Kaggle machines with the support of the AVX512 extensions.
   You can use the `com-392-451-project-1.ipynb` Jupiter notebook to prepare the
   environment on their server. You will have to register an account.
   Sometimes, you have to reload the notebook to get a server with the CPU
   supporting `AVX512f` SIMD instructions. It is recommended to use the
   `auca.space` server to develop and write code and only test on Kaggle
   machines.

## Part 1-2, The Sepia Filter

In this part, you need to do an opposite operation. You have all the intrinsics
to apply a Sepia filter to an image. You have to make the code less portable
between compilers by writing inline assembly.

The Sepia filter converts a color image to a duotone image with a dark
Brown-Gray color. The algorithm is explained in the following
[paper](https://software.intel.com/en-us/articles/image-processing-acceleration-techniques-using-intel-streaming-simd-extensions-and-intel-advanced-vector-extensions)
written by Petter Larsson and Eric Palmer.

![The Sepia Filter](https://i.imgur.com/bKsDknj.png)

### Tasks

1. Open the `sepia.c` file.

2. Find the `TODO` comment.

3. Write the inline assembly.

4. Test the code on Kaggle machines with the support of the AVX512 extensions.
   You can use the `com-392-451-project-1.ipynb` Jupiter notebook to prepare the
   environment on their server. You will have to register an account.
   Sometimes, you have to reload the notebook to get a server with the CPU
   supporting `AVX512f` SIMD instructions. It is recommended to use the
   `auca.space` server to develop and write code and only test on Kaggle
   machines.

### What to Submit

1. In your private course repository that was given to you by the instructor
   during the lecture, create the path `project-1/part-1/`.

2. Put the `brightness.c` and `sepia.c` files into that directory.

3. Commit and push your repository through Git. Submit the last commit ID to
   Canvas before the deadline.

## Part 2-1, The Threaded Sepia Filter

You are now given a threaded implementation of the Sepia program in the
`mt_sepia.c` file. You have to study the code to figure out how the POSIX
Threads interface is being used to utilize the power of a multicore/cpu machine.
After that, you have to port your inline assembly code from Part 1-2, recompiler
the program with the extra `-pthread` flag, and ensure that everything still
works.

### Tasks

1. Open the `mt_sepia.c` file.

2. Find the `TODO` comment.

3. Move your inline assembly from Part 1-2.

4. Test the code on a machine with AVX512 support.

### What to Submit

1. In your private course repository that was given to you by the instructor
   during the lecture, create the path `project-1/part-2/`.

2. Put the `mt_sepia.c` file into that directory.

3. Commit and push your repository through Git. Submit the last commit ID to
   Canvas before the deadline.

## Part 2-2, The Threaded Brightness and Contrast Filter

In this part, you have to create a multithreaded version of the brightness and
contrast filter. Create a copy of the `brightness.c` file under the name
`mt_brightness.c`. Use `mt_sepia.c` to help you make the `mt_brightness.c`
utilize all the threads of your test machine.

### Tasks

1. Create the `mt_brightness.c` file from `brightness.c`.

3. Make the multithreaded version of the code with the help of PThreads. Use
   `mt_sepia.c` and all the given extra header files to help you make the
   code multithreaded.

4. Test the code on a machine with AVX512 support.

### What to Submit

1. In your private course repository that was given to you by the instructor
   during the lecture, create the path `project-1/part-2/`.

2. Put the `mt_brightness.c` file into that directory.

3. Commit and push your repository through Git. Submit the last commit ID to
   Canvas before the deadline.

### Deadline

Check Canvas for information about the deadline for the first part.

## Research Papers

* [Image Processing Acceleration Techniques using Intel Streaming SIMD Extensions](https://software.intel.com/en-us/articles/image-processing-acceleration-techniques-using-intel-streaming-simd-extensions-and-intel-advanced-vector-extensions)

## Documentation

    man make
    man gcc
    man as
    man gdb
    man objdump

## Links

### C, GDB, Radare2

* [Beej's Guide to C Programming](https://beej.us/guide/bgc)
* [GDB Quick Reference](http://users.ece.utexas.edu/~adnan/gdb-refcard.pdf)

### x86 ISA

* [Intel x86 Software Developer Manuals](https://software.intel.com/en-us/articles/intel-sdm)
* [System V AMD64 ABI](https://software.intel.com/sites/default/files/article/402129/mpx-linux64-abi.pdf)
* [X86 Opcode Reference](http://ref.x86asm.net/index.html)
* [X86 Instruction Reference](http://www.felixcloutier.com/x86)
* [Optimizing Subroutines in Assembly Language](http://www.agner.org/optimize/optimizing_assembly.pdf)

### x86 SIMD

* [SIMD Basics](https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX)
* [SIMD Intrinsics Guide](https://software.intel.com/sites/landingpage/IntrinsicsGuide)
* [Visual SIMD Guide](https://www.officedaytime.com/simd512e/)

### Assemblers

* [Linux assemblers: A comparison of GAS and NASM](https://www.ibm.com/developerworks/library/l-gas-nasm/index.html)
* [GCC Inline Assembly HOWTO](https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html)
* [GAS Syntax](https://en.wikibooks.org/wiki/X86_Assembly/GAS_Syntax)

## Books

* C Programming: A Modern Approach, 2nd Edition by K. N. King
* Assembly Language for x86 Processors, 7th Edition by Kip R. Irvine
