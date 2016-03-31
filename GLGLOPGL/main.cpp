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

void mkList(char* winName, Mat3b image) { // ���� ������׷�
	Mat3b hist = Mat3b::zeros(image.rows, image.cols/2); // ������׷��� �׸� �ν��Ͻ�

	Point* startP = new Point[image.rows];
	Point* endP = new Point[image.rows]; //���� �������� ����. �̹��� �ڸ�����̴�.

	bool areaStarted = false; // ���� ���� �÷���

	for (int i = 0; i < image.rows; i++) {
		int count = 0; // �������� ��, ������׷� �׸��µ� ���ȴ�.
		for (int j = 0; j < image.cols; j++) { // ���ʺ��� ���������� �ȼ����� �˻�
			Vec3b pix = image.at<Vec3b>(i, j);
			if (pix[0] < 40 && pix[1] < 40 && pix[2] < 40) { // �ȼ��� �������迭�̸�
				if (!areaStarted) { // ���ۺ��Ͻ�
					areaStarted = true;
					startP[i] = Point(0, i); // ���� ���� ���Ѵ�.
				}
				count++; // �󵵰��� �ø���.
			}
			else // ���� ������ �ƴϸ�
				if (areaStarted) { // �÷��װ� true�̸�
					areaStarted = false;
					endP[i] = Point(image.cols, i); // ������ ���Ѵ�.
				}
		}
		if (areaStarted) { // ���ܷ� �÷��װ� false�� �ȵ� ���
			areaStarted = false;
			endP[i] = Point(image.cols, i);
		}
		line(hist, Point(0, i), Point((count - 1)/3, i), Scalar::all(0), 1, 8, 0); // ������ ������ �ش� ���� ������ �󵵼� ��Ÿ��
		line(hist, Point(count/3, i), Point(hist.cols - 1, i), Scalar::all(254), 1, 8, 0); // �������� �Ͼ��
	}

	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	imshow("Histogram", hist);
}