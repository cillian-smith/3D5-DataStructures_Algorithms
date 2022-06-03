#include "t1.h"
#include <stdio.h>
#include <stdlib.h>

const int SIZE = 10;

int main()
{
    int array[SIZE];
    fill_descending(array, 10);
    printArray(array, SIZE);
    fill_ascending(array, 10);
    printArray(array, SIZE);
    fill_uniform(array, 10);
    printArray(array, SIZE);
    fill_with_duplicates(array, 10);
    printArray(array, SIZE);
    fill_without_duplicates(array, 10);
    printArray(array, SIZE);

    return 0;
}