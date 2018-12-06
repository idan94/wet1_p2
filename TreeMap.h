//
// Created by idanc on 23/11/2018.
//

#ifndef WET1_TREEMAP_H
#define WET1_TREEMAP_H

#include <stdexcept>
#include <exception>
#include <iostream>
#include "library1.h"

template<class T>
class Node {
    int key;
    T *object;
    Node<T> *fatherPtr;
    Node<T> *leftSonPtr;
    Node<T> *rightSonPtr;
    int height;
public:
    //Constructors:
    Node<T>() : key(-1),
                object(nullptr),
                fatherPtr(nullptr),
                leftSonPtr(nullptr),
                rightSonPtr(nullptr),
                height(0) {}

    Node<T>(int key, T *t) : key(key),
                             object(nullptr),
                             fatherPtr(nullptr),
                             leftSonPtr(nullptr),
                             rightSonPtr(nullptr), height(0) {
        T *newObject = new T(*t);
        object = newObject;
    }

    //Destructor:
    ~Node<T>() {
        delete ((T *) object);
    }

    //Copy Constructor:
    Node<T>(const Node<T> &a) : key(a.key),
                                object(a.object),
                                fatherPtr(nullptr),
                                leftSonPtr(nullptr),
                                rightSonPtr(nullptr), height(a.height) {}

    //Assignment Operator:
    Node<T> &operator=(const Node<T> &a) {
        if (this == &a) return *this;
        key = a.key;
        object = new T(*(a.object));
        fatherPtr = nullptr;
        leftSonPtr = nullptr;
        rightSonPtr = nullptr;
        height = a.height;
        return *this;
    }

    int getKey() const {
        return this->key;
    }

    T *getObject() const {
        return this->object;
    }

    int getHeight() const {
        return this->height;
    }

    void setHeight(int newHeight) {
        this->height = newHeight;
    }

    int fixHeight() {
        int left, right;
        if (!this->getLeftSonPtr()) {
            left = -1;
        } else {
            left = this->getLeftSonPtr()->getHeight();
        }
        if (!this->getRightSonPtr()) {
            right = -1;
        } else {
            right = this->getRightSonPtr()->getHeight();
        }
        this->setHeight(left > right ? left + 1 : right + 1);   //take max
    }

    Node<T> *getFatherPtr() const {
        return this->fatherPtr;
    };

    Node<T> *getLeftSonPtr() const {
        return this->leftSonPtr;
    };

    Node<T> *getRightSonPtr() const {
        return this->rightSonPtr;
    };

    void setFather(Node<T> *node) {
        this->fatherPtr = node;
    }

    void setLeftSon(Node<T> *node) {
        this->leftSonPtr = node;
    }

    void setRightSon(Node<T> *node) {
        this->rightSonPtr = node;
    }

    int numberOfSons() const {
        int num = 0;
        if (leftSonPtr) {
            num++;
        }
        if (rightSonPtr) {
            num++;
        }
        return num;
    }

    void swap(Node<T> *a) {
        int tempKey = a->getKey();
        T *tempObject = a->getObject();
        a->key = this->getKey();
        a->object = this->getObject();
        this->key = tempKey;
        this->object = tempObject;
    }

    int getBalance() const {
        int left = 0, right = 0;
        if (this->getLeftSonPtr()) {
            left = this->getLeftSonPtr()->getHeight() + 1;
        }
        if (this->getRightSonPtr()) {
            right = this->getRightSonPtr()->getHeight() + 1;
        }
        return (left - right);
    }

    bool isBalanced() const {
        return (this->getBalance() <= 1 && this->getBalance() >= (-1));
    }
};


template<class T>
class TreeMap {
    Node<T> *root;
    int size;
public:
    //Constructor:
    TreeMap() : root(nullptr),
                size(0) {
    }

    static void postOrderDelete(Node<T> *node) {
        if (!node) {
            return;
        }
        postOrderDelete(node->getLeftSonPtr());
        postOrderDelete(node->getRightSonPtr());
        delete node;
    }

    //Destructor:
    ~TreeMap() {
        postOrderDelete(root);
    }

    static void preOrderCopy(Node<T> *newNode, Node<T> *node) {
        if (!newNode) {
            node = nullptr;
        } else {
            newNode = node;
            preOrderCopy(newNode->getLeftSonPtr(), node->getLeftSonPtr());
            preOrderCopy(newNode->getRightSonPtr(), node->getRightSonPtr());
        }
    }

    //Copy Constructor:
    TreeMap(const TreeMap &a) : root(nullptr),
                                size(a.size) {
        preOrderCopy(root, a.root);
    }

    //Assignment Operator:
    TreeMap &operator=(const TreeMap &a)= default;

