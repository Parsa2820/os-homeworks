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
    struct x *data3;

    data = (struct x *)mm_malloc(sizeof(struct x));
    data->a = 1;
    data->b = 2;
    data->c = 3;
    mm_free(data);
    return 0;

    data2 = (struct x *)mm_malloc(sizeof(struct x));
    data2->a = 1;
    data2->b = 2;
    data2->c = 3;

    data3 = (struct x *)mm_malloc(sizeof(struct x));
    data3->a = 1;
    data3->b = 2;
    data3->c = 3;

    printf("%p, %p, %p\n", data, data2, data3);

    mm_free(data3);
    mm_free(data2);

    data3 = (struct x *)mm_malloc(sizeof(struct x));
    printf("%p\n", data3);

    return 0;
}
