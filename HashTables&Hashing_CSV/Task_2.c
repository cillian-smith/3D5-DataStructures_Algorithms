#include<stdio.h>
#include<stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum


#define MAX_STRING_SIZE 20 //max length of a string
#define ARRAY_SIZE 59  //best be prime

int num_terms = 0;
int collisions = 0;


typedef struct Element Element;
struct Element{
    
    char* key;
    int count;
};

Element* hashTable[ARRAY_SIZE];

unsigned long hash_function(char *str){ // hash 2
    unsigned long hash = 5381; // explanation of 5381 and 33 can be found in report 
    int c;

    while (*str) {
    c = *str;
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    str++;
    }
    c = *str;

return hash % ARRAY_SIZE;
}

Element *createNewElement(char *name) // Assigns mem and initialises key and count
{
	Element *temp = (Element *)malloc(sizeof(Element));
	temp->key = NULL;
	temp->count = 0;

	temp->key = strdup(name);
	temp->count++;
	num_terms++;

	return temp;
}

// returns the element with name name or NULL if the element is not present
Element *search(char *name)
{
	int index = hash_function(name);
	if (hashTable[index] == NULL)	//element at that index is null, name is not in list 
	{
		return NULL;
	}
	else
	{
		for (;;)
		{
			if (hashTable[index] != NULL) 	//if there is an element at index
			{
				if (strcmp(hashTable[index]->key, name) == 0) 	//compare string and return element if the equal
				{
					return hashTable[index];
				}
				else
				{
					if (index == (ARRAY_SIZE - 1))
					{
						index = -1;
					}
					index++; //resets the index to 0 if it goes above ARRAY_SIZE
				}
			}
			else // if hash table index = NULL return NULL
			{
				return NULL;
			}
		}
	}
}

//insert assuming no element at index
void insert(char *name) 	
{
	int index = hash_function(name);
	hashTable[index] = createNewElement(name);
}

//insert at given index
void insert_index(char *name, int index) 	
{
	hashTable[index] = createNewElement(name);
}

//searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char *name)		
{
	int index = hash_function(name);
	if (hashTable[index] == NULL) 	//if Element at index is NULL insert 
	{
		insert(name);
	}
	else
	{
		collisions++;
		while (hashTable[index] != NULL)
		{
			if (strcmp(hashTable[index]->key, name) == 0) // compare strings and increment if equal 
			{
				hashTable[index]->count++;
				return;
			}
			else
			{
				if (index == (ARRAY_SIZE - 1))
				{
					index = -1; // reset indxe to 0 if equal to ARRAY_SIZE
				}
				index++;
				if (hashTable[index] == NULL)
				{
					insert_index(name, index); //insert at gievn index
					return;
				}
				else
				{
					//collisions++; // counts collisions when it encounters a element of different key at same index
				}
			}
		}
	}
}

// prints the number of occurences, or 0 if not in the file
void printNumberOfOccurences(char *name)
{
	int count = 0;
	Element *searched_element = search(name);
	if (searched_element != NULL)
	{
		name = strdup(searched_element->key);
		count = searched_element->count;
		printf(">>> %s - %i \n", name, count);
	}
	else
	{
		printf(">>> %s - 0 \n", name);
	}
}

// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
void next_token(char *buf, FILE *f, int string_max)
{
	// start by skipping any characters we're not interested in
	buf[0] = fgetc(f);
	while (!isalnum(buf[0]) && !feof(f))
	{
		buf[0] = fgetc(f);
	}
	// read string of alphanumeric characters
	int i = 1;
	for (;;)
	{
		buf[i] = fgetc(f); // get next character from file
		if (!isalnum(buf[i]))
		{
			break;
		} // only load letters and numbers
		if (feof(f))
		{
			break;
		} // file ended?
		if (i < (string_max - 1))
		{
			++i;
		} // truncate strings that are too long
	}
	buf[i] = '\0'; // NULL terminate the string
}

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file(char *fname)
{
	FILE *f;
	char buf[MAX_STRING_SIZE];

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f)
	{
		printf("Unable to open %s\n", fname);
		return 0;
	}

	// read until the end of the file
	while (!feof(f))
	{
		next_token(buf, f, MAX_STRING_SIZE);
		addOrIncrement(buf); //here you call your function from above!
	}

	// always remember to close your file stream
	fclose(f);

	return 1;
}

int main(int argc, char *argv[])
{
	char checked_name[21];
	char quit_token[21] = "quit";

	if (load_file(argv[1]) == 0)
	{
		perror("Exited error 1, file could not open");
	}

	double load = (double)num_terms / ARRAY_SIZE;

	printf("File %s loaded\n", argv[1]);
	printf(" Capacity: %d\n", ARRAY_SIZE);
	printf(" Num Terms: %d\n", num_terms);
	printf(" Collisions: %d\n", collisions);
	printf(" Load: %f\n", load);

	printf("Enter term to get frequency or type \"quit\" to escape\n");
	for (;;)
	{
		scanf("%21s", checked_name);
		if (strcmp(quit_token, checked_name) != 0)
		{
			printNumberOfOccurences(checked_name); // print number of occurences and name of searched 
		}
		else
		{
			break;
		}
	}
	printf(">>> ");

	return 0;
}