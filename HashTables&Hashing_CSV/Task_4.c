#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>	//for isalnum

#define MAX_STRING_SIZE 20 //max length of a string
#define ARRAY_SIZE 99991   //best be prime

int num_terms = 0;
int collisions = 0;

typedef struct Element Element;
struct Element
{
	char Person_ID[MAX_STRING_SIZE];
	char Age[MAX_STRING_SIZE];
	char Surname[MAX_STRING_SIZE];
	char Deposition_ID[MAX_STRING_SIZE];
	char Forename[MAX_STRING_SIZE];
	char Person_Type[MAX_STRING_SIZE];
	char Gender[MAX_STRING_SIZE];
	char Nationality[MAX_STRING_SIZE];
	char Religion[MAX_STRING_SIZE];
	char Occupation[MAX_STRING_SIZE];
	Element *next;
};

Element *hashTable[ARRAY_SIZE];

//first hash function
int hash1(char *s)
{
	int hash = 0;
	while (*s)
	{
		hash = (hash + *s); //% ARRAY_SIZE;
		s++;
	}
	return hash;
}

//second hash function 
int hash3(char *s)
{
	int hash = 0;
	while (*s)
	{
		hash = 1 + (hash + *s) % (ARRAY_SIZE - 1);
		s++;
	}
	return hash;
}

//double hash function
int hash_function(char *s, int i)
{
	int index = (hash1(s) + i * hash3(s)) % ARRAY_SIZE;
	return index;
}

//create new element 
Element *createNewElement()
{
	Element *newElement = malloc(sizeof(Element));
	//num_terms++;
	return newElement;
}

// returns the element with name name or NULL if the element is not present
Element *search(char *surname)
{
	int hash_iteration = 0;
	int index = hash_function(surname, hash_iteration);
	if (hashTable[index] == NULL)
	{ //if index position is NULL return NULL as it is not in list
		return NULL;
	}
	else
	{
		for (;;)
		{ //loop until element is put in table
			if (strcmp(hashTable[index]->Surname, surname) == 0)
			{ //compare serached name and indexed surname
				return hashTable[index];
			}
			else
			{ //if same index but different surnames
				hash_iteration++;
				index = hash_function(surname, hash_iteration);
				if (hashTable[index] == NULL)
				{ //if index position is NULL add element
					return NULL;
				}
			}
		}
	}
}

//searches the name in the array, if it is there add to linked list, if not add it
void addOrList(Element *person)
{
	int hash_iteration = 0;
	int index = hash_function(person->Surname, hash_iteration);
	if (hashTable[index] == NULL)
	{ //if index position is NULL add element
		hashTable[index] = person;
		num_terms++;
	}
	else
	{
		for (;;)
		{ //loop until element is put in table
			if (strcmp(hashTable[index]->Surname, person->Surname) == 0)
			{									 //if two elements have the same surname and index
				person->next = hashTable[index]; // set element currently in table to pointer of current
				hashTable[index] = person;
				return;
			}
			else
			{ //if same element but different surnames
				hash_iteration++;
				collisions++;
				index = hash_function(person->Surname, hash_iteration);
				if (hashTable[index] == NULL)
				{ //if index position is NULL add element
					hashTable[index] = person;
					num_terms++;
					return;
				}
			}
		}
	}
}

//print element 
void print_element(Element *searched_element)
{
	printf("    %s    %s    %s    %s    %s   %s    %s    %s   %s   %s\n",
		   searched_element->Person_ID,
		   searched_element->Deposition_ID,
		   searched_element->Surname,
		   searched_element->Forename,
		   searched_element->Age,
		   searched_element->Person_Type,
		   searched_element->Gender,
		   searched_element->Nationality,
		   searched_element->Religion,
		   searched_element->Occupation);
}

// prints the number of occurences, or 0 if not in the file
void printNumberOfOccurences(char *name)
{
	Element *searched_element = search(name);
	if (searched_element != NULL)
	{
		printf(">>> Person ID Deposition ID Surname Forename Age Person Type Gender Nationality Religion Occupation\n");
		for (;;)
		{
			print_element(searched_element);
			if (searched_element->next == NULL)
			{
				return;
			}
			else
			{
				searched_element = searched_element->next;
			}
		}
	}
	else
	{
		printf(">>> %s not in table \n", name);
	}
}

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

//assigns each piece fo data to members of the struct 
Element *fetch_person(FILE *csv)
{
	Element *p = createNewElement();

	next_field(csv, p->Person_ID, MAX_STRING_SIZE); 
	next_field(csv, p->Deposition_ID, MAX_STRING_SIZE);
	next_field(csv, p->Surname, MAX_STRING_SIZE);
	next_field(csv, p->Forename, MAX_STRING_SIZE);
	next_field(csv, p->Age, MAX_STRING_SIZE);
	next_field(csv, p->Person_Type, MAX_STRING_SIZE);
	next_field(csv, p->Gender, MAX_STRING_SIZE);
	next_field(csv, p->Nationality, MAX_STRING_SIZE);
	next_field(csv, p->Religion, MAX_STRING_SIZE);
	next_field(csv, p->Occupation, MAX_STRING_SIZE);
	p->next = NULL; // sets next pointer to NULL

	return p;
}

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file(char *fname)
{
	FILE *f;

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f)
	{
		printf("Unable to open %s\n", fname);
		return 0;
	}
	Element *disgard = fetch_person(f);
	free(disgard); //disgard first line of header data

	// read until the end of the file
	while (!feof(f))
	{
		addOrList(fetch_person(f)); //here you call your function from above!
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

	float load = (float)num_terms/(float)ARRAY_SIZE;

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
			printNumberOfOccurences(checked_name);
		}
		else
		{
			break;
		}
	}
	printf(">>> ");

	for(int i; i< ARRAY_SIZE; i++){
			while(hashTable[i]){
				Element* next = hashTable[i]->next;
				free(hashTable[i]);
				hashTable[i] = next;
		}
	}

	return 0;
}