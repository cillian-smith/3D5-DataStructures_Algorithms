#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum

#define MAX_STRING_SIZE 30 //max length of a string
#define ARRAY_SIZE 18625

struct game
{ //title,platform,Score,release_year
    char title[MAX_STRING_SIZE], platform[MAX_STRING_SIZE];
    int score, release_year;
};

struct game *gameArray[ARRAY_SIZE];

// The CSV parser
int next_field(FILE *f, char *buf, int max)
{
    int i = 0, end = 0, quoted = 0;

    for (;;)
    {
        // fetch the next character from file
        buf[i] = fgetc(f);
        // if we encounter quotes then flip our state and immediately fetch next char
        if (buf[i] == '"')
        {
            quoted = !quoted;
            buf[i] = fgetc(f);
        }
        // end of field on comma if we're not inside quotes
        if (buf[i] == ',' && !quoted)
        {
            break;
        }
        // end record on newline or end of file
        if (feof(f) || buf[i] == '\n')
        {
            end = 1;
            break;
        }
        // truncate fields that would overflow the buffer
        if (i < max - 1)
        {
            ++i;
        }
    }

    buf[i] = 0; // null terminate the string
    return end; // flag stating whether or not this is end of the line
}

// Stuff to make life a bit neater in main
struct game *fetch_game(FILE *csv)
{
    char buf[MAX_STRING_SIZE];

    struct game *p = malloc(sizeof(struct game));

    next_field(csv, p->title, MAX_STRING_SIZE);
    next_field(csv, p->platform, MAX_STRING_SIZE);
    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->score = atoi(buf);
    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->release_year = atoi(buf);

    return p;
}

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file(char *fname)
{
    FILE *f;
    int i = 0;

    // boiler plate code to ensure we can open the file
    f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }
    fetch_game(f);

    // read until the end of the file
    while (!feof(f))
    {
        gameArray[i++] = fetch_game(f);
    }

    // always remember to close your file stream
    fclose(f);

    return 1;
}

//----------------------------------------------------------
void swap(struct game *array[], int i, int j)
{
    struct game *temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
int compare(int a, int b){ 
    //number_comparisons++;
    return (a-b);
}

void selectionSort(struct game *arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        int index_smallest = i;
        for (int k = i; k < size; k++)
        {
            if (arr[k]->score < arr[index_smallest]->score)
            {
                index_smallest = k;
            }
        }
        swap(arr, index_smallest, i);
    }
}

int partitionHoare(struct game *array[], int p, int r)
{
    if (p == r) //compare(p,r) == 0
    {
        return p;
    }

    int pivot = array[r]->score;
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
        } while (compare(array[i]->score, pivot) < 0); //while (array[i] < pivot)

        do
        {
            j--;
            if (j == p) // compare(j,p) == 0
            {
                break;
            }
        } while (compare(array[j]->score, pivot) > 0); //while (array[j] > pivot)

        if (i >= j) // compare(i,j) >= 0
        {
            swap(array, i, r);
            return i;
        }
        swap(array, i, j);
    }
}

void quickSortAlgorithm(struct game* arr[], int low, int high)
{
    if (low < high) // if (low < high) //compare(low,high) < 0
    {
        int q = partitionHoare(arr, low, high);
        quickSortAlgorithm(arr, low, q - 1);
        quickSortAlgorithm(arr, q + 1, high);
    }
}

void quickSort(struct game* arr[], int size)
{
    quickSortAlgorithm(arr, 0, size - 1);
}

//----------------------------------------------------------
int main(int argc, char *argv[])
{
    if (load_file(argv[1]) == 0)
    {
        perror("Exited error 1, file could not open");
    }

    //selectionSort(gameArray, ARRAY_SIZE);
    quickSort(gameArray, ARRAY_SIZE);
    int n = 20;       // column width
    int n_first = 30; //first column width
    for (int i = (ARRAY_SIZE - 1); i > ARRAY_SIZE - 11; i--)
    {
        printf("Game: %*s    -   Score: %*d    -   Platform: %*s   -   Year: %*d\n", n_first, gameArray[i]->title, 5, gameArray[i]->score, n, gameArray[i]->platform, n, gameArray[i]->release_year);
    }

    return 0;
}