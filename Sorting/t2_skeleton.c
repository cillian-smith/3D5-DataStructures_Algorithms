#include <stdio.h>
#include "t2.h"

int number_comparisons = 0;
int number_swaps = 0;

//swaps
void swap(int array[], int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
    number_swaps++;
}

//returns difference between a and b
int compare(int a, int b)
{
    number_comparisons++;
    return (a - b);
}

void selectionSort(int arr[], int size)
{
    for (int i = 0; i < (size - 1); i++)
    {
        int index_smallest = i;
        for (int k = i; k < size; k++)
        {
            if (compare(arr[k], arr[index_smallest]) < 0) //if (arr[k] < arr[index_smallest])
            {
                index_smallest = k;
            }
        }
        swap(arr, index_smallest, i);
    }
}

void insertionSort(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int k = i; k > 0 && (compare(arr[k], arr[k - 1]) < 0); k--)
        { // if (arr[k] < arr[k - 1])
            swap(arr, k, k - 1);
        }
    }
}

int partitionHoare(int array[], int p, int r)
{
    if (p == r) //compare(p,r) == 0
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
            if (i == r) // compare(i,r) == 0
            {
                break;
            }
        } while (compare(array[i], pivot) < 0); //while (array[i] < pivot)

        do
        {
            j--;
            if (j == p) // compare(j,p) == 0
            {
                break;
            }
        } while (compare(array[j], pivot) > 0); //while (array[j] > pivot)

        if (i >= j) // compare(i,j) >= 0
        {
            swap(array, i, r);
            return i;
        }
        swap(array, i, j);
    }
}

void quickSortAlgorithm(int arr[], int low, int high)
{
    if (low < high) // if (low < high) //compare(low,high) < 0
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
