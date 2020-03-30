//file I/O and resize with interpolation
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void BilinearInterpolation(Mat &src, Mat &dst, double Hrate, double Wrate);

Mat src, bi_dst;

int main() {

	// �̹��� �о���� (������ ����)
	src = imread("C:/Users/JaneYi/source/repos/openCVtest/openCVtest/example.jpeg", 1);  //���� ��� Ȥ�� ��� ���(0�� gray scale)
	if (src.empty()) {
		cout << "Cannot find an image" << endl;
		return -1;
	}

	// ���� �̹��� ����
	imshow("SourceImage", src);
	// ���� ������ ���
	std::cout << "(src) height * width = " << src.rows << "*" << src.cols << "\n";
	// �Է� ���� ������ ��ٸ�
	waitKey(0);

	int newH, newW;
	cout << "input ---- new height, width :";
	cin >> newH >> newW;

	// resize�ϱ�
	double Hrate = (double)newH / (double)src.rows;
	double Wrate = (double)newW / (double)src.cols;
	std::cout << "-> scale rate : height=" << Hrate << ", width=" << Wrate << "\n";
	bi_dst = Mat(src.rows*Hrate, src.cols*Wrate, src.type(), Scalar(0)); // ������� ���� Mat

	// bilinear interpolation
	BilinearInterpolation(src, bi_dst, Hrate, Wrate);

	// �ٲ� ������ ���
	std::cout << "(dst) height * width = " << bi_dst.rows << "*" << bi_dst.cols << "\n";
	// �ٲ� �̹��� ����
	imshow("BIImage", bi_dst);

	waitKey(0);

	return 0;
}

// bilinear interpolation
void BilinearInterpolation(Mat &src, Mat &dst, double Hrate, double Wrate) {
	// (src) --[ * rate ]--> (dst)
	//	px,py				  x,y

	for (int y = 0; y < dst.rows; y++) {
		for (int x = 0; x < dst.cols; x++) {

			// src���� �ȼ� �̾ƿ���
			int px = (int)(x / Wrate);
			int py = (int)(y / Hrate);

			// RGB ������ �� ����ִ� �ȼ��� �����ϱ� ���� Vec3b���
			// 3���� unsigned char�� ���� ���� 
			Vec3b P1 = src.at<Vec3b>(py, px);
			Vec3b P2 = src.at<Vec3b>(py, px + 1);
			Vec3b P3 = src.at<Vec3b>(py + 1, px);
			Vec3b P4 = src.at<Vec3b>(py + 1, px + 1);

			// ���� ���
			double dx = (double)x / (double)Wrate - (double)px;
			double dy = (double)y / (double)Hrate - (double)py;

			double w1 = (double)(1 - dx) * (double)(1 - dy);
			double w2 = dx * (double)(1 - dy);
			double w3 = (double)(1 - dx) * dy;
			double w4 = dx * dy;

			// dst �ȼ��� ���� ��
			dst.at<Vec3b>(y, x) = w1 * P1 + w2 * P2 + w3 * P3 + w4 * P4;
		}
	}
}