#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include "bstdb.h"

typedef struct Tree Tree;
struct Tree
{
	unsigned int UID;
	int word_count;
	char *name;
	Tree *left, *right;
};

Tree *root; // declaring root as a global
int g_num_books, num_searches, num_comps;

//--------------------------------------Functions-------------------------------------
//precalculated values of 2^n
const int pow_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072};

//generates a balanced bst key with root key of 2^n.
unsigned int gen_ID()
{
	static int count = 1, k = 0, prev_height = 0, n = 16;
	int height = log2(count++);					   // Taking the log of count gives the height in the tree
	k = (prev_height != height) ? 0 : k;		   // reset k if height changes
	int key = (2 * k++ + 1) * (pow_2[n - height]); // pre calculated powers of 2^n to save computational power
	prev_height = height;						   // update prev height
	return key;									   //return key
}

//Creates a branch.
Tree *create_node(char *name, int word_count, unsigned int UID)
{
	Tree *node = (Tree *)malloc(sizeof(Tree));
	if (node) //if malloc is successful
	{
		node->UID = UID;
		node->word_count = word_count;
		node->left = node->right = NULL;

		size_t name_len = strlen(name) + 1;			 // get string length
		node->name = calloc(name_len, sizeof(char)); // assign memory which is given 0 value
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
	g_num_books++; // number of books created
	return node;
}

//Inserts value to correct position to create a branch.
void insert_tree(Tree **r, char *name, int word_count, unsigned int UID)
{
	if (*r == NULL)
	{ //if tree is null, create node from data
		*r = create_node(name, word_count, UID);
		return;
	}
	if (UID < (*r)->UID) // compare value of UID
	{
		insert_tree(&(*r)->left, name, word_count, UID);
	}
	else
	{
		insert_tree(&(*r)->right, name, word_count, UID);
	}
}

//Recursively searches for UID Value.
Tree *search(Tree *root, unsigned int UID)
{
	if (root == NULL || root->UID == UID) // if root is null or IDs are equal return root
	{
		return root;
	}
	else
	{
		num_comps++;
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

//Iteritively searches for UID value.
Tree *iterativeSearch(Tree *root, int key)
{
	// Traverse until root reaches to dead end
	while (root != NULL)
	{
		// pass right subtree as new tree
		if (key > root->UID)
			root = root->right;

		// pass left subtree as new tree
		else if (key < root->UID)
			root = root->left;
		else
			return root; // if the key is found return 1
	}
	return NULL;
}

//Frees dynamically allocated memory.
void tree_delete(Tree *root)
{
	if (root == NULL)
	{
		return;
	}
	tree_delete(root->left);
	tree_delete(root->right);
	if (root->name)
	{
		free(root->name);
	}
	free(root);
}

//Returns the max of two ints.
int max(int a, int b)
{
	return (a >= b) ? a : b;
}

//Returns the height of a tree.
int height(Tree *node)
{
	if (node == NULL)
	{
		return 0;
	}
	return 1 + max(height(node->left), height(node->right));
}

//Checks if a tree is balanced.
int balanced(Tree *root)
{
	if (root == NULL)
	{
		return 1;
	}

	int height_left, height_right;
	height_left = height(root->left);
	height_right = height(root->right);

	if (abs(height_left - height_right) <= 1 && balanced(root->left) && balanced(root->right))
	{
		return 1;
	}
	return 0;
}

//Recursively checks if a tree is wihtin a valid range.
int isBounded(Tree *node, int min, int max)
{
	if (node == NULL)
	{
		return 1;
	}
	if (node->UID < min || node->UID > max)
	{
		return 0;
	}
	return isBounded(node->left, min, node->UID - 1) && isBounded(node->right, node->UID + 1, max);
}

//Checks if a tree is a valid BST.
int isValid(Tree *root)
{
	return (isBounded(root, 0, INT_MAX));
}

//-------------------------------------Given Functions---------------------------------

//Only run once at beginning of program.
int bstdb_init(void)
{
	root = NULL;
	g_num_books = 0;
	return 1;
}

//Generates a key and inserts into tree.
int bstdb_add(char *name, int word_count)
{
	unsigned int UID = gen_ID();
	insert_tree(&root, name, word_count, UID);
	if (root)
	{
		return UID;
	}
	return -1;
}

//Searchs an ID and returns the word count if it exists.
int bstdb_get_word_count(int doc_id)
{
	num_searches++;
	Tree *node = search(root, doc_id);
	if (node == NULL)
	{
		return -1;
	}
	else
	{
		return node->word_count;
	}
}

//Searchs an ID and returns the name if it exists.
char *bstdb_get_name(int doc_id)
{
	num_searches++;
	Tree *node = search(root, doc_id);
	if (node == NULL)
	{
		return NULL;
	}
	else
	{
		return node->name;
	}
	return 0;
}

//Calculates some statistics about the tree.
void bstdb_stat(void)
{
	printf("STAT\n");
	char isBST = ((isValid(root)) == 1) ? 'Y' : 'N';
	printf("Is the tree a valid BST -> %c\n", isBST);

	char isBalanced = (balanced(root) == 1) ? 'Y' : 'N';
	printf("Is the tree balanced -> %c\n", isBalanced);

	double avg_comp_per_search = (double)num_comps / (double)num_searches;
	printf("Average Comparisons per search -> %4.2f\n", avg_comp_per_search);

	double capacity = 100 * ((double)g_num_books / (2 * pow_2[16]));
	printf("The tree is at %4.2f %% capacity.\n", capacity);

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

//Frees the dynamically allocated memory
void bstdb_quit(void)
{
	tree_delete(root);
}
