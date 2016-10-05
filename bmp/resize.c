/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 * Resizes file in scale from 1 to 100
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize scale infile outfile\n");
        return 1;
    }

    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 100 ) {
        printf("Please enter scale from 1 to 100");
        return 1;
    }

    // remember filenames
    int scale = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // storing original file values
    int originalWidth = bi.biWidth;
    int originalHeight = bi.biHeight;
    bi.biHeight *= scale;
    bi.biWidth *= scale;

    // determine padding for scanlines
    int paddingOrig =  (4 - (originalWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingNew =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    
    // changing output file's header
 
    bi.biSizeImage = ((bi.biWidth) * sizeof(RGBTRIPLE) + (paddingNew)) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + 54;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    

    // iterate over infile's scanlines
    
    for (int i = 0, biHeight = abs(originalHeight); i < biHeight; i++) {
        for (int l = 0; l < scale; l++) {
            // iterate over pixels in scanline
            for (int j = 0; j < originalWidth; j++) {
                // temporary storage
                RGBTRIPLE triple;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                //iterate over line again scale times
                for (int n = 0; n < scale ; n++) {    
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }    
            
            for (int k = 0; k < paddingNew; k++) {
                fputc(0x00, outptr);
            }  
            
            if (l < scale - 1) {
                fseek(inptr, -(originalWidth * 3), SEEK_CUR);    
            };  
        }
        
        // skip over padding, if any
        fseek(inptr, paddingOrig, SEEK_CUR);
       
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
