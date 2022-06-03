#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
typedef struct book_node book_node;
struct book_node
{
    int wordcount;
    char *title;
    int id;
    book_node *left;
    book_node *right;
    int height; //distance to leaf from node
};

book_node *tree_root;
int counter;

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(book_node *book)
{
    if (book == NULL)
    {
        return -1;
    }
    return book->height;
}

int generate_id()
{
    static int i = 1;
    return i++;
}

book_node *create_node(char title[], int wordcount, int id)
{
    book_node *new_node = (book_node *)malloc(sizeof(book_node));
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->wordcount = wordcount;
    new_node->id = id;
    new_node->height = 1;

    new_node->title = calloc(strlen(title) + 1, sizeof(char));
    if (new_node->title)
    {
        strcpy(new_node->title, title);
    }
    else
    {
        book_node *delete = (book_node *)malloc(sizeof(book_node));
        delete = new_node;
        free(delete);
        new_node = NULL;
    }

    return new_node;
}

book_node *right_rotate(book_node *book)
{ //rotates the given node
    book_node *x = book->left;
    book_node *y = x->right;

    // Perform rotation
    x->right = book;
    book->left = y;

    // Update heights
    book->height = max(height(book->left), height(book->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

book_node *left_rotate(book_node *book)
{
    book_node *y = book->right;
    book_node *x = y->left;

    // Perform rotation
    y->left = book;
    book->right = x;

    //  Update heights
    book->height = max(height(book->left), height(book->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

int get_balance(book_node *book)
{
    if (book == NULL)
    {
        return 0;
    }
    return height(book->left) - height(book->right);
}

void tree_insert(book_node **root, char name[], int wordcount, int id)
{
    if (*root == NULL)
    {
        *root = create_node(name, wordcount, id);
        counter++;
        return;
    }
    if ((*root)->id > id)
    {
        tree_insert(&(*root)->left, name, wordcount, id);
        return;
    }
    else
    { //new data is less or equal to current branch
        tree_insert(&(*root)->right, name, wordcount, id);
        return; //keeping current node as parent of new one
    }

    (*root)->height = 1 + max((*root)->left->height, (*root)->right->height);

    int balance = get_balance(*root);

    //four cases:
    //	left heavy with subtree which is left heavy (left left)
    //	left heavy with subtree which is right heavy (left right)
    //	right heavy with subtree which is left heavy (right left)
    //	right heavy with subtree which is right heavy (right left)

    //left left:
    if (balance > 1 && id < (*root)->left->id)
    {
        *root = right_rotate(*root);
    }
    // Right Right
    if (balance < -1 && id > (*root)->right->id)
    {
        *root = left_rotate(*root);
    }
    // Left Right
    if (balance > 1 && id > (*root)->left->id)
    {
        (*root)->left = left_rotate((*root)->left);
        *root = right_rotate(*root);
    }

    // Right Left
    if (balance < -1 && id < (*root)->right->id)
    {
        (*root)->right = right_rotate((*root)->right);
        *root = left_rotate(*root);
    }
}

int bstdb_init(void)
{
    tree_root = NULL;
    counter = 0;
    return 1;
}

int bstdb_add(char *name, int word_count)
{
    int book_id = generate_id();
    tree_insert(&tree_root, name, word_count, book_id);
    if (tree_root)
    {
        return book_id;
    }
    // If something goes wrong and the data cannot be stored, this function
    // should return -1. Otherwise it should return the ID of the new node
    return -1;
}

book_node *search_tree(book_node *root, int doc_id)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (doc_id == root->id)
    {
        return root;
    }
    else if (doc_id < root->id)
    {
        return search_tree(root->left, doc_id);
    }
    else
    {
        return search_tree(root->right, doc_id);
    }

    return NULL;
}

int bstdb_get_word_count(int doc_id)
{
    book_node *book = search_tree(tree_root, doc_id);
    if (book == NULL)
    {
        return -1;
    }
    else
    {
        return book->wordcount;
    }
}

char *bstdb_get_name(int doc_id)
{
    book_node *book = search_tree(tree_root, doc_id);
    if (book == NULL)
    {
        return 0;
    }
    else
    {
        return book->title;
    }
}

int get_count(book_node *root)
{
    static int tree_counter = 0;
    if (root == NULL)
    {
        return counter;
    }

    tree_counter += get_count(root->left);
    tree_counter++;
    tree_counter += get_count(root->right);
    return tree_counter;
}

void bstdb_stat(void)
{
    printf("STAT\n");
    int tree_counter = get_count(tree_root);
    if (counter == tree_counter)
    {
        printf("Correct number of nodes in tree.\n");
    }
    else
    {
        printf("Incorrect number of nodes. %d %d", tree_counter, counter);
    }
}

void tree_delete(book_node *root)
{
    if (tree_root == NULL)
    {
        return;
    }
    tree_delete(tree_root->left);
    tree_delete(tree_root->right);
    free(tree_root);
}

void bstdb_quit(void)
{
    // This function will run once (and only once) when the program ends. Use
    // it to free any memory you allocated in the course of operating the
    // database.
    tree_delete(tree_root);
}

void tree_print_sorted(book_node *root)
{
    if (root == NULL)
    {
        return;
    }
    tree_print_sorted(root->left);
    printf("%d - ", root->id);
    tree_print_sorted(root->right);
}

int main()
{
    bstdb_init();

    int arr[22];
    arr[0] = bstdb_add("Frick ya chicken strips", 1625);
    arr[1] = bstdb_add("Live laugh love", 1342);
    arr[2] = bstdb_add("Minecraft", 1866);
    arr[3] = bstdb_add("Road works ahead", 1225);
    arr[4] = bstdb_add("Sure hope it does", 1377);
    arr[5] = bstdb_add("ROLF", 1066);
    arr[6] = bstdb_add("Frick ya chicken strips", 1625);
    arr[7] = bstdb_add("Live laugh love", 1342);
    arr[8] = bstdb_add("Minecraft", 1866);
    arr[9] = bstdb_add("Road works ahead", 1225);
    arr[10] = bstdb_add("Sure hope it does", 1377);
    arr[11] = bstdb_add("ROLF", 1066);
    arr[12] = bstdb_add("Frick ya chicken strips", 1625);
    arr[13] = bstdb_add("Live laugh love", 1342);
    arr[14] = bstdb_add("Minecraft", 1866);
    arr[15] = bstdb_add("Road works ahead", 1225);
    arr[16] = bstdb_add("Sure hope it does", 1377);
    arr[17] = bstdb_add("ROLF", 1066);
    arr[18] = bstdb_add("ROLF", 1066);
    arr[19] = bstdb_add("ROLF", 1066);
    arr[20] = bstdb_add("ROLF", 1066);
    arr[21] = bstdb_add("ROLF", 1066);

    for (int i = 0; i < 18; i++)
    {
        printf("%s\n", bstdb_get_name(arr[i]));
    }
    printf("\n");
    for (int i = 0; i < 18; i++)
    {
        printf("%d\n", bstdb_get_word_count(arr[i]));
    }

    tree_print_sorted(tree_root);
    printf("\n\n");

    return 0;
}
