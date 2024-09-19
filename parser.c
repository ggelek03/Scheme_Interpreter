#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"

// takes tokens in, returns it as a tree
Value *parse(Value *tokens) {

    Value *tree = makeNull(); // tree = empty
    int depth = 0; // count the depth 
    Value *current = tokens; // ptr to the start

    while (current->type != NULL_TYPE) { // while there are still tokens
        Value *token = current -> c.car; // first token

        if (token -> type != CLOSE_TYPE) { // while it isn't a close type, you want to push it to the stack
            tree = cons(token, tree); // update the tree with the new token
            
            if (token -> type == OPEN_TYPE) {
               depth = depth + 1;
            }

        } else { // it is a close parenthesis
            Value *newnode = makeNull(); // going deeper into the tree (sub tree)

            while (tree -> type != NULL_TYPE && tree -> c.car -> type != OPEN_TYPE) {
                Value *token = tree -> c.car; // get the first token
                newnode = cons(token, newnode); // add on to the new node as long as the token is not an open type
                tree = tree -> c.cdr; // moving through tokens to the next ones
            }
            if (tree -> type == NULL_TYPE) {
                printf("Syntax error!\n");
                texit(0);
            }
            tree = cons(newnode, tree -> c.cdr);
            depth = depth - 1;
        }
        
        current = cdr(current); 
    }
    
    if (depth != 0) {
        printf("Syntax error. The amount of parenthesis do not match.\n");
        texit(0);
    }
    return reverse(tree);
}

void printTree(Value *tree) {
    while (tree -> type != NULL_TYPE) {
        switch (tree -> c.car -> type) {
            case INT_TYPE:
                printf("%i ", tree -> c.car -> i);
                break;
            case DOUBLE_TYPE:
                printf("%lf ", tree -> c.car -> d);
                break;
            case CONS_TYPE:
                printf("(");
                printTree(tree -> c.car); // cons cell
                printf(") ");
                break;
            case SYMBOL_TYPE:
                printf("%s ", tree -> c.car -> s);
                break;
            case STR_TYPE:
                printf("%s ", tree -> c.car -> s);
                break;
            case BOOL_TYPE:
                printf("%s ", tree -> c.car -> s);
                break;
            case NULL_TYPE:
                printf("()");
            default:
                break;
        }
        tree = tree -> c.cdr;
    }
}