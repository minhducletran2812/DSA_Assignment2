#ifndef CACHE_H
#define CACHE_H

#include "main.h"
#define QSIZE 65536
//Check mem leak



//
class Node {
public:
	Node* left;
	Node* right;
	int height;
	Elem* elem = new Elem(0, 0, false);
	~Node() {
		left = right = NULL;
		delete elem;
	}
};
class AQueue
{
public:
	Elem* Queue[QSIZE]; //pointer to pointer 
	int front = -1;
	int rear = -1;

//	~AQueue() {
//		delete Queue;
//	};
	bool isempty()
	{
		if (front == -1 && rear == -1)
			return true;
		else return false;
	}
	bool isfull()
	{
		if (rear-front == MAXSIZE -1)
			return true;
		else return false;
	}
	void enqueue(Elem* value)
	{
		//if (isfull()) { ; }
		//else
		//{
			if (front == -1) front = 0;
			rear++;
			Queue[rear] = value;
		//}

	}

	Elem* dequeue2()
	{
		if (isempty()) return NULL;
		else if (front == rear)
		{
			int temp = front;
			front = rear - 1;
			return Queue[temp];
		}
		else
		{
			front++;
			return Queue[front - 1];
		}
	}
	bool findele(int addr)
	{
		for (int i = front; i <= rear; i++)
		{
			if (Queue[i]->addr == addr)
			{
				return true;
			}
		} return false;
	}
	void replacele(int addr, Data* cont)
	{
		for (int i = front; i <= rear; i++)
		{
			if (Queue[i]->addr == addr)
			{
				Queue[i]->data = cont;
				Queue[i]->sync = false;
			}
		} return ;
	}
};

//
class Cache {
		Elem** arr;
		int p;
		Node* root;
		AQueue* queue;
	public:
		Cache(int s) {
			arr = new Elem*[s];
			p = 0;
			root = NULL;
			queue = new AQueue();
		}
		~Cache() {
			delete[] arr;
			//delete[] root;
			delete[] queue;
			deleteTree(root);
		}
		Data* read(int addr);
		Elem* put(int addr, Data* cont);
		Elem* write(int addr, Data* cont);
		void print();
		void preOrder();
		void inOrder();	

		void prePrint(Node* root);
		void InPrint(Node* root);
		void deleteTree(Node* root);

};
void Cache::deleteTree(Node* root)
{
	if (root != NULL)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;

	}
	root = NULL;
}
//____________________

Node* newNode(int key, Data* cont, bool s) {
	Node* node = new Node();
	node->elem->addr = key;
	node->elem->data = cont;
	node->elem->sync = s;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return node;
}
int findH(Node* N)
{
	if (N == NULL)return 0;
	else return N->height;
}
Node* rightRotate(Node* y)
{
	Node* x = y->left;
	Node* T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(findH(y->left), findH(y->right)) + 1;
	x->height = max(findH(x->left), findH(x->right)) + 1;

	// Return new root
	return x;
}


Node* leftRotate(Node* x)
{
	Node* y = x->right;
	Node* T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(findH(x->left),findH(x->right)) + 1;
	y->height = max(findH(y->left),findH(y->right)) + 1;

	// Return new root
	return y;
}
//____________________

int getBalance(Node* N)
{
	if (N == NULL)
		return 0;
	return findH(N->left) - findH(N->right);
}
//_______________________
Node* insert(Node* node, Elem* key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key->addr,key->data,key->sync));

	if (key->addr < node->elem->addr)
		node->left = insert(node->left, key);
	else if (key->addr > node->elem->addr)
		node->right = insert(node->right, key);
	else 
		return node;

	node->height = 1 + max(findH(node->left),findH(node->right));

	int balance = getBalance(node);

	//Tree Rotation

	// Left Left Case
	if (balance > 1 && key->addr < node->left->elem->addr)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key->addr > node->right->elem->addr)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key->addr > node->left->elem->addr)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key->addr < node->right->elem->addr)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

//_____________________
Node* minValueNode(Node* node)
{
	Node* current = node;
	while (current->left != NULL) current = current->left;
	return current;
}

//_____________________
Node* deleteNode(Node* root, Elem* key) 
{

	// STEP 1: PERFORM STANDARD BST DELETE 
	if (root == NULL)
		return root;

	// If the key to be deleted is smaller 
	// than the root's key, then it lies
	// in left subtree 
	if (key->addr < root->elem->addr)
		root->left = deleteNode(root->left, key);

	// If the key to be deleted is greater 
	// than the root's key, then it lies 
	// in right subtree 
	else if (key->addr > root->elem->addr)
		root->right = deleteNode(root->right, key);

	// if key is same as root's key, then 
	// This is the node to be deleted 
	else
	{
		// node with only one child or no child 
		if ((root->left == NULL) ||
			(root->right == NULL))
		{
			Node* temp = root->left ? root->left : root->right;

			// No child case 
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case 
				*root = *temp; // Copy the contents of 
							   // the non-empty child 
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder 
			// successor (smallest in the right subtree) 
			Node* temp = minValueNode(root->right);

			// Copy the inorder successor's 
			// data to this node 
			root->elem->addr = temp->elem->addr;

			// Delete the inorder successor 
			root->right = deleteNode(root->right,
				temp->elem);
		}
	}

	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
	root->height = 1 + max(findH(root->left),findH(root->right));

	// STEP3: GET BALANCE FACTOR
	int balance = getBalance(root);

	//Tree rotation
	
	// Left Left Case 
	if (balance > 1 &&
		getBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case 
	if (balance > 1 &&
		getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case 
	if (balance < -1 &&
		getBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case 
	if (balance < -1 &&
		getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

//____________________


#endif