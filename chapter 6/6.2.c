#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;
void x() {
    printf("Inside function x\n");
    switch(setjmp(env)) {
        case 0: 
            printf("Initial call\n");
            break;
        case 1:
            printf("Hey we long jumped here\n");
            break;
    }
    return;
}

void y() {
    printf("Inside function y\n");
    longjmp(env, 1);
}

int main() {
    printf("From function main\n");

    x();
    y();
}