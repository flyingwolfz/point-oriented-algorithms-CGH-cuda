#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cufft.h>
#include "cuda_runtime.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include<iostream>


using namespace std;
using namespace cv;

extern "C" void kernel(float *dimage, float *dholo);

int main()
{
	Mat image = imread("C:\\Users\\zcl\\Desktop\\tuzi1.png", 0);


	Mat out(1024, 1024, CV_8UC1, 1);

	int imagx = 100, imagey = 100;
	int holox = 1024, holoy = 1024;
	
	size_t sizei = imagx * imagey * sizeof(float);
	size_t sizeh = holox * holoy * sizeof(float);

	float *dimage, *dholo;
	float *himage, *hholo;


	himage = (float *)malloc(sizei);
	hholo = (float *)malloc(sizeh);
	cudaMalloc(&dimage, sizei);
	cudaMalloc(&dholo, sizeh);

	long int k = 0;
	for (int i = 0; i < imagx; i++)
	{
		uchar* tu = image.ptr<uchar>(i);

		for (int j = 0; j < imagey; j++)
		{
			himage[k] = tu[j]/255.0;
			k++;
		}
	}
	k = 0;

	cudaMemcpy(dimage, himage, sizei, cudaMemcpyHostToDevice);

	kernel(dimage,dholo);

	cudaMemcpy(hholo, dholo, sizeh, cudaMemcpyDeviceToHost);
	
	k = 0;
	for (int i = 0; i < 1024; i++)
	{
		uchar* tu = out.ptr<uchar>(i);

		for (int j = 0; j < 1024; j++)
		{
			tu[j] = floor(hholo[k] * 255.0);
			k++;
		}
	}
	imshow("111", out);
	//imwrite("C:/Users/zcl/Desktop/quanxi4001.png", out);
	
	waitKey(0);
	return(0);


}