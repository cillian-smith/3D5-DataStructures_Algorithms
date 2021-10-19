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

// original hash function 
int hash_function(char* s){ 
    int hash = 0;
    while(*s){
        hash = (hash + *s);//% ARRAY_SIZE;
        s++;
    }
    return hash;
}

//provided hash function
int hash3(char* s){
    int hash = 0;
    while(*s){
    hash = 1+ (hash + *s) % (ARRAY_SIZE-1);
    s++;
    }
    return hash;
}

//joined double hash function 
int hash3_joined(char* s, int i){
    int index = (hash_function(s) + i*hash3(s)) % ARRAY_SIZE;
        return index;
}

//create new element 
Element* createNewElement(char* name){
	Element* temp = (Element*)malloc(sizeof(Element));
	temp->key = NULL;
	temp->count = 0;

	temp->key = strdup(name);
	temp->count++;
	num_terms++;
    
    return temp;    
}


// returns the element with name name or NULL if the element is not present
Element* search (char* name){
    int i =0;
    int index = (hash_function(name) % ARRAY_SIZE);
	if(hashTable[index] == NULL){
		return NULL;
	}else{
		for(;;){
			if(hashTable[index] != NULL){
				if(strcmp(hashTable[index]->key, name) == 0){
					return hashTable[index];
				}else{
					i++;
                    index = hash3_joined(name, i);
				}
			}else{
				return NULL;
			}
		}
	}
}

//insert assuming no element at index 
void insert(char* name){
    int i =0;
   int index = hash3_joined(name, i);
   hashTable[index] = createNewElement(name);
}

//insert at specific index
void insert_index(char* name, int index){
   hashTable[index] = createNewElement(name);
}

//searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char* name){
    int i = 0;
	int index = (hash_function(name) % ARRAY_SIZE);
	if(hashTable[index] == NULL){
		insert_index(name, index);
	}else{
		while(hashTable[index] != NULL){
			if(strcmp(hashTable[index]->key, name) == 0){
				hashTable[index]->count++;
				return;
			}else{
                i++;
                index = hash3_joined(name, i);
                if(hashTable[index]==NULL){
                    insert_index(name, index);
                    return;
                }else{collisions++;}
             }
		}
	}
}


// prints the number of occurences, or 0 if not in the file
void printNumberOfOccurences(char* name){
    int count=0;
	Element* searched_element = search(name);
	if(searched_element != NULL){
		name = strdup(searched_element->key);
		count = searched_element->count;
		printf(">>> %s - %i \n", name, count);
	}else{
		printf(">>> %s - 0 \n", name);
	}

}

// function from the assignment 0
// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
void next_token ( char *buf, FILE *f, int string_max ) {
	// start by skipping any characters we're not interested in
	buf[0] = fgetc(f);
	while ( !isalnum(buf[0]) && !feof(f) ) { buf[0] = fgetc(f); }
	// read string of alphanumeric characters
	int i=1;
	for (;;) {
		buf[i] = fgetc(f);                // get next character from file
		if( !isalnum(buf[i]) ) { break; } // only load letters and numbers
		if( feof(f) ) { break; }          // file ended?
		if( i < (string_max-1) ) { ++i; } // truncate strings that are too long
	}
	buf[i] = '\0'; // NULL terminate the string
}


//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file ( char *fname ) {
	FILE *f;
	char buf[MAX_STRING_SIZE];

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
	
	// read until the end of the file
	while ( !feof(f) ) {
		next_token(buf, f, MAX_STRING_SIZE);
		addOrIncrement( buf);                           //here you call your function from above!
	}

	// always remember to close your file stream
	fclose(f);

	return 1;
}

int main( int argc, char * argv[]){
	char checked_name[21];
	char quit_token[21] = "quit";

    if(load_file(argv[1]) == 0){
        perror("Exited error 1, file could not open");
    }

	double load = (double)num_terms/59;

	printf("File %s loaded\n", argv[1]);
	printf(" Capacity: %d\n", ARRAY_SIZE);
	printf(" Num Terms: %d\n", num_terms);
	printf(" Collisions: %d\n", collisions);
	printf(" Load: %f\n", load);

	printf("Enter term to get frequency or type \"quit\" to escape\n");
	for(;;){//search loop for name 
		scanf("%21s", checked_name);
		if(strcmp(quit_token, checked_name)!=0){
		printNumberOfOccurences(checked_name);
		}else{break;}

	}
	printf(">>> ");
	
	return 0;
}