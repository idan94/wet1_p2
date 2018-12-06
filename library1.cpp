//
// Created by idanc on 23/11/2018.
//
#include "TreeMap.h"
#include "library1.h"



void *Init() {
    void * DS;
    try {
        DS =(void*) new TreeMap<void *>();
    }
    catch (std::bad_alloc &a){
        return nullptr;
    }
    return DS;
}


StatusType Add(void *DS, int key, void *value, void **node) {
    if (!DS || !node) return INVALID_INPUT;
    try {
        return ((TreeMap<void*> *) DS)->addNodeAvl(key, &value, node);
    }
    catch (std::bad_alloc &a){
        return ALLOCATION_ERROR;
    }
}

StatusType Find(void *DS, int key, void **value) {
    if (!DS || !value) return INVALID_INPUT;
    return ((TreeMap<void*> *) DS)->findNode(key, ((Node<void*> **)value));
}


StatusType Delete(void *DS, int key) {
    if (!DS) return INVALID_INPUT;
    return ((TreeMap<void*> *) DS)->deleteNodeAvl(key);
}


StatusType DeleteByPointer(void *DS, void *p) {
    if (!DS) return INVALID_INPUT;
    return ((TreeMap<void*> *) DS)->deleteNodeByPtr((Node<void*> *) p);
}


StatusType Size(void *DS, int *n) {
    if (!DS || !n) return INVALID_INPUT;
    ((TreeMap<void*> *) DS)->getSize(n);
    return SUCCESS;
}


void Quit(void **DS) {
    delete *(TreeMap<void*> **)DS;
    *DS= nullptr;
}
