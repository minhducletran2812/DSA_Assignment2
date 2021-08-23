#include "main.h"
#include "Cache.h"

Data* Cache::read(int addr) {
 
    Node* tempRoot = this->root;
    while (tempRoot)
    {
        if (tempRoot->elem->addr == addr)
            return tempRoot->elem->data;
        else
        {
            if (tempRoot->elem->addr > addr)
                tempRoot = tempRoot->left;
            else if (tempRoot->elem->addr < addr)
                tempRoot = tempRoot->right;
        }
    }
    return  NULL;
   

}




//_____
Elem* Cache::put(int addr, Data* cont) {
	Elem* key1 = new Elem(addr,cont,true);
    if (queue->isfull())
    {
        Elem* tempElem=queue->dequeue2();
        this->root = deleteNode(this->root,tempElem);
        this->root = insert(this->root, key1);
        queue->enqueue(key1);
        return tempElem;
    }
    else
    {
        this->root = insert(this->root, key1);
        queue->enqueue(key1);
        return NULL;
    }
}
Elem* Cache::write(int addr, Data* cont) {
    //_________
    Node* tempRoot = this->root;
    int check = 0;
    while (tempRoot)
    {
        if (tempRoot->elem->addr == addr)
        {
            check = 1;
            tempRoot->elem->data = cont;
            tempRoot->elem->sync = false;
            if (queue->findele(addr))
            {
                queue->replacele(addr, cont);
            }
            break;
        }
        else
        {
            if (tempRoot->elem->addr > addr)
                tempRoot = tempRoot->left;
            else if (tempRoot->elem->addr < addr)
                tempRoot = tempRoot->right;
        }
    }
    if (!check)
    {
        Elem* key1 = new Elem(addr, cont, false);
        if (queue->isfull())
        {
            Elem* tempElem = queue->dequeue2();
            this->root = deleteNode(this->root, tempElem);
            //free(tempElem);
            this->root = insert(this->root, key1);
            queue->enqueue(key1);
            return tempElem;
        }
        else
        {
            this->root = insert(this->root, key1);
            queue->enqueue(key1);
            return NULL;
        }
    }
    return NULL;

}
void Cache::print() {
    for (int i = queue->front; i <= queue->rear; i++)
    {
        queue->Queue[i]->print();
    }
}
void Cache::preOrder() {
    prePrint(this->root);

}
void Cache::InPrint(Node* root) {
    if (root == NULL)
        return;
    InPrint(root->left);
    root->elem->print();
    InPrint(root->right);
}
void Cache::prePrint(Node* root)
{
    if (root == NULL)
        return;
    root->elem->print();
    prePrint(root->left);
    prePrint(root->right);
}

void Cache::inOrder() {
    InPrint(this->root);
}


