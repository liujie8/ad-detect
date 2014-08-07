/*
	Copyright 2014 Liu Jie.
	This is an advertisement image detection tool depending on gocr and netpbm.
*/

#ifndef _ADDETECT
#define _ADDETECT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  
#include <sys/time.h>

class Timer
{
    struct  timeval start;
public:
    Timer () { restart(); }
    /// Start timing.
    void restart ()
    {   
        gettimeofday(&start, 0); 
    }   
    /// Stop timing, return the time passed (in second).
    float elapsed () const
    {   
        struct timeval end;
        float   diff;
        gettimeofday(&end, 0); 

        diff = (end.tv_sec - start.tv_sec)
                + (end.tv_usec - start.tv_usec) * 0.000001;
        return diff;
    }   
};
 
//ad detect function
float adDetect(char * fn, int step = 3, bool print = false)
{
	float score=0;

	//check file size
	long size = -1;
	struct stat statbuff;
	if(stat(fn, &statbuff) >= 0)size = statbuff.st_size;

	if(print)printf("file size %ld\n", size);

	//only detect image smaller than 50000 bytes
	if(size <= 0 || size >= 50000)return 0;

	char buf[1024];
	char cmd[100];

	sprintf(cmd,"gocr -u \" \" -C 0-9Qq \"%s\"", fn);
	if(print)printf("%s\n", cmd);

	//use gocr to extract characters from image
	FILE *pf = popen(cmd, "r");

	if(pf == NULL)
	{
		printf("popen %s error\n", cmd);
		return 0;
	}
	
	//detect phone number or qq number
	while( fgets(buf,1024,pf) != NULL )
	{
		if(print)printf(buf);
		int n = 0;
		int m = 0;
		int nz = 0; //none zero number
		char num[1024];

		for(; buf[n] != 0; n++)
		{
			if(buf[n] >= '0' && buf[n] <= '9')
			{
				if(buf[n] > '0')nz++; //counting none zero number

				num[m++] = buf[n];

				bool cut = true;

				//cut number serials if there is no number in 'step' steps forward.
				for(int i=1; i <= step; i++)
				{
					if( buf[n+i] == '\0' )break;
					if( buf[n+i] >= '0' && buf[n+i] <= '9' )
					{
						cut = false;
						break;
					}
				}

				if(cut)
				{
					num[m] = '\0';

					int len = strlen(num);
					if(len >= 6 && len <= 15 && (float)nz/len >= 0.5)
					{
						score++;
						puts(num);
					}

					m = 0;
					nz = 0;
				}
			}
		}
	}

	pclose(pf);
	pf = NULL;

	return score;
};

#endif
