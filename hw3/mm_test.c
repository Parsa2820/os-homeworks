/* A simple test harness for memory alloction. */

#include "mm_alloc.h"
#include <stdio.h>

struct x
{
    int a;
    int b;
    int c;
};

int main(int argc, char **argv)
{
    struct x *data;
    struct x *data2;

    data = (struct x *)mm_malloc(sizeof(struct x));
    data->a = 1;
    data->b = 2;
    data->c = 3;

    data2 = (struct x *)mm_realloc(data, sizeof(struct x));

    printf("%d %d %d\n", data2->a, data2->b, data2->c);

    mm_free(data2);

    printf("%d %d %d\n", data2->a, data2->b, data2->c);

    printf("malloc sanity test successful!\n");

    return 0;
}
