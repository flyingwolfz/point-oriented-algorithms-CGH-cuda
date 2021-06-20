#include <stdio.h>
#include <stdlib.h>
#include<cuda_runtime.h>
#include "device_launch_parameters.h"
__global__ void test(float *dimage, float *dholo )
{
	int h = blockIdx.x*blockDim.x + threadIdx.x;
	int l = blockIdx.y*blockDim.y + threadIdx.y;
	float x = (float)h;
	float y = (float)l;
	float hx = (x - 512.0)*0.008;
	float hy = (y - 512.0)*0.008;
	float lambda = 639.0 * 0.000001;
	float d = 0.008*10.0, holo = 0.0,  c = 0.0, s = 0.0;
	float ii = 0.0, jj = 0.0;
	long int prt = h + l * 1024;
	register float xx;
	register float yy,image,r,phi;
	register int k=0;
	float dep = 0.0f;
	 for (register int i = 0; i < 100; i++)
	 {
		 for (register int j = 0; j < 100; j++)
		 {
			 xx = ((float)i - 50.0)*d;
			 yy = ((float)j - 50.0)*d;	
			 image = dimage[k];
			 dep = 300;
			 k++;
			 r = (hx - xx)*(hx - xx) + (hy - yy)*(hy - yy) + dep*dep;
			 phi = 2 * 3.14159 / lambda * sqrt(r);
			 c = c+ image * cos(phi);
			 s = s+ image * sin(phi);	

		 }	 
	 }
	float jiao = atan2(s, c);
	if (jiao < 0)
	{
		jiao = jiao + 2.0 * 3.14159;
	}
	dholo[prt] = jiao / 2.0 / 3.14159;
}

extern "C"  void kernel(float *dimage, float *dholo)
{
	
	dim3 block(32, 32);
	dim3 grid(32, 32);

	test <<<grid, block >>> (dimage, dholo);

}