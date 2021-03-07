
[Simple Ray-Sphere Tracer link](https://github.com/toksaitov/raytracing-project) 
========================

![Results](https://i.imgur.com/AUNmCix.png)

In this task, you need to modify a simple Ray-Sphere serial tracing program
to run in parallel on execution units of a CUDA-capable GPU.

For this task, you have to get access to a machine with an NVIDIA CUDA GPU. You
can get one from Kaggle or Google Collab projects for a limited time. Use the
provided Jupiter notebook to create the environment. You can also rent the
machine from Google Cloud. Tesla T4 GPUs are available in multiple regions and
has a low price per hour especially for preemptable types of virtual machines.
Remember to commit and push in such environments often, because the machines
can go offline at any moment. Note, that you have to install the CUDA Toolkit
and the compiler on your own. Refer to lecture videos or the official
documentation on how to do that.

## Tasks

1. Open the `raytracer.c` and the `spheres.h` file.

2. Find the `TODO` comments.

3. Write the required CUDA commands, calculate IDs properly.

4. Test the code on a machines with a CUDA-enabled GPU. Compile with `make`, and
   run the code.

```bash
make
time ./raytracer > image.ppm
time ./raytracer-cuda > gpu_image.ppm
```

5. Compare the two images with `vimdiff`. The files must be the same.

```
vimdiff image.ppm gpu_image.ppm
```

6. Compare timing with the serial implementation.

## What to Submit

1. In your private course repository that was given to you by the instructor
   during the lecture, create the path `project-3/`.
2. Put the `raytracer.c` and `spheres.h` files into that directory.
3. Commit and push your repository through Git. Submit the last commit ID to
   Canvas before the deadline.

## Deadline

Check Canvas for information about the deadline for the first part.

