#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"

// Create a new NULL_TYPE value node.
Value *makeNull(){
    Value *null = talloc(sizeof(Value));
    null->type = NULL_TYPE;
    return null; 
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr){
    Value *consType = talloc(sizeof(Value));
    consType -> type = CONS_TYPE;
    consType -> c.car = newCar; 
    consType -> c.cdr = newCdr; 
    return consType;
}


// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
    switch (list -> type){
    case NULL_TYPE:
        break;
    case INT_TYPE:
        printf("%i", list -> i);
        break;
    case DOUBLE_TYPE:
        printf("%lf", list -> d);
        break;
    case STR_TYPE:
        printf("%s", list -> s);
        break;
    case CONS_TYPE:
        // printf("(");
        display(list -> c.car);
        display(list -> c.cdr);
        // printf(")"); 
        break;
    default: 
        break;
    }   
}

Value *reverse(Value *list){
    if (list -> type == NULL_TYPE) {
        return makeNull();    
    }
    Value *copy = makeNull();
    while (list -> type == CONS_TYPE) {
        Value *new_list = talloc(sizeof(Value));
        new_list = list -> c.car;
        copy = cons(new_list, copy);
        list = list->c.cdr;
    }
    return copy;
}
// Frees up all memory directly or indirectly referred to by list. This includes strings.
void cleanup(Value *list){
    if(list -> type == STR_TYPE){
        free(list -> s);
        list -> s = NULL; 
    }
    if (list -> type == CONS_TYPE){
        cleanup(list -> c.car);
        cleanup(list -> c.cdr);
    }
    free(list);
    
}

bool isNull(Value *value){
    assert(value != NULL); 
    if(value -> type == NULL_TYPE){
        return true;
    }
    return false; 
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
    assert(list != NULL);
    return(list -> c.car);
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
    assert(list != NULL);
    return(list -> c.cdr);
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.

int length(Value *value){
    assert(value != NULL);
    if(value -> type == NULL_TYPE){
        return 0;
    }
    if (value -> type != CONS_TYPE) {
        return 1;
    }
    int count = 0;
    Value *current = value;
    while (current -> type == CONS_TYPE) {
        count = count + 1;
        current = current -> c.cdr;
    }
    printf("Count value is %i", count);
    return count;
}