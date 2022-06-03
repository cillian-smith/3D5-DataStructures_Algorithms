#include "t1.h"
#include "t2.h"
#include <stdio.h> 
#include <stdlib.h>

int is_sorted(int* array, int size){
    int i=0;
	for( i=0; i<size-1; i++ ) {
		if( array[i] > array[i+1] ) { 
            printf("unsorted\n");
            return 0; 
        }
	}
    //printf("sorted\n");
	return 1;
}


void print_test_headers(){
    int n =10; // column width
    int n_first =25; //first column width
    printf("%*s | %*s | %*s | %*s |\n", n_first, "TEST", n, "SORTED", n, "SWAPS", n, "COMPS");
}

void print_test_results(char* test_name, int sorted,  int num_swaps, int num_comparisons){
    int n =10; // column width
    int n_first =25; //first column width
    printf("%*s | %*s | %*d | %*d |\n",n_first, test_name, n, sorted? "YES" : "NO", n, num_swaps, n, num_comparisons);

}

void test_and_print_for_given_array(void (*f) (int*,int), int size, int* array, char* test_name){
    number_comparisons = 0; 
    number_swaps = 0;
    (*f)(array, size);
    print_test_results( test_name, is_sorted(array,size),number_swaps,number_comparisons);
}

void test_all_arrays_of_given_size(void (*f) (int*,int), int size){
    
    print_test_headers();

    int* array =  malloc(size * sizeof(int));

    fill_ascending(array,size);
    test_and_print_for_given_array(f,size, array, "Ascending");
    
    
    fill_descending(array,size);
    test_and_print_for_given_array(f,size, array, "Descending");


    fill_uniform(array,size);
    test_and_print_for_given_array(f,size, array, "Uniform");


    fill_with_duplicates(array,size);
    test_and_print_for_given_array(f,size, array, "Random w duplicates");


    fill_without_duplicates(array,size);
    test_and_print_for_given_array(f,size, array, "Random w/o  duplicates");
    
    printf("\n\n");


}

void test_all_functions_for_given_size(int size){
    printf("Arrays of size %d: \n", size); 
    printf("  Selection sort\n");
    test_all_arrays_of_given_size(selectionSort, size);
    printf("  Insertion sort\n");
    test_all_arrays_of_given_size(insertionSort, size);
    printf("  Quick sort\n");
    test_all_arrays_of_given_size(quickSort, size);
    printf("\n\n");

}


int main()
{

    
    
    test_all_functions_for_given_size(10000);


    return 0;
}