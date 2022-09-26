#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
This approach is also good since it provides better extensibility for more operations of aggregate function following the open-closed principle but for each element, we need to perform 3 operations on the float-point numbers (multiplication, then addition, then division) and this may affect the precision of the result.
*/

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*));


// A struct for the mean operation and capable of holding two values 
// The  temp_mean is the mean value for n elements
typedef struct Mean{
	double temp_mean;
	int n;
} Mean;

// Applies the addition operation on two elements
void* addInt(const void* a, const void* b){
	int* output = malloc(sizeof(int));
	*output = (*((int*)a) + *((int*)b));
	return output;
}

// Applies the addition operation on two elements
void* addDouble(const void* a, const void* b){
        double* output = malloc(sizeof(double)); 
        *output = (*((double*)a) + *((double*)b));
        return output;
}

// Applies the multiplication operation on two elements
void* mulInt(const void* a, const void* b){
	int* output = malloc(sizeof(int));
	*output = (*((int*)a) * *((int*)b));
	return output;
}

// Applies the multiplication operation on two elements
void* mulDouble(const void* a, const void* b){
        double* output = malloc(sizeof(double));
        *output = (*((double*)a) * *((double*)b));
        return output;
}

// Applies the arithmetic mean operation on two elements
void* meanInt(const void* a, const void* b){
        Mean* mean = (Mean*)a;
        mean->temp_mean = mean->temp_mean * mean->n + *((int*) b);
        mean->n++;
        double sum = mean->temp_mean;
        mean->temp_mean = sum / mean->n;
        return mean;
}

// Applies the arithmetic mean operation on two elements
void* meanDouble(const void* a, const void* b){
        Mean* mean = (Mean*)a;
        mean->temp_mean = mean->temp_mean * mean->n + *(double*) b;
        mean->n++;
        double sum = mean->temp_mean;
        mean->temp_mean = sum / mean->n;
        return mean;
}


// Applies the aggregation operation opr on n elements
void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)){

	if (base==NULL || initial_value==NULL || opr==NULL){
		return NULL;
	}
	
	if (n<2){
		return base;
	}

	void* output = initial_value;

	for (int i=0; i<n; i++){
		output = opr(output, base + size * i);
	}

	return output;
}



// Main function
int main(){
        int n = 5;
        int* arr = calloc(n, sizeof(int));

        printf("ints = [");
        arr[0] = 8;
	printf("%d,", arr[0]);
        arr[1] = 13;
	printf("%d,", arr[1]);
        for (int i=2; i<n; i++){
                *(arr+i) = *(arr+i-1) + *(arr+i-2);

                printf("%d,", arr[i]);
        }
        printf("\b]\n");

        int n2 = 5;
        double* arr2 = calloc(n2, sizeof(double));

        printf("doubles = [");
        arr2[0] = 1;
        printf("%.2f,", arr2[0]);
        arr2[1] = 2;
        printf("%.2f,", arr2[1]);
        for (int i=2; i<n2; i++){
                *(arr2+i) = *(arr2+i-1) + *(arr2+i-2);

                printf("%.2f,", arr2[i]);
        }
        printf("\b]\n");


	int* init1 = malloc(sizeof(int));
	*init1 = 0;
	int* init2 = malloc(sizeof(int));
	*init2 = 1;
	double* init3= malloc(sizeof(double));
	*init3 = 0.0;
	double* init4 = malloc(sizeof(double));
	*init4 = 1.0;


	// Addition
        int* result1a = (int*)aggregate(arr, sizeof(int), n, init1, addInt);
        printf("Addition(ints) = %d\n", *result1a);

	double* result2a = (double*)aggregate(arr2, sizeof(double), n2, init3, addDouble);
        printf("Addition(doubles) = %.2f\n", *result2a);

	// Multiplication
        int* result1m = (int*)aggregate(arr, sizeof(int), n, init2, mulInt);
        printf("Multiplication(ints) = %d\n", *result1m);

	double* result2m = (double*)aggregate(arr2, sizeof(double), n2, init4, mulDouble);
        printf("Multiplication(doubles) = %.2f\n", *result2m);
        
        Mean* initMeanInt = malloc(sizeof(Mean));
        initMeanInt->temp_mean = 0.0;
        initMeanInt->n = 0;
        Mean* initMeanDouble = malloc(sizeof(Mean));
        initMeanDouble->temp_mean = 0.0;
        initMeanDouble->n = 0;

	// Arithmetic Mean
        double* result1mean = (double*)aggregate(arr, sizeof(int), n, initMeanInt, meanInt);
        printf("Mean(ints) = %.2f\n", *result1mean);

	double* result2mean = (double*)aggregate(arr2, sizeof(double), n2, initMeanDouble, meanDouble);
        printf("Mean(doubles) = %.2f\n", *result2mean);

        // Free pointers
        free(arr);free(arr2);free(init1);free(init4);free(init2);free(init3);
        free(result1a);free(result2a);free(result1m);free(result2m);
        free(result1mean);free(result2mean);


        return EXIT_SUCCESS;
}


