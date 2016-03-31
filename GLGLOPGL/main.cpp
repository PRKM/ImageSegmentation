#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//void showHist(char* winName, Mat3b image);
void mkList(char* winName, Mat3b image);

int main() {
	Mat3b target;
	//Mat1b output;

	string impath;

	cout << "Drag Image Here : ";

	getline(cin, impath);

	target = imread(impath, 1);

	if (!target.data) {
		cout << "Can't load Image" << endl;
		return 1;
	}

	cout << "Width : " << target.cols << endl;
	cout << "Height : " << target.rows << endl;

	namedWindow("rgb", CV_WINDOW_AUTOSIZE);
	imshow("rgb", target);

	mkList("Hist", target);
	//showHist("hist", target);

	waitKey(0);
	return 0;
}

/*void showHist(char* winName, Mat3b image) {
	int bins = 256;
	int histSize[] = { bins };

	float lranges[] = { 0, bins };
	const float* ranges[] = { lranges };

	Mat hist;
	int channels[] = { 0 };

	int const hist_height = 256;
	Mat3b hist_image = Mat3b::zeros(hist_height, bins);

	calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

	double max_val = 0;
	minMaxLoc(hist, 0, &max_val);

	for (int b = 0; b < bins; b++) {
		float const binVal = hist.at<float>(b);
		int   const height = cvRound(binVal*hist_height / max_val);
		cv::line
			(hist_image
				, cv::Point(b, hist_height - height), cv::Point(b, hist_height)
				, cv::Scalar::all(255)
				);
	}

	imshow(winName, hist_image);
}*/

void mkList(char* winName, Mat3b image) { // 수평 히스토그램
	Mat3b hist = Mat3b::zeros(image.rows, image.cols/2); // 히스토그램을 그릴 인스턴스

	Point* startP = new Point[image.rows];
	Point* endP = new Point[image.rows]; //각각 시작점과 끝점. 이미지 자르기용이다.

	bool areaStarted = false; // 글자 구역 플래그

	for (int i = 0; i < image.rows; i++) {
		int count = 0; // 검정색의 빈도, 히스토그램 그리는데 사용된다.
		for (int j = 0; j < image.cols; j++) { // 왼쪽부터 오른쪽으로 픽셀들을 검사
			Vec3b pix = image.at<Vec3b>(i, j);
			if (pix[0] < 40 && pix[1] < 40 && pix[2] < 40) { // 픽셀이 검은색계열이면
				if (!areaStarted) { // 시작부일시
					areaStarted = true;
					startP[i] = Point(0, i); // 시작 점을 정한다.
				}
				count++; // 빈도값을 올린다.
			}
			else // 글자 영역이 아니면
				if (areaStarted) { // 플래그가 true이면
					areaStarted = false;
					endP[i] = Point(image.cols, i); // 끝점을 정한다.
				}
		}
		if (areaStarted) { // 예외로 플래그가 false가 안된 경우
			areaStarted = false;
			endP[i] = Point(image.cols, i);
		}
		line(hist, Point(0, i), Point((count - 1)/3, i), Scalar::all(0), 1, 8, 0); // 검은색 선으로 해당 열의 검정색 빈도수 나타냄
		line(hist, Point(count/3, i), Point(hist.cols - 1, i), Scalar::all(254), 1, 8, 0); // 나머지는 하얀색
	}

	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	imshow("Histogram", hist);
}