    //Rotations:
    void rotate(Node<T> *node) {
        Node<T> *toFix;
        if (node == root) {
            if (node->getBalance() == 2) {        //left_____
                if (node->getLeftSonPtr()->getBalance() >= 0) {   //leftLeft
                    this->root = leftLeftRotate(node);
                    this->root->setFather(nullptr);
                } else {      //leftRight
                    this->root = leftRightRotate(node);
                    this->root->setFather(nullptr);
                }
            } else {                  //right____getBalance==(-2)
                if (node->getRightSonPtr()->getBalance() <= 0) {   //rightRight
                    this->root = rightRightRotate(node);
                    this->root->setFather(nullptr);
                } else {        //rightLeft
                    this->root = rightLeftRotate(node);
                    this->root->setFather(nullptr);
                }
            }
            toFix = root;
        } else {
            Node<T> *father = node->getFatherPtr();
            if (node->getBalance() == 2) {        //left_____
                if (node->getLeftSonPtr()->getBalance() >= 0) {   //leftLeft
                    if (father->getLeftSonPtr() == node) {
                        father->setLeftSon(leftLeftRotate(node));
                        father->getLeftSonPtr()->setFather(father);
                        toFix = father->getLeftSonPtr();
                    } else {      //node is the right son of his father
                        father->setRightSon(leftLeftRotate(node));
                        father->getRightSonPtr()->setFather(father);
                        toFix = father->getRightSonPtr();
                    }
                } else {      //leftRight
                    if (father->getLeftSonPtr() == node) {
                        father->setLeftSon(leftRightRotate(node));
                        father->getLeftSonPtr()->setFather(father);
                        toFix = father->getLeftSonPtr();
                    } else {      //node is the right son of his father
                        father->setRightSon(leftRightRotate(node));
                        father->getRightSonPtr()->setFather(father);
                        toFix = father->getRightSonPtr();
                    }
                }
            } else {                  //right____getBalance==(-2)
                if (node->getRightSonPtr()->getBalance() <= 0) {    //rightRight
                    if (father->getLeftSonPtr() == node) {
                        father->setLeftSon(rightRightRotate(node));
                        father->getLeftSonPtr()->setFather(father);
                        toFix = father->getLeftSonPtr();
                    } else {      //node is the right son of his father
                        father->setRightSon(rightRightRotate(node));
                        father->getRightSonPtr()->setFather(father);
                        toFix = father->getRightSonPtr();
                    }
                } else {        //rightLeft
                    if (father->getLeftSonPtr() == node) {
                        father->setLeftSon(rightLeftRotate(node));
                        father->getLeftSonPtr()->setFather(father);
                        toFix = father->getLeftSonPtr();
                    } else {      //node is the right son of his father
                        father->setRightSon(rightLeftRotate(node));
                        father->getRightSonPtr()->setFather(father);
                        toFix = father->getRightSonPtr();
                    }

                }
            }

        }
        if (toFix->getLeftSonPtr()) {
            toFix->getLeftSonPtr()->fixHeight();
        }
        if (toFix->getRightSonPtr()) {
            toFix->getRightSonPtr()->fixHeight();
        }
        toFix->fixHeight();
    }

    Node<T> *leftLeftRotate(Node<T> *node) {
        Node<T> *newFather = node->getLeftSonPtr();
        node->setLeftSon(newFather->getRightSonPtr());
        if (node->getLeftSonPtr()) {
            node->getLeftSonPtr()->setFather(node);
        }
        newFather->setRightSon(node);
        node->setFather(newFather);
        return newFather;
    }

    Node<T> *leftRightRotate(Node<T> *node) {
        Node<T> *newFather = node->getLeftSonPtr()->getRightSonPtr();
        node->getLeftSonPtr()->setRightSon(newFather->getLeftSonPtr());
        if (newFather->getLeftSonPtr()) {
            newFather->getLeftSonPtr()->setFather(node->getLeftSonPtr());
        }
        newFather->setLeftSon(node->getLeftSonPtr());
        node->getLeftSonPtr()->setFather(newFather);
        node->setLeftSon(newFather->getRightSonPtr());
        if (newFather->getRightSonPtr()) {
            newFather->getRightSonPtr()->setFather(node);
        }
        newFather->setRightSon(node);
        node->setFather(newFather);
        return newFather;
    }

    Node<T> *rightRightRotate(Node<T> *node) {
        Node<T> *newFather = node->getRightSonPtr();
        node->setRightSon(newFather->getLeftSonPtr());
        if (node->getRightSonPtr()) {
            node->getRightSonPtr()->setFather(node);
        }
        newFather->setLeftSon(node);
        node->setFather(newFather);
        return newFather;
    }

