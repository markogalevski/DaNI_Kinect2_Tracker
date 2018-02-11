// DaNI_Kinect2_Tracker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <Kinect.h>
#include <Windows.h>

#define STOP 's'
unsigned int bufferSize = 0;
unsigned char * pBuffer = nullptr;

IKinectSensor * sensorKinect; 
IColorFrameReader * colorReader;
IBodyFrameReader * bodyReader;
IColorFrame * colorFrame = NULL; //"frame" is a pointer which points to the address in which an IColorFrame object will be stored. It is 
								 // currently assigned to NULL location.
IBodyFrame * bodyFrame;


bool initKinectColor(int * rows, int * cols);

bool initKinectBody();

int main()
{
	int rows, cols;
	if (FAILED(initKinectColor(&rows, &cols))) {
		std::cerr << "Failed to initialise colour frame, exiting..." << std::endl;
		return 1;
	}
	if (FAILED(initKinectBody())) {
		std::cerr << "Failed to initialise body frame, exiting..." << std::endl;
		return 1;
		}
			
	
	cv::Mat ocvRGB(rows, cols, CV_8UC3);
	cv::namedWindow("AAAA", cv::WINDOW_AUTOSIZE);

	while (1) {
		if (SUCCEEDED(colorReader->AcquireLatestFrame(&colorFrame))) {

			if SUCCEEDED(colorFrame->AccessRawUnderlyingBuffer(&bufferSize, &pBuffer)) {
				cv::Mat ocvRAW(rows, cols, CV_8UC2, pBuffer);
				cv::cvtColor(ocvRAW, ocvRGB, CV_YUV2BGR_YUYV);
				cv::imshow("AAAA", ocvRGB);
				colorFrame->Release();
			}
		}
		if (cv::waitKey(10) == STOP)
			break;
	}
	cv::destroyAllWindows();

    return 0;
}


bool initKinectColor(int * rows, int * cols) {
	if (FAILED(GetDefaultKinectSensor(&sensorKinect))) 
		return false;

	if (sensorKinect) {
		sensorKinect->Open();
		
		IColorFrameSource* framesource = NULL;
		sensorKinect->get_ColorFrameSource(&framesource);
		framesource->OpenReader(&colorReader);
		if (framesource) {
			IFrameDescription * description;
			framesource->get_FrameDescription(&description);
			description->get_Width(cols);
			description->get_Height(rows);
			framesource->Release();
			framesource = NULL;
		}
		
		return true;


	}
	else
		return false;
}

bool  initKinectBody() {
	if (FAILED(GetDefaultKinectSensor(&sensorKinect)))
		return false;

	IBodyFrameSource * BFSource = NULL;
	sensorKinect->get_BodyFrameSource(&BFSource);
	BFSource->OpenReader(&bodyReader);
	BFSource->Release();
	BFSource = NULL;
	return true;
}