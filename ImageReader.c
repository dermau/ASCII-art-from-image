#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void imagereader(char filename[])
{
    FILE *ptr; //file pointer

    ptr = fopen(filename,"rb");  // r for read, b for binary. open file and place a pointer at the beginning
    unsigned char *string = malloc(0xFFFFFFFF); //allocate 4x8bit address of memory to buffer string
    size_t len = fread(string, sizeof(char), 0xFFFFFFFF, ptr); //read and load file contents into buffer
    printf("file opened and buffered\n");
    
    //READ LITTLE ENDIAN
    //sum += bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24)

    if(string[0] == 0x42 && string[1] == 0x4D)
    {
        printf("file is bmp\n");
        //READ LITTLE ENDIAN
        //sum += bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24)
        int fsize = string[0x0002] | (string[0x0002+1]<<8) | (string[0x0002+2]<<16) | (string[0x0002+3]<<24); //calculate size from little endian storage
        int intWidth = 0;
        intWidth += string[0x0012] | (string[0x0012+1]<<8) | (string[0x0012+2]<<16) | (string[0x0012+3]<<24);
        int intHeight = 0;
        intHeight += string[0x0016] | (string[0x0016+1]<<8) | (string[0x0016+2]<<16) | (string[0x0016+3]<<24);
        int intPlanes = 0;
        intPlanes += string[0x001A] | (string[0x001A+1]<<8);
        int intCompr = 0;
        intCompr += string[0x001E] | (string[0x001E +1]<<8) | (string[0x001E +2]<<16) | (string[0x001E +3]<<24);
        int intDepth = 0;
        intDepth = string[0x001C] | (string[0x001C+1]<<8);
        int intOffset = 0;
        intOffset = string[0x000A] | (string[0x000A+1]<<8) | (string[0x000A+2]<<16) | (string[0x000A+3]<<24);

        int intPxdatalen = intWidth * intHeight * (intDepth/8);

        printf("\n");
        printf("size: %i bytes len: %i\tcolour depth: %i bits per pixel\toffset: %02X\n", fsize, len, intDepth, intOffset);
        printf("width: %i \theight: %i \tplanes: %i \tcompression: %i\n", intWidth, intHeight, intPlanes, intCompr);
        printf("length of pixel data: %i bytes\toutputstring supposed length: %i\n", intPxdatalen, intPxdatalen/3);
        printf("First four bytes of pixels: %02X %02X %02X %02X\n", string[string[0x000A]], string[string[0x000A]+1], string[string[0x000A]+2], string[string[0x000A]+3]);
        printf("\n");
        /*
        //print out entire file in hex bytes
        // for(int x, i = 0; i < fsize; i++)
        // {
        //     printf("%02X ", *(string+i));

        //     x++;
        //     if (x ==16) //line break every 16 bytes
        //     {
        //         x=0;
        //         printf("\n");
        //     }

        // } */
        printf("\n");



        // ░░ ▒▒ ▓▓ ██
        //pixel data to ascii characters
        if(intDepth == 24)
        {
            printf("intDepth==24bit\n");

            

            //unsigned char outputstr[(intPxdatalen/3)];

            unsigned char outputstr[(intHeight * intWidth)];




            printf("outputstr declare. length: %i\n", sizeof(outputstr));
            //strcpy(outputstr, "");
            printf("outputstr assign\n");
            unsigned int test[1000];

            //translate from luminance to ascii
            printf("for loop size==: %i\n", fsize-intOffset);
            //from end of pxdata to offset address
            
            for(int x=0, y=1, temp, i=intOffset+intPxdatalen; i >= intOffset; i-=3, y++, x++)
            {
                float pLuminance = (0.114*string[i] + 0.587*string[i-1] + 0.299*string[i-2]);
                //printf("pluminance\n");
                if(pLuminance > 191.25)
                {
                    strcat(outputstr, ".");
                }
                else if(pLuminance > 127.5 && pLuminance < 191.25)
                {
                    strcat(outputstr, "=");
                }
                else if(pLuminance > 63.75 && pLuminance < 127.5)
                {
                    strcat(outputstr, "?");
                    
                }
                else if(pLuminance < 63.75)
                {
                    strcat(outputstr, "M");
                    
                }

                

                // if(y == intWidth)
                // {
                //     y=1;
                //     x++;
                // }
                
                

                float percent = (float) (i-intOffset)/intPxdatalen*100;
                int check = (int)(percent) % 10;
                //if percent == (int)percent
                if (check == 0 && temp!=(int)percent)
                {
                    printf("%i%%|", (int)percent);
                    temp = (int) percent;

                }
                //printf("%f|", percent);
                
                
                
                
            }
            strcat(outputstr, "\0");
            printf("\n");


            //print outputstr
            //size_t n = sizeof(outputstr)/sizeof(outputstr[0]);
            int n = sizeof(outputstr);
            for(int x=0, i=0; i<n; i++)
            {
                printf("%c", outputstr[i]);
                if ((i+1) % intWidth == 0)
                {
                    //x++;
                    //i=n-1;
                    printf("\t %i %i\n", i, (i+1) % intWidth);
                    
                }
            }
            printf("\n");
            printf("outputstr done\n");
            printf("%s\n", outputstr);
        }

        //24bit stored as BGR in 3 bytes, 1 byte each



    }

    fclose(ptr);
    
    
    
}

void printAscii(unsigned char *pixeldata)
{

}

int main(int argc, char* argv[])
{
    int i;
    for(i=0; i<argc; ++i)
    {   
        printf("Argument %d : %s\n", i, argv[i]); //argv[0] == pwd/file

        //switch case. case argv[i]: argv[i+1] for file/name/etc

        if(strstr(argv[i], ".bmp") != NULL)
        {
            char *filename = argv[i];
            imagereader(filename);
        }
    }
    return 0;
}



