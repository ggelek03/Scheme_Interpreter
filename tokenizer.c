#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "value.h"
#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"
int charRead;

Value *subsequent(char c, Value *list) {
    int i = 1;
    Value *sub = talloc(sizeof(Value));
    sub -> type = SYMBOL_TYPE;
    sub -> s = talloc(sizeof(char)*302);
    sub -> s[0] = c; 
    charRead = (char)fgetc(stdin);
    while (isalpha(charRead) || charRead == '!' || charRead == '$' || charRead == '%' ||
charRead == '&' || charRead == '*' || charRead == '/' || charRead == ':' || charRead == '<' || charRead == '=' || charRead == '>' || 
charRead == '?' || charRead == '~' || charRead =='_' || charRead == '^' || charRead >= '0'&& charRead <= '9' || charRead == '.' || charRead == '+' || charRead == '-') {
        sub -> s[i++] = charRead;
        charRead = fgetc(stdin);
    }
    sub -> s[i++] = '\0'; 
    ungetc(charRead, stdin);
    return cons(sub, list);              
}


Value *string(char c, Value *list) {
    Value *quotation = talloc(sizeof(Value));
    quotation -> type = STR_TYPE;
    quotation -> s = talloc(sizeof(char) * 301);
    int i = 1;
    if(c == '"'){
        quotation -> s[0] = c;
        charRead = fgetc(stdin);
        while (charRead != '"'){
            if(charRead == EOF){
                printf("Syntax error. No end quote found in file");
                texit(1);
            }
            quotation -> s[i] = charRead;
            i++;
            charRead = fgetc(stdin);
        }
        quotation -> s[i] = c;
        quotation -> s[i+1] = '\0';
    }
    return cons(quotation, list);
}

Value *boolean(char c, Value *list) {
    Value *boolean = talloc(sizeof(Value));
    boolean -> type = BOOL_TYPE;
    boolean -> s = talloc(sizeof(char)*3);
    boolean -> s[0] = '#';
    boolean -> s[1] = c;
    boolean -> s[2] = '\0';
    return cons(boolean, list);
}

Value *number(char c, Value *list) {
    Value *num = talloc(sizeof(Value));
    int posneg = 0; 
    int dotpresent = 0;
    char *ptr;
    char *stringofnumbers = talloc(sizeof(char) * 301);
    int i = 0;
    if(c == '+'){
        charRead = fgetc(stdin);
        if(charRead == ' ' || charRead == '(' || charRead == ')'){
            num -> type = SYMBOL_TYPE;
            num -> s = talloc(sizeof(char) * 2);
            num -> s[0] = c;
            num -> s[1] = '\0';
            return cons(num,list);
        }
        if(charRead == ')' || charRead == '('){
            ungetc(charRead, stdin);
        }
        
    }
    else if(c == '-'){
        charRead = fgetc(stdin);
        posneg = -1;
        if(charRead == ')' || charRead == '('){
            ungetc(charRead, stdin);
        }
        if(charRead == ' ' || charRead == '(' || charRead == ')'){
            num -> type = SYMBOL_TYPE;
            num -> s = talloc(sizeof(char) * 2);
            num -> s[0] = c;
            num -> s[1] = '\0';
            return cons(num,list);
        }
    }

    while(isdigit(charRead) || charRead == '.') {
        stringofnumbers[i] = charRead;
        if (charRead == '.') {
            dotpresent = 1;
        }
        charRead = fgetc(stdin);
        i++;
    }
    stringofnumbers[i] = '\0'; // knows when to stop when reading mem -- useful for strol and strtod
    if (dotpresent == 1) {
        num -> type = DOUBLE_TYPE;
        double transform = 0;
        if (posneg == -1) {
            transform = strtod(stringofnumbers, &ptr);
            transform = -1 * transform;
            num -> d = transform; 
        }
        else {
            transform = strtod(stringofnumbers, &ptr);
            num -> d = transform; 
        }
        if(charRead == ')' || charRead == '('){
            ungetc(charRead, stdin);
        }
    }
    else if (dotpresent == 0) {
        num -> type = INT_TYPE;
        int transform = 0;
        if (posneg == -1) {
            transform = strtol(stringofnumbers, &ptr, 10);
            transform = -1 * transform;
            num -> i = transform;
        }
        else {
            transform = strtol(stringofnumbers, &ptr, 10);
            num -> i = transform; 
        }
        if(charRead == ')' || charRead == '('){
            ungetc(charRead, stdin);
        }
    }
    return cons(num,list); 
}

