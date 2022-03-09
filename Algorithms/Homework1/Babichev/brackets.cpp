#include <stdio.h>

int main() {
    char x;
    char *ala_stack = new char [120000ll];
    scanf("%c", &x);
    size_t head = 0;
    while (x != '\n' && x != '\0') {
        if (x == '{' || x == '[' || x == '(' || x == '<') {
            ala_stack[head] = x;
            head++;
        }
        else if (head > 0 && x == ala_stack[head - 1] + (x == ')' ? 1 : 2))
            head--;
        else {
            printf("NO\n");
            delete [] ala_stack;
            return 0;
        }
        scanf("%c", &x);
    }
    printf(head == 0 ? "YES\n" : "NO\n");
    delete [] ala_stack;
}