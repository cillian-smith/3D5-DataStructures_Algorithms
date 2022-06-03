#include <stdio.h>
#include <stdlib.h>

int compar = 0;
const int SIZE = 20;

//Fills the array with ascending, consecutive numbers, starting from 0.
void fill_ascending(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = i;
    }
}
//Fills the array with descending numbers, starting from size-1
void fill_descending(int *array, int size)
{
    int decend = (size - 1);
    for (int i = 0; i < size; i++)
    {
        array[i] = decend--;
    }
}

//Fills the array with uniform numbers.
void fill_uniform(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = 3;
    }
}

//Fills the array with random numbers within 0 and size-1. Duplicates are allowed.
void fill_with_duplicates(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % (size - 1);
    }
}

//Fills the array with unique numbers between 0 and size-1 in a shuffled order. Duplicates are not allowed.
void fill_without_duplicates(int *array, int size)
{
    fill_ascending(array, size);
    for (int i = 0; i < size - 1; ++i)
    {
        int j = rand() % (size - i) + i;
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void printArray(int *arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
//-----------------------------------------------------------------
void swap(int array[], int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
int compare(int a, int b){ 
    return (a-b);
}

void insertionSort(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        
        for (int k = i; k > 0 && arr[k] < arr[k - 1]; k--)
        {
            compar++;
            swap(arr, k, k - 1);
            
        }
    }
}

void selectionSort(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        int index_smallest = i;
        for (int k = i; k < size; k++)
        {
            compar++;
            if (arr[k] < arr[index_smallest])
            {
                index_smallest = k;
            }
        }
        swap(arr, index_smallest, i);
    }
}

int partitionHoare1(int array[], int p, int r)
{
    compar++;
    if (p == r)
    {
        return p;
    }

    int pivot = array[r];
    int i = p - 1;
    int j = r;
    for (;;)
    {
        do
        {
            i++;
            compar++;
            if (i == r)
            {
                break;
            }
            compar++;
        } while (array[i] < pivot);
        do
        {
            j--;
            compar++;
            if (j == p)
            {
                break;
            }
            compar++;
        } while (array[j] > pivot);
        compar++;
        if (i >= j)
        {
            swap(array, i, r);
            return i;
        }
        swap(array, i, j);
    }
}

void quickSortAlgorithm1(int arr[], int low, int high)
{
    compar++;
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partitionHoare1(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSortAlgorithm1(arr, low, pi - 1);
        quickSortAlgorithm1(arr, pi + 1, high);
    }
}
void quickSort1(int arr[], int size)
{
    quickSortAlgorithm1(arr, 0, size - 1);
}
int partitionHoare(int array[], int p, int r)
{
    if (compare(p,r) == 0)// if (p == r)
    {
        return p;
    }

    int pivot = array[r];
    int i = p - 1;
    int j = r;
    for (;;)
    {
        do
        {
            i++;
            if (compare(i,r) == 0) // if (i == r)
            {
                break;
            }
        }while (compare(array[i], pivot) < 0); //while (array[i] < pivot)
        do
        {
            j--;
            if (compare(j,p) == 0) //if (j == p)
            {
                break;
            }
        } while (compare(array[j], pivot) > 0); //while (array[j] > pivot)
        if (compare(i,j) >= 0) //if (i >= j)
        {
            swap(array, i, r);
            return i;
        }
        swap(array, i, j);
    }
}

void quickSortAlgorithm(int arr[], int low, int high)
{
    if (compare(low,high) < 0) // if (low < high)
    {
        int q = partitionHoare(arr, low, high);
        quickSortAlgorithm(arr, low, q - 1);
        quickSortAlgorithm(arr, q + 1, high);
    }
}

void quickSort(int arr[], int size)
{
    quickSortAlgorithm(arr, 0, size - 1);
}

int main()
{
    int array[SIZE];
    int array1[SIZE];
    int array2[SIZE];


    fill_without_duplicates(array1, SIZE);
    quickSort1(array1, SIZE);
    printArray(array1, SIZE);
    printf("Comp: %d \n", compar);

    fill_without_duplicates(array2, SIZE);
    quickSort(array2, SIZE);
    printArray(array2, SIZE);
    printf("Comp under test: %d \n", compar);

    // printf("\n");

    // fill_without_duplicates(array, SIZE);
    // printArray(array, SIZE);
    // selectionSort(array, SIZE);
    // printArray(array, SIZE);


   
    return 0;
}