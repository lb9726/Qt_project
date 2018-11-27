#include <stdio.h>


void BinarySearch(unsigned char *data, unsigned int length, unsigned char target);
unsigned int GetSecond(unsigned char *data, unsigned int length);

int main(void)
{
    printf("Hello World!\n");
    unsigned char a[] = {33, 22, 11, 0, 77};
//    BinarySearch(a, 1, 1);
    printf("Second: %d \n", GetSecond(a, 5));

    return 0;
}

void BinarySearch(unsigned char *data, unsigned int length, unsigned char target)
{
    unsigned int start = 0;
    unsigned int end = length - 1;
    unsigned int mid = (start + end) / 2;

    while ((start != end) && (data[mid] != target)) {
        if (target > data[mid]) {
            start = mid + 1;
        }

        if (target < data[mid]) {
            end = mid - 1;
        }

        mid = (start + end) / 2;
    }

    if (data[mid] == target) {
        printf("Position is %d! \n", mid);
    } else {
        printf("Not found! \n");
    }
}

unsigned int GetSecond(unsigned char *data, unsigned int length)
{
    unsigned int i;
    unsigned int first = 0;
    unsigned int second = 0;

    for (i = 1; i < length; i++) {
        if (data[i] > data[first]) {
            second = first;
            first = i;
        } else if ((data[i] > data[second]) && (data[i] < data[first])) {
            second = i;
        }
    }

    return second;
}