Value *openParen(char c, Value *list) {
    Value *open = talloc(sizeof(Value));
    open -> type = OPEN_TYPE;
    open -> s = talloc(sizeof(char)*3);
    open -> s[0] = c;
    open -> s[1] = '\0';
    return cons(open, list);
}

Value *closeParen(char c, Value*list) {
    Value *close = talloc(sizeof(Value));
    close -> s = talloc(sizeof(char)*3);
    close -> type = CLOSE_TYPE;
    close -> s[0] = c;
    close -> s[1] = '\0';
    return cons(close, list);
}

Value *tokenize() {
    Value *list = makeNull(); // make a null ptr
    charRead = fgetc(stdin); // get the first character
    if(charRead == '@'){ // check to see what the character is, it cannot be a @
        printf("Syntax error (readSymbol): symbol @ does not start with an allowed first character.");
        texit(1);
        Value *revList = reverse(list);
        return revList;
    }
    while (charRead != EOF) { // it will increment through the file until it reaches the end of it
        if (charRead == '(') {
            // The parenthesis is a standalone token.
            list = openParen(charRead, list);
        } 
        else if (charRead == ')') {
            // The parenthesis is a standalone token.
            list = closeParen(charRead, list);
        } 
        if (charRead == '+' || charRead == '-' || isdigit(charRead)) {
            list = number(charRead, list);
        } 
        else if (charRead == '#') { // boolean
            charRead = fgetc(stdin);
            if (charRead == 'f' || charRead == 't'){ // next char has to be t or f
                list = boolean(charRead, list);
            }
            else {
                printf("Syntax error. The boolean was not true or false\n");
                texit(1);
            }
        } 
        else if(charRead =='"'){ // string
            list = string(charRead, list);
        }
        else if (charRead == ';') { // comment
            while (charRead != '\n') {
                charRead = fgetc(stdin);
            }
        } 
        else if (charRead >= 'a' && charRead <= 'z' || charRead >= 'A' && charRead <= 'Z' || charRead == '!' || charRead == '$' || charRead == '%' ||
        charRead == '&' || charRead == '*' || charRead == '/' || charRead == ':' || charRead == '<' || charRead == '=' || charRead == '>' || 
        charRead == '?' || charRead == '~' || charRead =='_' || charRead == '^' ) {
            list = subsequent(charRead, list); //subsequent instead of initial, just want subsequent and error check within that
        }
        else if(charRead == '[' || charRead == ']' || charRead == '@'){
            printf("Syntax error\n");
            texit(1);
        } 
        charRead = fgetc(stdin);
        }
    Value *revList = reverse(list);
    return revList;
}

void displayTokens(Value *list){
    while (list -> type != NULL_TYPE) {
        Value *copy = list -> c.car;
        switch (copy -> type){
            // case NULL_TYPE:
            //     break;
            case INT_TYPE:
                printf("%i:integer\n", copy -> i);
                break;
            case DOUBLE_TYPE:
                printf("%lf:double\n", copy -> d);
                break;
            case OPEN_TYPE:
                printf("%s:open\n", copy -> s);
                break;
            case CLOSE_TYPE:
                printf("%s:close\n", copy -> s);
                break;
            case STR_TYPE:
                printf("%s:string\n", copy -> s);
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", copy -> s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", copy -> s);
                break;
            // case CONS_TYPE:
            //     displayTokens(list -> c.car);
            //     displayTokens(list -> c.cdr); 
            //     break; 
            default:
                break;
        }
        list = list -> c.cdr;
    }
}