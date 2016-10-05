#include <stdio.h>
#include <stdlib.h>


int main (void) {
    FILE* raw = fopen("card.raw", "r");
    if (raw == NULL) {
        printf("Error. Couldn't open the file");
        return 1;
    }
    float totalSize = sizeof(raw);
    printf("%.10f", totalSize);
    
    fclose(raw);
    return 0;
}