    Node<T> *rightLeftRotate(Node<T> *node) {
        Node<T> *newFather = node->getRightSonPtr()->getLeftSonPtr();
        node->getRightSonPtr()->setLeftSon(newFather->getRightSonPtr());
        if (newFather->getRightSonPtr()) {
            newFather->getRightSonPtr()->setFather(node->getRightSonPtr());
        }
        newFather->setRightSon(node->getRightSonPtr());
        node->getRightSonPtr()->setFather((newFather));
        node->setRightSon(newFather->getLeftSonPtr());
        if (newFather->getLeftSonPtr()) {
            newFather->getLeftSonPtr()->setFather(node);
        }
        newFather->setLeftSon(node);
        node->setFather(newFather);
        return newFather;
    }

    void inorderPrint() {
        std::cout << "Printing tree In Order: " << std::endl;
        inorderPrintAux(root);
        std::cout << "Done Printing In Order " << std::endl;
    }

    void inorderPrintAux(Node<T> *node) {
        if (!node) return;
        inorderPrintAux(node->getLeftSonPtr());
        std::cout << (node->getKey()) << "  ";
        inorderPrintAux(node->getRightSonPtr());
    }

    StatusType findNode(int key, Node<T> **node) const {
        if (!this->getRoot()) {
            return FAILURE;
        }
        return findNodeAux(key, root, node);
    }

    StatusType findNodeAux(int key, Node<T> *node, Node<T> **wanted) const {
        if (node->getKey() == key) {        //if we found they key
            *wanted = node;
            return SUCCESS;
        }
        if (node->getKey() > key) {
            if (!node->getLeftSonPtr()) {
                *wanted = node;
                return FAILURE;
            }
            return findNodeAux(key, node->getLeftSonPtr(), wanted);
        } else { // case: it->getKey() < key)
            if (!node->getRightSonPtr()) {
                *wanted = node;
                return FAILURE;
            }
            return findNodeAux(key, node->getRightSonPtr(), wanted);
        }
    }

    /*Node<T> *find(int key, Node<T>* node=root) {
        if(!root) {
            return nullptr;
        }
        Node<T>* it=root;
        if (it->getKey() == key) {
            return it;
        }
        if(it->getKey() > key){
            return find(key,(it->getLeftSonPtr()));
        }
        else { // case: it->getKey() < key)
            return find(key,(it->getRightSonPtr()));
        }
    }*/
    StatusType addNodeAvl(int key, T *t, void **node) {
        Node<T> *curNode;
        addNodeTree(key, t, &curNode);
        while (curNode != root) {
            curNode->fixHeight();
            Node<T> *parent = curNode->getFatherPtr();
            if (parent->getHeight() >= curNode->getHeight() + 1) {
                parent->fixHeight();
                node = ((void **) curNode);
                return SUCCESS;
            }
            parent->setHeight(curNode->getHeight() + 1);
            //parent->fixHeight();
            if (!parent->isBalanced()) {
                rotate(parent);
                node = ((void **) curNode);
                return SUCCESS;
            } else {
                // ((Node<T> *) node) = parent;
                curNode = parent;
            }
        }
        node = ((void **) curNode);
        return SUCCESS;
    }

    StatusType addNodeTree(int key, T *t, Node<T> **node) {
        if (size == 0) {    //if its the first Node of the tree
            try {
                Node<T> *newNode = new Node<T>(key, t);
                root = newNode;
                *node = newNode;
            }
            catch (std::bad_alloc &a) {
                return ALLOCATION_ERROR;
            }
            size++;

            return SUCCESS;
        }
        Node<T> *lastCheck = nullptr;
        StatusType findStatus = findNode(key, &lastCheck);
        if (findStatus == SUCCESS) {
            return FAILURE;
        } else {       //there is no key like this, and we will insert new Node<T>
            *node = new Node<T>(key, t);
            this->size++;
            if (key < lastCheck->getKey()) {
                lastCheck->setLeftSon(((Node<T> *) *node));
                ((Node<T> *) *node)->setFather(lastCheck);
            } else {      //when key> lastCheck->getKey()
                lastCheck->setRightSon(((Node<T> *) *node));
                ((Node<T> *) *node)->setFather(lastCheck);
            }

            return SUCCESS;
        }
    }

    StatusType deleteNodeAvl(int key) {
        Node<T> *node = nullptr;
        if (deleteNodeTree(key, &node) == FAILURE) {
            return FAILURE;
        }
        if (!node) return SUCCESS; //means we deleted the root
        while (1) {
            //step 1: Update h(v)
            node->fixHeight();
            int fatherPrevHeight;
            if (node != root) {
                fatherPrevHeight = node->getFatherPtr()->getHeight();
            }
            //step 2: check balance, rotate if needed and go up
            if (!node->isBalanced()) {
                rotate(node);
                if (node == root) break;
                if (node->getFatherPtr()->getHeight() == fatherPrevHeight) {
                    return SUCCESS;
                }
                node = node->getFatherPtr();
                continue;
            }
            if (node == root) break;
            node = node->getFatherPtr();

            /*if (node->getFatherPtr()->getHeight() == fatherPrevHeight) {
                return SUCCESS;
            } else {      //the height has benn changed and BS is good
                node = node->getFatherPtr();
            }*/
        }
        return SUCCESS;
    }

