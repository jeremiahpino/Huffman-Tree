/* table of encodings for given file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* One node */
typedef struct Node
{
    int count;
    int character;
    char codes[10000];
    struct Node* next;
    struct Node* left;
    struct Node* right;
}Node;

/* Function Prototypes */
int compare(const void* a, const void* b);
int isLeaf(Node* root);
Node** getResults(Node* root, char string[], Node** nodes, int top);
void fillArray(int arr[], FILE* file);
void printList(Node* head);
Node* newNode(int count, int character);
Node* sortedInsert(Node* head, int count, int character);
Node* treeInsert(Node* head, Node* newNode);
Node* merge(Node* first, Node* second);

int main (int argc, char* argv[])
{
    /* declare variables */
    Node* head = NULL;
    FILE* fPointer = NULL; 
    int charArray[256] = {0};
    Node* first = NULL;
    Node* second = NULL;
    Node* addBack = NULL;
    int b = 0;
    int j = 0;
    char string[10]; 
    Node** nodes = malloc(256 * sizeof(Node*));
    int size;

    /* if argc is greater than 1 read file into fPointer */
    if(argc > 1)
    {
        fPointer = fopen(argv[1], "r");
	
	/* check if file is empty */
	if(fPointer != NULL)
	{
	 fseek(fPointer, 0, SEEK_END);
	 size = ftell(fPointer);
	 if(size == 0)
	 {
	  exit(EXIT_FAILURE);
	 }
	}
	fseek(fPointer, 0, SEEK_SET);
    }
    else
    {
        fPointer = stdin;
    }
    
    /* fPointer is NULL EXIT */
    if(fPointer == NULL)
    {
        perror("No FILE\n");
        exit(EXIT_FAILURE);
    }

    /* fill array with characters in file */
    fillArray(charArray, fPointer);

    /* iteratre and create newNodes if count is greater than 0 */
    for(; j < 256; j++)
    {
        if(charArray[j] > 0)
        {
            if(head == NULL)
            {
                head = newNode(charArray[j], j);
            }
            else
            {
                head = sortedInsert(head, charArray[j], j);
            }
        }
    }

    /* create Huffman Tree */
    /* check if head and head next is not NULL proceed */
    while(head != NULL && head->next != NULL)
    {
        first = head;
        second = head->next;

        /* increment head two nodes over */
        head = head->next->next;
        
        /* create new node from first and second node */
        addBack = merge(first, second);
        if(head == NULL)
        {
            head = addBack;
            break;
        }
        head = treeInsert(head, addBack);
    }

    /* get Huffman codes and put in array of Nodes */
    nodes = getResults(head, string, nodes, 0);

    /* sort codes by alphabetical order */   
    qsort(nodes, 256, sizeof(nodes[0]), compare);
    
    /* print codes */
    for(; b < 256; b++)
    {
        if(nodes[b] != NULL)
        {
            printf("0x%02x: %s\n", nodes[b]->character, nodes[b]->codes);
        }
    }
    
    fclose(fPointer);

    return 0;
}

/* compare function to sort in alphabetic order */
int compare(const void* a, const void* b)
{
    const Node* first = *(Node**)a;
    const Node* second = *(Node**)b;
    
    if(!first) return 1;
    if(!second) return 1;
    return (first->character) - (second->character);
}

/* determine if node is a leaf */
int isLeaf(Node* root)
{
    return !(root->left) && !(root->right);
}

/* get the Huffman codes and store in each node */
Node** getResults(Node* root, char string[], Node** nodes, int top)
{
    static int level = 0;
    /* iterate through root left and right nodes until leaf */
    if(root->left)
    {
        string[top] = '0';
        getResults(root->left, string, nodes, top + 1);
    }
    if(root->right)
    {
        string[top] = '1';
        getResults(root->right, string, nodes, top + 1);
    }
    if(isLeaf(root))
    {
        string[top] = '\0';
        strcpy(root->codes, string);
        /* assign root to nodes */
        nodes[level++] = root;
    }
    return nodes;
} 

/* fill array from file */
void fillArray(int arr[], FILE* file)
{
    int c = 0;

    while((c = getc(file)) != EOF)
    {
        /* index is the character and total count is number in array */
        arr[c]++;
    }
}

/* print Linked List */
void printList(Node* head)
{
    Node* start = head;
    while(start != NULL)
    {
        printf("Character: %d and Count: %d\n", start->character, start->count);
        start = start->next;
    }
}

/* create a new Node */
Node* newNode(int count, int character)
{
    Node* newNode = (Node*)malloc(sizeof(newNode));
    newNode->count = count;
    newNode->character = character;
    newNode->next = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* Build Huffman Tree Function */
Node* treeInsert(Node* head, Node* newNode)
{
    Node* current = NULL;
    
    /* replace head if NULL or newNode count is less than */    
    if(head == NULL || newNode->count < head->count)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        current = head;
        /* place new node in order of count */
        while(current->next != NULL && current->next->count < newNode->count)
        {
            current = current->next;
        }
        /* assign newNode next pointer with current next pointer address */
        newNode->next = current->next;

        /* assign current next pointer with newNode address */
        current->next = newNode;
    }
    return head;
}

/* Sorted insert into Linked List */
Node* sortedInsert(Node* head, int count, int character)
{
    Node* current = NULL;
    Node* newP = NULL;

    newP = newNode(count, character);
     
    if(head == NULL || count < head->count)
    {
        newP->next = head;
        head = newP;
    }
    else
    {
        current = head;
        /* place new node in order of count */
        while(current->next != NULL && current->next->count <= count)
        {
            current = current->next;
        }
        /* newNode next pointer holds current next address */
        newP->next = current->next;

        /* current next pointer holds newP address */
        current->next = newP;
    }
    return head;
}
       
/* merge two nodes into one node */
Node* merge(Node* first, Node* second)
{
    Node* newP = NULL;

    if(first == NULL || second == NULL) return NULL;
    first->next = NULL;
    second->next = NULL;
    
    /* create new Node to return */
    newP = (Node*)malloc(sizeof(Node));
    newP->left = first;
    newP->right = second;

    /* new node count is first + second counts */
    newP->count = first->count + second->count;
    return newP;
}
    
