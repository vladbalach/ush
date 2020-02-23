#include "libmx.h"

int mx_sqrt(int x) {
    int sqrt = 46341;

    if (x == 1)
        return x;
    if (x > 1) {
        while (sqrt > 1) {
            if (sqrt == x / sqrt && x % sqrt == 0)
                return sqrt;
            sqrt--;
        }
    }
    return 0;
}
