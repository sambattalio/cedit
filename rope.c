#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// rope test

typedef struct Rope Rope;

struct Rope{
    Rope* left;
    Rope* right;
    char* data;
    int weight;
    int length;
};

const int MAX_SIZE = 2;

// recursively creates rope data structure based on string
void rope_create(Rope *node, char* string, int l, int r) {
    if (!node) return;

    // can split... need an empty node to hold 2 more nodes
    if ( (r - l) + 1 > MAX_SIZE ) {
        // basically anything to the left of it is the weight
        node->weight = (r - l) / 2 + 1; // left side gets truncated half of it 
        // split divide by two and yeet to different nodes
        node->left  = (Rope*) calloc(1, sizeof(Rope));
        rope_create(node->left, string, l, (l + r) / 2);
        node->right = (Rope*) calloc(1, sizeof(Rope));
        rope_create(node->right, string, (l + r) / 2 + 1, r);
        node->length = r - l + 1;
    } else {
        // populate node with the text
        node->data = (char*) calloc(r - l, sizeof(char)); // allocate needed

        for (int i = 0; i <= r - l; i++) {
            node->data[i] = string[l + i];
        }
        
        node->weight = r - l + 1; // size of string is weight
        node->left  = NULL;
        node->right = NULL;
        node->length = node->weight;
    }
}

char* rope_index(Rope *node, int i) {
    // if to the right, goto and lower search i by weight
    if (node->weight <= i && node->right)
        return rope_index(node->right, i - node->weight);
    
    // otherwise descend to the left child if exists.
    // Don't subtract weight b/c its same
    if (node->left)
        return rope_index(node->left, i);
    
    // otherwise this is a leaf
    return node->data;
}

void rope_delete(Rope *node) {
    if (!node) return;

    rope_delete(node->left);
    rope_delete(node->right);

    free(node->data);
    free(node);    
}

Rope* rope_concat(Rope *node_a, Rope* node_b) {
    if(!node_a || !node_b) return NULL;
    
    Rope* new_parent = (Rope*) calloc(1, sizeof(Rope));

    new_parent->length = node_a->length + node_b->length;
    new_parent->weight = node_a->length;
    new_parent->left = node_a;
    new_parent->right = node_b;

    return new_parent;
}

void rope_print(Rope *node) {
    if (!node) return;

    rope_print(node->left);
    rope_print(node->right);
    
    if (node->data) printf("%s", node->data);
}

void postorder_print(Rope *node) {
    if (!node) return;

    postorder_print(node->left);
    
    printf("node weight: %d ", node->weight);
    if (node->data) printf("%s", node->data);
    printf("\n");
    postorder_print(node->right);
}

int main () {
    char* test_string = "Hello World";
    char* test2_string = "! Beast";
    Rope* root = (Rope*) calloc(1, sizeof(Rope));
    Rope* new_root = (Rope*) calloc(1, sizeof(Rope));
    rope_create(root, test_string, 0, strlen(test_string) - 1);
    rope_create(new_root, test2_string, 0, strlen(test2_string) - 1);

    root = rope_concat(root, new_root);

    printf("traversing...\n"); 
    postorder_print(root);
    printf("\nlooping through index func\n");
    for (int i = 0; i < strlen(test_string) + strlen(test2_string); i++) 
        printf("%d %s\n",i, rope_index(root, i));
   
    rope_print(root);
    printf("\n");

    return 0;
}
