/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char* argv[1])
{
    // TODO
    if (argc < 1) {
        printf("ERROR");
        return 1;
    }
    // OPEN A FILE
    FILE* raw = fopen("card.raw", "r");
    if (raw == NULL) {
        printf("Error. Couldn't open the file");
        return 1;
    }
    
    uint8_t block[512] = {};
    
    //creating needed variables and arrays
    int number = 0;
    int haveOpened = 0;
    char* title = argv[0];
    FILE* image;
    
    
    while (fread(block, 512, 1, raw) > 0) {
        
        if (block[0] == 0xff &&
            block[1] == 0xd8 &&
            block[2] == 0xff &&
            (block[3] & 0xf0) == 0xe0) {
            
           
            if (haveOpened == 0) {
                sprintf(title, "%03i.jpg", number);
                 // creating img file
                image = fopen(title, "w");
                if (image == NULL) {
                    printf("Error. Couldn't create a file");
                    return 2;
                } 
                fwrite (block, 512, 1, image);
                haveOpened = 1;

            }
            else if (haveOpened == 1) {
                fclose(image);
                number++;
                sprintf(title, "%03i.jpg", number);
                 // creating img file
                image = fopen(title, "w");
                if (image == NULL) {
                    printf("Error. Couldn't create a file");
                    return 2;
                } 
                fwrite (block, 512, 1, image);
            }
            
        }
        
        else {
            if (haveOpened == 1) {
                fwrite (block, 512, 1, image);  
            } 
        }
    };
    fclose(raw);
    fclose(image);
    return 0;
}
/*
Get 512 bytes
if have jpeg signature
    if haveOpened = 0 > create JPG > write bytes > haveOpened = 1; 
    if haveOpened = 1 > close previous JPG >  number++ > create  JPG > write bytes
else
    if haveOpened = 0 > skip step
    if haveOpened = 1 > write bytes to current file.
*/