    StatusType deleteNodeTree(int key, Node<T> **fatherOfDeleted) {
        Node<T> *node = nullptr;
        StatusType findStatus = findNode(key, &node);
        if (findStatus == FAILURE) {
            return FAILURE;
        } else {       //means we found the node
            int numberOfSons = node->numberOfSons();
            switch (numberOfSons) {
                case 0: {          //if node is a leaf
                    if(node == root){
                        delete node;
                        root= nullptr;
                        this->size--;
                        return SUCCESS;
                    }
                    Node<T> *father = node->getFatherPtr();
                    if (father->getLeftSonPtr() == node) {
                        father->setLeftSon(nullptr);
                    } else { //means node is the right son of his father.
                        father->setRightSon(nullptr);
                    }
                    *fatherOfDeleted = father;
                    delete node;
                    this->size--;
                    return SUCCESS;
                }
                case 1: { //there is ONE son to node
                    //we will let node's father to point to his son
                    if (node == root) {
                        if (node->getLeftSonPtr()) {
                            root = node->getLeftSonPtr();
                        } else {
                            if (node->getRightSonPtr()) {
                                root = node->getRightSonPtr();
                            } else { root = nullptr; }
                        }
                        *fatherOfDeleted = nullptr;
                        delete node;
                        this->size--;
                        return SUCCESS;
                    }
                    Node<T> *father = node->getFatherPtr();
                    if (father->getLeftSonPtr() == node) {
                        //node is the left son
                        if (node->getLeftSonPtr()) {
                            father->setLeftSon(node->getLeftSonPtr());
                            (father->getLeftSonPtr())->setFather(father);
                        } else { //means node has ONLY right son
                            father->setLeftSon(node->getRightSonPtr());
                            (father->getLeftSonPtr())->setFather(father);
                        }
                    } else {    //node is the right son
                        if (node->getLeftSonPtr()) {
                            father->setRightSon(node->getLeftSonPtr());
                            (father->getRightSonPtr())->setFather(father);
                        } else { //means node has ONLY right son
                            father->setRightSon(node->getRightSonPtr());
                            (father->getRightSonPtr())->setFather(father);
                        }
                    }
                    *fatherOfDeleted = father;
                    delete node;
                    this->size--;
                    return SUCCESS;
                }
                case 2: {
                    Node<T> *swappingNode = node->getRightSonPtr();
                    while (swappingNode->getLeftSonPtr()) {
                        swappingNode = swappingNode->getLeftSonPtr();
                    }
                    swappingNode->swap(node);
                    if (swappingNode->numberOfSons() == 0) {
                        Node<T> *father = swappingNode->getFatherPtr();
                        if (father->getLeftSonPtr() == swappingNode) {
                            father->setLeftSon(nullptr);
                        } else { //means node is the right son of his father.
                            father->setRightSon(nullptr);
                        }
                        *fatherOfDeleted = father;
                        delete swappingNode;
                        this->size--;
                        return SUCCESS;
                    } else { //means it has ONE son ONLY
                        Node<T> *father = swappingNode->getFatherPtr();
                        if (father->getLeftSonPtr() == swappingNode) {
                            //node is the left son
                            if (swappingNode->getLeftSonPtr()) {
                                father->setLeftSon(
                                        swappingNode->getLeftSonPtr());
                                (father->getLeftSonPtr())->setFather(father);
                            } else { //means node has ONLY right son
                                father->setLeftSon(
                                        swappingNode->getRightSonPtr());
                                (father->getLeftSonPtr())->setFather(father);
                            }
                        } else {    //node is the right son
                            if (swappingNode->getLeftSonPtr()) {
                                father->setRightSon(
                                        swappingNode->getLeftSonPtr());
                                (father->getRightSonPtr())->setFather(father);
                            } else { //means node has ONLY right son
                                father->setRightSon(
                                        swappingNode->getRightSonPtr());
                                (father->getRightSonPtr())->setFather(father);
                            }
                        }
                        *fatherOfDeleted = father;
                        delete swappingNode;
                        this->size--;
                        return SUCCESS;
                    }
                    return deleteNodeTree(key, fatherOfDeleted);
                }
            }
        }
    }

    StatusType deleteNodeByPtr(Node<T> *node) {
        if (!node) {
            return INVALID_INPUT;
        }
        return deleteNodeAvl(node->getKey());
    }

    StatusType getSize(int *n) const {
        *n = this->size;
        return SUCCESS;
    }

    Node<T> *getRoot() const {
        return root;
    }
};

#endif //WET1_TREEMAP_H
