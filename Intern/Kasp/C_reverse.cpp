#include <stdio.h>

int main() {
    char s[10000];
    size_t len = 0;
    char c = '\0';
    while (scanf("%c", &c) && c != '\0') {
        //printf("c=%c\n", c);
        printf("len=%zu\n", len);
        s[len] = c;
        if (c == '\n') {
            for (size_t i = 0; i < len / 2; ++i) {
                char temp = s[i];
                s[i] = s[len - i - 1];
                s[len - i - 1] = temp;
            }
            s[len] = '\0';
            len = 0;
            c = '\0';
            printf("%s\n", s);
            //break;
        } else
            ++len;
    }
}