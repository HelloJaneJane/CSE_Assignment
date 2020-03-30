//file I/O and resize with interpolation
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void BilinearInterpolation(Mat &src, Mat &dst, double Hrate, double Wrate);

Mat src, bi_dst;

int main() {

	// 이미지 읽어오기 (없으면 에러)
	src = imread("C:/Users/JaneYi/source/repos/openCVtest/openCVtest/example.jpeg", 1);  //절대 경로 혹은 상대 경로(0은 gray scale)
	if (src.empty()) {
		cout << "Cannot find an image" << endl;
		return -1;
	}

	// 원래 이미지 보기
	imshow("SourceImage", src);
	// 현재 사이즈 출력
	std::cout << "(src) height * width = " << src.rows << "*" << src.cols << "\n";
	// 입력 들어올 때까지 기다림
	waitKey(0);

	int newH, newW;
	cout << "input ---- new height, width :";
	cin >> newH >> newW;

	// resize하기
	double Hrate = (double)newH / (double)src.rows;
	double Wrate = (double)newW / (double)src.cols;
	std::cout << "-> scale rate : height=" << Hrate << ", width=" << Wrate << "\n";
	bi_dst = Mat(src.rows*Hrate, src.cols*Wrate, src.type(), Scalar(0)); // 결과물을 담을 Mat

	// bilinear interpolation
	BilinearInterpolation(src, bi_dst, Hrate, Wrate);

	// 바뀐 사이즈 출력
	std::cout << "(dst) height * width = " << bi_dst.rows << "*" << bi_dst.cols << "\n";
	// 바뀐 이미지 보기
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

			// src에서 픽셀 뽑아오기
			int px = (int)(x / Wrate);
			int py = (int)(y / Hrate);

			// RGB 정보가 다 들어있는 픽셀에 접근하기 위해 Vec3b사용
			// 3개의 unsigned char를 갖는 벡터 
			Vec3b P1 = src.at<Vec3b>(py, px);
			Vec3b P2 = src.at<Vec3b>(py, px + 1);
			Vec3b P3 = src.at<Vec3b>(py + 1, px);
			Vec3b P4 = src.at<Vec3b>(py + 1, px + 1);

			// 비율 계산
			double dx = (double)x / (double)Wrate - (double)px;
			double dy = (double)y / (double)Hrate - (double)py;

			double w1 = (double)(1 - dx) * (double)(1 - dy);
			double w2 = dx * (double)(1 - dy);
			double w3 = (double)(1 - dx) * dy;
			double w4 = dx * dy;

			// dst 픽셀에 넣을 값
			dst.at<Vec3b>(y, x) = w1 * P1 + w2 * P2 + w3 * P3 + w4 * P4;
		}
	}
}