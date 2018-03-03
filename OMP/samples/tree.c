#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct node
{
	int data;
	struct node* left;
	struct node* right;
};

struct node* newNode(int data)
{
	struct node* node = (struct node*)
	malloc(sizeof(struct node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return(node);
}

void printPostorder(struct node *p)
{
// first recur on left subtree
#pragma omp task 
if(p->left)
	printPostorder(p->left);

// then recur on right subtree
#pragma omp task  
if(p->right)	
	printPostorder(p->right);

// now deal with the node p
#pragma omp taskwait
	printf("%d \n",p->data);
//#pragma omp barrier
}

void printInorder(struct node* node)
{
	if (node == NULL)
	return;

/* first recur on left child */
#pragma omp task 
	printInorder(node->left);

/* then print the data of node */
#pragma omp taskwait
	printf("%d ", node->data); 

/* now recur on right child */
#pragma omp task 
	printInorder(node->right);
}

	void printPreorder(struct node* node)
{
	if (node == NULL)
	return;

/* first print data of node */
	printf("%d ", node->data); 

/* then recur on left sutree */
	printPreorder(node->left); 

/* now recur on right subtree */
	printPreorder(node->right);
} 

int main()
{
#ifdef _OPENMP
	omp_set_num_threads(4);
#endif
	struct node *root = newNode(1);
	root->left		 = newNode(2);
	root->right		 = newNode(3);
	root->left->left	 = newNode(4);
	root->left->right 	 = newNode(5); 
//	root->left->right->left 	 = newNode(6); 
//	root->left->right->right 	 = newNode(7); 

//	printf("\n Preorder traversal of binary tree is \n");
//	printPreorder(root);

#pragma omp parallel 
#pragma omp single
//	printf("\n Inorder traversal of binary tree is \n");
//	printInorder(root); 
	printf("\n Postorder traversal of binary tree is \n");
	printPostorder(root);

	return 0;
}

