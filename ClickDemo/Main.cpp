#include <opencv2/opencv.hpp>

#include "Engine/Engine.h"
#include "Level/DemoLevel.h"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <commdlg.h>
#include <cstdio>
#include <locale>
#include <codecvt>

using namespace std;

std::wstring OpenFileDialog()
{
	OPENFILENAME ofn;       // 구조체
	wchar_t szFile[MAX_PATH] = { 0 };  // 선택한 파일 경로를 저장할 버퍼

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; // 부모 윈도우 핸들 (콘솔 앱이라 NULL)
	ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";  // 파일 필터
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST; // 존재하는 파일만 선택 가능

	if (GetOpenFileName(&ofn)) {
		return std::wstring(szFile); // 선택한 파일 경로 반환
	}
	return L""; // 취소 시 빈 문자열 반환
}


int main()
{
	// 열의 개수
	const int width = 192;

	// 행의 개수
	const int height = 60;

	std::string filename;

	// 	srand(static_cast<unsigned int>(time(nullptr)));
	// 
	// 	int num = rand() % 3 + 1;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string str = converter.to_bytes(OpenFileDialog());

	filename = str;

	cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

	cv::Mat newImage;

	cv::Size size(width, height);

	cv::resize(image, newImage, size);

	cv::Mat binaryImage;

	cv::threshold(newImage, binaryImage, 128, 255, cv::THRESH_BINARY);

	ofstream file("map.txt");
	if (!file) {
		cerr << "파일 생성에 실패했습니다." << endl;
		return 1;
	}

	// 맵 생성: i는 행, j는 열
	for (int i = 0; i < binaryImage.rows; i++)
	{
		for (int j = 0; j < binaryImage.cols; j++)
		{
			file << (binaryImage.at<uchar>(i, j) == 255 ? "0" : "1");
		}
		file << "\n";
	}

	file.close();

	// 랜덤 맵 생성 로직
// 외곽은 모두 1로 설정
// srand(static_cast<unsigned int>(time(nullptr)));
// if (y == 0 || y == height - 1 || x == 0 || x == width - 1) 
// {
//		file << "1";
// 	}
// 	else {
// 		// 내부: 15% 확률로 장애물(1), 그 외는 0(이동 가능)
// 		int r = rand() % 100;
// 		// 15% 확률
// 		if (r < 15) {
// 			file << "1";
// 		}
// 		else 
//		{
// 			file << "0";
// 		}
// 	}
	// 	// 화면에 보이는 콘솔 창(윈도우) 핸들
	HWND consoleWindow = GetConsoleWindow();

	int screenWidth = GetSystemMetrics(0);
	int screenHeight = screenWidth / 16 * 9;

	MoveWindow(consoleWindow, (screenWidth - 450) / 2, (screenHeight - 450) / 2, 450, 450, true);

	Engine engine;
	engine.LoadLevel(new DemoLevel());
	engine.Run();
}

//// #include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <stdio.h>
//
//using namespace cv;
//using namespace std;
//
//int main(int, char**)
//{
//	Mat frame;
//	//--- INITIALIZE VIDEOCAPTURE
//	VideoCapture cap;
//	// open the default camera using default API
//	// cap.open(0);
//	// OR advance usage: select any API backend
//	int deviceID = 0;             // 0 = open default camera
//	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
//	// open selected camera using selected API
//	cap.open(deviceID, apiID);
//	// check if we succeeded
//	if (!cap.isOpened()) {
//		cerr << "ERROR! Unable to open camera\n";
//		return -1;
//	}
//
//	//--- GRAB AND WRITE LOOP
//	cout << "Start grabbing" << endl
//		<< "Press any key to terminate" << endl;
//	for (;;)
//	{
//		// wait for a new frame from camera and store it into 'frame'
//		cap.read(frame);
//		// check if we succeeded
//		if (frame.empty()) {
//			cerr << "ERROR! blank frame grabbed\n";
//			break;
//		}
//		// show live and wait for a key with timeout long enough to show images
//		imshow("Live", frame);
//		if (waitKey(5) >= 0)
//			break;
//	}
//	// the camera will be deinitialized automatically in VideoCapture destructor
//	return 0;
//}