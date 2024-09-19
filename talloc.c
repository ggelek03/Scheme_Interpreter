#include <stdlib.h>
#include "value.h"
#include "talloc.h"
#include "linkedlist.h"
Value *head;

Value *talloccons(Value *newCar, Value *newCdr){
    Value *consType = malloc(sizeof(Value));
    consType -> type = CONS_TYPE;
    consType -> c.car = newCar; 
    consType -> c.cdr = newCdr; 
    return consType;
}

Value *tallocnull(){
    Value *null = malloc(sizeof(Value));
    null->type = NULL_TYPE;
    return null; 
}
// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size){
    void *memory = malloc(size); // purpose of void is 
    if(head == NULL){
        head = talloccons(memory, tallocnull());
    }
    else {
        head = talloccons(memory, head);
    }
    return memory; 
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree(){
    if(head == NULL){
        return; 
    }
    Value *current = head; 
    while(current -> type == CONS_TYPE){
        void *next_value = current -> c.cdr; 
        free(current -> c.car);
        free(current); 
        current = next_value; 
    }
    free(current);
    head = NULL;
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
    tfree();
    exit(status);
}