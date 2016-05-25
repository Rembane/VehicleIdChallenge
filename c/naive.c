#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <inttypes.h>

#define ONE      0x1
#define IDX_MASK 0x7

uint32_t regnr_to_int(char* s);

int main() {
    char regnrs[2196986] = {0};
    FILE* fp = fopen("test2.txt", "r");
    struct stat buf;
    fstat(fileno(fp), &buf);
    size_t size = buf.st_size;
    void* input = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fileno(fp), 0);

    char* input_end = input + size;
    for(char* p = input; p < input_end; p += 7) {
        uint32_t idx   = regnr_to_int(p);
        uint32_t shidx = idx >> 3;

        /* Seriously handy debugging code:
        for(int i=0; i < 6; i++) {
            putchar(p[i]);
        }
        printf(" %i %i %i %i\n", idx, shidx, IDX_MASK & idx, ONE << (IDX_MASK & idx));
        */

        if((regnrs[shidx] & (ONE << (IDX_MASK & idx))) > 0) {
            printf("Found duplicate!\n");
            exit(0);
        } else {
            regnrs[shidx] = regnrs[shidx] | (ONE << (IDX_MASK & idx));
        }
    }
    printf("Nope.\n");
}

// Assume we have 26 characters. That is the length of the ASCII characters.
// Also assume that all strings are uppercase.
//
// This function returns a number between 0 and 17 575 888.
// The domain of all swedish registreringsnumber as integers.
uint32_t regnr_to_int(char* s) {
    return (
    ((uint32_t) s[0] - 64) * 676000 +
    ((uint32_t) s[1] - 64) * 26000  +
    ((uint32_t) s[2] - 64) * 1000   +
    ((uint32_t) s[3] - 48) * 100    +
    ((uint32_t) s[4] - 48) * 10     +
    ((uint32_t) s[5] - 48)) - 703111;
}

