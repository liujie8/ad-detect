#include "ad-detect.h"

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("Usage: ad-detect <image>\n");
		return 0;
	}
	
	float score=0;
	Timer timer;

	score = adDetect(argv[1]);

	printf("Image score: %.2f\n", score);
	printf("Time cost: %f seconds.\n", timer.elapsed());
	return 1;
};
