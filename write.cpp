#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

uint64_t c1,
    c2;
struct timespec ts1, ts2;
double t;
double tsum;

int main()
{
    int fdo = open("write_check.txt", (O_CREAT | O_RDWR), 0644);

    if (fdo == -1)
    {
        printf("file open error\n");
        exit(1);
    }

    unsigned long int size = 2500L * 125;

    char arry[size];

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    write(fdo, &arry, sizeof(arry));
    fsync(fdo);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    t = ts2.tv_sec - ts1.tv_sec + (ts2.tv_nsec - ts1.tv_nsec) / 1e9;
    tsum = tsum + t;
    printf("%ld ,%.4f\n", size, t);

    return 0;
}
