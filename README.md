# Distance Calculator Kernel x86-to-C interface programming project

# Overview

This project provides an implementation for calculating the distances between the coordinate points across two vectors. It also includes performance testing and verification of the correctness of the conversion.

# Features

**1. Calculates Euclidean distance between vector pairs**  
Computes the Euclidean distance for each element in two-dimensional vector pairs using the formula:  
&nbsp;&nbsp;&nbsp;&nbsp;`Z[i] = sqrt((X2[i] - X1[i])² + (Y2[i] - Y1[i])²)`  
This is implemented in both a scalar C function and an optimized SIMD assembly routine.

**2. Performance Testing**  
Measures and compares the execution time of the C and x86-64 SIMD implementations using large vector sizes (e.g., 2²⁰ and 2²⁴ elements) over 30 iterations. This helps evaluate the benefits of low-level optimizations and hardware-level parallelism.

**3. Verifies correctness with expected output**  
Ensures both implementations produce accurate results by comparing outputs to known expected values. The correctness check displays the first few results and confirms they match the mathematical expectation within a small error margin.


# Setup Instructions

**1. Clone the Repository**
git clone https://github.com/Minwicorn/LBYARCH-MP-2.git
cd LBYARCH-MP-2

**2. Compile and Build the Project**
nasm -f win64 LBYARCHMP2.asm -o LBYARCHMP2.obj
gcc LBYARCHMP2.c LBYARCHMP2.obj -o LBYARCHMP2.exe -lm

**3. Run the Program**
LBYARCHMP2.exe

# Performance Analysis

The SIMD-optimized x86-64 assembly kernel demonstrates a significant performance advantage over the standard C implementation, particularly with large datasets. At a vector size of 2²⁴ (over 16 million elements), the SIMD version completed its task in approximately 82.67 milliseconds, while the C version took 205.37 milliseconds—yielding nearly a **2.5× speedup**. This improvement is attributed to SIMD’s ability to perform parallel operations on multiple data elements simultaneously using vector registers, compared to the scalar nature of C which processes data sequentially.

Interestingly, both versions produced identical results in the correctness checks, validating the functional equivalence of the SIMD and C kernels. The consistent speedup observed across different vector sizes confirms the scalability of the SIMD approach. Overall, this highlights how low-level optimization through assembly and SIMD can yield substantial performance gains for compute-intensive numerical tasks, especially when processing large volumes of data.

# Performance Test Snapshot



# Correctness Snapshot


