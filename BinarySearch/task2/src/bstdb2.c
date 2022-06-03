#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bstdb.h"

// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.

typedef struct Tree Tree;
struct Tree
{
	unsigned int UID;
	int word_count;
	char *name;
	Tree *left, *right;
};

Tree *root; // declaring root as a global

//________________________________________________________________________________________
//precalculated values of 2^n
const int pow_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072};

//generates a balanced bst key with root key of 2^n.
unsigned int gen_ID()
{
	static int count = 1, k = 0, prev_height = 0, n = 17;
	int height = log2(count++);
	k = (prev_height != height) ? 0 : k;
	int key = (2 * k++ + 1) * (pow_2[n - height]); // pre calculated powers of 2^n to save computational power
	prev_height = height;
	return key;
}

Tree *create_node(char *name, int word_count, unsigned int UID)
{
	Tree *node = (Tree *)malloc(sizeof(Tree));
	if (node)
	{
		node->UID = UID;
		node->word_count = word_count;
		node->left = node->right = NULL;

		size_t name_len = strlen(name) + 1;
		node->name = calloc(name_len, sizeof(char));
		if (node->name)
		{
			strcpy(node->name, name);
		}
		else
		{ // safety if calloc fails
			Tree *del = (Tree *)malloc(sizeof(Tree));
			del = node;
			free(del);
			return NULL;
		}
	}
	return node;
}

Tree *insert_tree(Tree *r, char *name, int word_count, unsigned int UID)
{
	if (r == NULL)
	{
		r = create_node(name, word_count, UID);
		return r;
	}

	if (UID < r->UID)
	{
		r->left = insert_tree(r->left, name, word_count, UID);
	}
	else
	{
		r->right = insert_tree(r->right, name, word_count, UID);
	}

	return r;
}

Tree *search(Tree *root, unsigned int UID)
{
	if (root == NULL || root->UID == UID)
	{
		return root;
	}
	else
	{
		if (UID < root->UID)
		{
			return search(root->left, UID);
		}
		else
		{
			return search(root->right, UID);
		}
	}
	return NULL;
}

void tree_delete(Tree *root)
{
	if (root == NULL)
	{
		return;
	}
	tree_delete(root->left);
	tree_delete(root->right);
	free(root);
}
//________________________________________________________________________________________

int bstdb_init(void)
{
	root = NULL;
	return 1;
}

int bstdb_add(char *name, int word_count)
{
	unsigned int UID = gen_ID();
	root = insert_tree(root, name, word_count, UID);
	if (root)
	{
		return UID;
	}
	return -1;
}

int bstdb_get_word_count(int doc_id)
{
	Tree *node = search(root, doc_id);
	if (node == NULL)
	{
		return -1;
	}
	else
	{
		return node->word_count;
	}

	// This is a search function. It should traverse the binary search tree
	// and return the word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1
}

char *
bstdb_get_name(int doc_id)
{
	Tree *node = search(root, doc_id);
	if (node == NULL)
	{
		return NULL;
	}
	else
	{
		return node->name;
	}
	// This is a search function. It should traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0
	return 0;
}

void bstdb_stat(void)
{
	// Use this function to show off! It will be called once after the
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//
	//  + Check if your tree is balanced and print the result
	//
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result?
	//
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?
}

void bstdb_quit(void)
{
	tree_delete(root);
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.
}

//________________________________________________________________________________________

// Total Inserts                :       110462
// Num Insert Errors            :            0
// Avg Insert Time              :   0.000006 s
// Var Insert Time              :   0.000003 s
// Total Insert Time            :   1.130574 s
//
// Total Title Searches         :        11046
// Num Title Search Errors      :            0
// Avg Title Search Time        :   0.000007 s
// Var Title Search Time        :   0.000000 s
// Total Title Search Time      :   0.131677 s
//
// Total Word Count Searches    :        11046
// Num Word Count Search Errors :            0
// Avg Word Count Search Time   :   0.000007 s
// Var Word Count Search Time   :   0.000000 s
// Total Word Count Search Time :   0.123568 s
//
// Press Enter to quit...