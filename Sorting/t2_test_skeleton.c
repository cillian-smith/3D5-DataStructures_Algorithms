#include "t1.h"
#include "t2.h"
#include <stdio.h>

const int SIZE = 10;

int main()
{
    int array[SIZE];

    fill_without_duplicates(array, SIZE);
    printArray(array, SIZE);
    quickSort(array,SIZE);
    printArray(array, SIZE);
 

return 0;
}