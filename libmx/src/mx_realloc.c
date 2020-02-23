#include "libmx.h"


void *mx_realloc(void *ptr, size_t size) {
    unsigned char *ptr_ = (unsigned char *)ptr;
    unsigned char *newMem = (unsigned char *)malloc(size); 

    if (newMem == 0) 
        return 0;
    if(ptr == 0) 
        return (void *)newMem;
    for(size_t i = 0; i < size; i++) {
        newMem[i] = ptr_[i];
    }
    free(ptr);
    return newMem;
}
