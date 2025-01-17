/*
By downloading, copying, installing or using the software you agree to this license.
If you do not agree to this license, do not download, install,
copy or use the software.


                  License Agreement For libfacedetection
                     (3-clause BSD License)

Copyright (c) 2018-2019, Shiqi Yu, all rights reserved.
shiqi.yu@gmail.com

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the names of the copyright holders nor the names of the contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are disclaimed.
In no event shall copyright holders or contributors be liable for any direct,
indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetectcnn.h"

//define the buffer size. Do not change the size!
#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s <image_file_name>\n", argv[0]);
		return -1;
	}

	//load an image and convert it to gray (single-channel)
	Mat image = imread(argv[1]); 
	if(image.empty())
	{
		fprintf(stderr, "Can not load the image file %s.\n", argv[1]);
		return -1;
	}

	int * pResults = NULL; 
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if(!pBuffer)
	{
		fprintf(stderr, "Can not alloc buffer.\n");
		return -1;
	}


	///////////////////////////////////////////
	// CNN face detection 
	// Best detection rate
	//////////////////////////////////////////
	//!!! The input image must be a BGR one (three-channel) instead of RGB
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);
	//pResults = facedetect_multiview_reinforce(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);

	printf("%d faces detected.\n", (pResults ? *pResults : 0));
	Mat result_cnn = image.clone();
	//print the detection results
	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short * p = ((short*)(pResults+1))+142*i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int confidence = p[4];
		int angle = p[5];

		char szConfidence[16];
		memset(szConfidence, sizeof(szConfidence), 0);
		sprintf(szConfidence, "%d", confidence);
		printf("face_rect=[%d, %d, %d, %d], confidence=%d, angle=%d\n", x,y,w,h,confidence, angle);
		rectangle(result_cnn, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

		// 增加置信度显示
		int font_face = cv::FONT_HERSHEY_COMPLEX;	// 字体
		double font_scale = 1;	// 大小
		int thickness = 2;	// 粗细
		int baseline;
		// 先获取字体框大小
		cv::Size text_size = cv::getTextSize(szConfidence, 
				font_face, font_scale, thickness, &baseline);
		// 显示字体
		putText(result_cnn, szConfidence, 
				Point(x, y + text_size.height), 
				font_face,
				font_scale, 
				// BGR
				Scalar(0, 0, 255), 
				thickness, 8);

		// 显示人脸特征点（最新的基于 CNN 完整开源无法显示）
		for (int j = 0; j < 68; j++)  
			circle(result_cnn, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(255, 0, 0),2); 
	}
	//namedWindow("result_cnn",WINDOW_NORMAL);		 // 自动缩放完整显示图像
													 // 可以鼠标拖拉缩放的窗口
	imshow("result_cnn", result_cnn);

	waitKey(0);

	//release the buffer
	free(pBuffer);

	return 0;
}
