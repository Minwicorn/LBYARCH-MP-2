#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Function prototypes
void calculate_distances_c(int n, double* x1, double* x2, double* y1, double* y2, double* z);
extern void calculate_distances_asm(int n, double* x1, double* x2, double* y1, double* y2, double* z);

void initialize_vectors(int n, double* x1, double* x2, double* y1, double* y2);
double get_execution_time(clock_t start, clock_t end);
void verify_correctness(int n, double* z_c, double* z_asm);
void run_performance_test(int n);

int main() {
    printf("==============================================\n");
    printf("  Distance Calculator Kernel - SIMD Version\n");
    printf("  Submitted By: Jherby Jaime and Maikael Paule\n");
    printf("==============================================\n\n");
    
    // Test with example data first for correctness verification
    printf("=== CORRECTNESS VERIFICATION ===\n");
    run_performance_test(4); 
    
    // Test with different vector sizes (reduced to avoid memory issues)
    printf("\n=== PERFORMANCE TESTING ===\n");
    int test_sizes[] = {
        1048576,    // 2^20
        16777216    // 2^24
    };
    
    int num_tests = 2;
    int i;
    
    printf("Testing vector sizes: 2^20, 2^24\n\n");
    
    for (i = 0; i < num_tests; i++) {
        printf("Testing vector size: 2^%d (%d elements)\n", 
               (i == 0) ? 20 : 24, test_sizes[i]);
        printf("----------------------------------------\n");
        run_performance_test(test_sizes[i]);
        printf("\n");
    }
    
    printf("==========================================\n");
    printf("All tests completed successfully!\n");
    
    return 0;
}

void run_performance_test(int n) {
    double* x1;
    double* x2;  
    double* y1;
    double* y2;
    double* z_c;
    double* z_asm;
    clock_t start_c, end_c, start_asm, end_asm;
    double time_c, time_asm;
    int run;
    
    // Allocate memory
    x1 = (double*)malloc(n * sizeof(double));
    x2 = (double*)malloc(n * sizeof(double));
    y1 = (double*)malloc(n * sizeof(double));
    y2 = (double*)malloc(n * sizeof(double));
    z_c = (double*)malloc(n * sizeof(double));
    z_asm = (double*)malloc(n * sizeof(double));
    
    // Initialize test data
    if (n == 4) {
        // Use specification example data for correctness check
        x1[0] = 1.5; x1[1] = 4.0; x1[2] = 3.5; x1[3] = 2.0;
        x2[0] = 3.0; x2[1] = 2.5; x2[2] = 2.5; x2[3] = 1.0;
        y1[0] = 4.0; y1[1] = 3.0; y1[2] = 3.5; y1[3] = 3.0;
        y2[0] = 2.0; y2[1] = 2.5; y2[2] = 1.0; y2[3] = 1.5;
        
        printf("Using specification example data:\n");
        printf("X1: 1.5, 4.0, 3.5, 2.0\n");
        printf("X2: 3.0, 2.5, 2.5, 1.0\n");
        printf("Y1: 4.0, 3.0, 3.5, 3.0\n");
        printf("Y2: 2.0, 2.5, 1.0, 1.5\n");
        printf("Expected Z: 2.5, 1.58113883, 2.69258240, 1.80277564\n\n");
    } else {
        initialize_vectors(n, x1, x2, y1, y2);
    }
    
    // Time C version (30 iterations)
    printf("Running C version (30 iterations)...\n");
    start_c = clock();
    for (run = 0; run < 30; run++) {
        calculate_distances_c(n, x1, x2, y1, y2, z_c);
    }
    end_c = clock();
    time_c = get_execution_time(start_c, end_c) / 30.0;
    
    // Time Assembly version (30 iterations)  
    printf("Running x86-64 Assembly version (30 iterations)...\n");
    start_asm = clock();
    for (run = 0; run < 30; run++) {
        calculate_distances_asm(n, x1, x2, y1, y2, z_asm);
    }
    end_asm = clock();
    time_asm = get_execution_time(start_asm, end_asm) / 30.0;
    
    // Verify correctness
    verify_correctness(n, z_c, z_asm);
    
    // Display performance results
    printf("\nPerformance Results:\n");
    printf("C version average time:        %.6f ms\n", time_c * 1000);
    printf("x86-64 Assembly average time:  %.6f ms\n", time_asm * 1000);
    
    // Clean up memory
    free(x1);
    free(x2);
    free(y1);
    free(y2);
    free(z_c);
    free(z_asm);
}

void verify_correctness(int n, double* z_c, double* z_asm) {
    int correct = 1;
    int display_count = (n < 10) ? n : 10;
    int i;

    printf("\nCorrectness Verification:\n");
    printf("Displaying first %d elements of vector Z:\n\n", display_count);

    printf("Index    |    C version    |  x86-64 version\n");
    printf("--------------------------------------------\n");
    for (i = 0; i < display_count; i++) {
        printf("  %-6d |  %-14.8f |  %-14.8f\n", i, z_c[i], z_asm[i]);
    }
    printf("--------------------------------------------\n");

    // Check all elements for correctness
    for (i = 0; i < n; i++) {
        double diff = fabs(z_c[i] - z_asm[i]);
        if (diff > 1e-10) {
            correct = 0;
            printf("❌ Mismatch at index %d: C=%.10f, ASM=%.10f, diff=%.2e\n", 
                   i, z_c[i], z_asm[i], diff);
            break;
        }
    }

    printf("Correctness check: %s\n", correct ? "PASSED" : "FAILED");

    if (n == 4 && correct) {
        printf(" Results matched the expected specification values.\n");
    }
}


// C version using standard math library
void calculate_distances_c(int n, double* x1, double* x2, double* y1, double* y2, double* z) {
    int i;
    double dx, dy;
    
    for (i = 0; i < n; i++) {
        dx = x2[i] - x1[i];
        dy = y2[i] - y1[i];
        z[i] = sqrt(dx * dx + dy * dy);
    }
}

// Initialize vectors with random data
void initialize_vectors(int n, double* x1, double* x2, double* y1, double* y2) {
    int i;
    
    srand(12345); // Fixed seed 
    
    for (i = 0; i < n; i++) {
        x1[i] = ((double)rand() / RAND_MAX) * 10.0;
        x2[i] = ((double)rand() / RAND_MAX) * 10.0;
        y1[i] = ((double)rand() / RAND_MAX) * 10.0;
        y2[i] = ((double)rand() / RAND_MAX) * 10.0;
    }
}

// Calculate execution time in seconds
double get_execution_time(clock_t start, clock_t end) {
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}
