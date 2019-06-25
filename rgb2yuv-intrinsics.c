#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

unsigned char* ColorSpaceConvert(unsigned char R, unsigned char G, unsigned char B)
{
    static unsigned char YUV[3];
    
    YUV[0] = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
    YUV[1] = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128; 
    YUV[2] = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
    
    //YUV[0] = 0.299 * R + 0.587 * G + 0.114 * B;
    //YUV[1] = -0.168736 * R - 0.331264 * G + 0.5 * B + 128; 
    //YUV[2] = 0.5 * R - 0.418688 * G - 0.081312 * B + 128;
    
    //printf("Before ---> R: %u, G: %u, B: %u\n", R, G, B);
    //printf("After ---> Y: %u, U: %u, V: %u\n", YUV[0], YUV[1], YUV[2]);
    
    return YUV;
}

void rgb2yub(char *input_image, char *output_image)
{
	FILE * input_fp;
	FILE * output_fp;
	char character;
	int charCounter=0;
	
	input_fp = fopen(input_image, "r");
	if (input_fp == NULL)
	{
		printf("Error en apertura de archivo\n\n");
		exit(1);
	}
	else
	{       
		while((character = fgetc(input_fp))!= EOF)
		{
			charCounter++;
		}

		unsigned char RGB_buffer[charCounter];
        
        rewind(input_fp);
        charCounter = 0;
        
        while((character = fgetc(input_fp))!= EOF)
        {
            RGB_buffer[charCounter] = (unsigned char)character;
            charCounter++;
        }
          
        unsigned char YUV_buffer[2*charCounter];
    
        unsigned char *YUV;
        unsigned char Y_buffer[(charCounter/3)];
        unsigned char U_buffer[(charCounter/3)];
        unsigned char V_buffer[(charCounter/3)];
        
        int k = 0;
        
        for(int i = 0; i <= charCounter-3; i=i+3)
        {  
            YUV = ColorSpaceConvert(RGB_buffer[i], RGB_buffer[i+1], RGB_buffer[i+2]);
            
            Y_buffer[k] = YUV[0];
            U_buffer[k] = YUV[1];
            V_buffer[k] = YUV[2];
            
            k++;
        }
    
        memcpy(YUV_buffer, Y_buffer, (charCounter/3)*sizeof(unsigned char));
        memcpy(YUV_buffer + (charCounter/3), U_buffer, (charCounter/3)*sizeof(unsigned char));
        memcpy(YUV_buffer + (2*charCounter/3), V_buffer, (charCounter/3)*sizeof(unsigned char));
        
        output_fp = fopen(output_image, "w");
        
		if (output_fp == NULL)
		{
			printf("Error en crear archivo de salida\n\n");
			exit(1);
		}

		fputs(YUV_buffer, output_fp);
	}

	fclose(input_fp);
	fclose(output_fp);
}

int main(int argc, char **argv )
{
	char *ivalue = "initial_input";
	char *ovalue = "initial_output";
	extern char *optarg;
	int iflag = 0;
    int oflag = 0;
    int aflag = 0;
    int hflag = 0;
	int c;

	while((c = getopt(argc, argv, "ahi:o:")) != -1)
	{
		switch(c)
		{
			case 'i':
				iflag = 1;
				ivalue = optarg;// optarg stores the value after -i parameter
				break;
			case 'o':
				oflag = 1;
				ovalue = optarg;// optarg stores the value after -o parameter
				break;
			case 'a':
				aflag = 1;
				break;
			case 'h':
				hflag = 1;
				break;
			case '?':
				if(optopt=='o')
				{	fprintf(stderr, "Option -%c requires argument for output file \n", optopt);
					oflag=0;}
				else if(optopt=='i')
				{	fprintf(stderr, "Option -%c requires argument for input file \n", optopt);
					iflag=0;}
				else if (isprint(optopt))
					fprintf(stderr, "unknown option `-%c \n", optopt);
				else
					fprintf(stderr, "unknown character `\\x%x'.\n", optopt);
				return 1;
			default:
				printf("unknown case \n");
				break;
		}
	}

	if(hflag)
        printf(" Usage:\n ./rgb2yub -i <RGB file name> -o <YUV file name>\n");
	else
    {
        if(aflag)
            printf(" Authors:\n Jose Pablo Vernava \n Albert Hernandez \n Natalia Rodriguez \n Anthony Vega\n");
        else
        {
            if(iflag*oflag)
            {
                clock_t beginExecution;
                clock_t endExecution;
                double timeOfExecution;
                
                beginExecution = clock();
                rgb2yub(ivalue, ovalue);
                endExecution = clock();
                    
                timeOfExecution = (double)(endExecution - beginExecution)/CLOCKS_PER_SEC;
                printf("\n\nrgb2yub execution time is: %f Seconds\n\n", timeOfExecution);
            }
            else
            {
                printf("Missing Paramenter \n");
                printf(" Usage:\n ./rgb2yub -i <RGB file name> -o <YUV file name>\n");
            }
        }
    }
    
	return 0;
}
