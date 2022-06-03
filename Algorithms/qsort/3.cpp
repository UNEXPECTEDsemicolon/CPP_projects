#include <stdio.h>
#include <stdlib.h>

struct Point {
    int x;
    int y;
};

typedef struct Point Point;

int cmp_Point(const void * p1, const void * p2) {
    Point a = *(const Point *)p1;
    Point b = *(const Point *)p2;
    return (a.x*a.x + a.y*a.y) - (b.x*b.x + b.y*b.y);
}

int cmp_Point_x(const void * p1, const void * p2) {
    Point a = *(const Point *)p1;
    Point b = *(const Point *)p2;
    return a.x - b.x;
}

int cmp_Point_y(const void * p1, const void * p2) {
    Point a = *(const Point *)p1;
    Point b = *(const Point *)p2;
    return a.y - b.y;
}

int main() {
    int n;
    scanf("%d", &n);
    Point *a = (Point *)malloc(sizeof(Point) * n);
    for (int i = 0; i < n; i++)
        scanf("%d %d", &a[i].x, &a[i].y);
    qsort(a, n, sizeof(Point), cmp_Point_y);
    qsort(a, n, sizeof(Point), cmp_Point_x);
    qsort(a, n, sizeof(Point), cmp_Point);
    for (int i = 0; i < n; i++)
        printf("%d %d\n", a[i].x, a[i].y);
    free(a);
}