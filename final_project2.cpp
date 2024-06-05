#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Point pt;
void on_mouse(int event, int x, int y, int flags, void* userdata);
void txt(Mat& src);
void box(Mat& src);
int main(void)
{
	Mat src(500, 950, CV_8UC3, Scalar(255, 255, 255));

	imshow("src", src);
	setMouseCallback("src", on_mouse, (void*)&src);
	txt(src);
	box(src);

	waitKey();
	return 0;
}
void on_mouse(int event, int x, int y, int flags, void* userdata)
{
	Mat src = *(Mat*)userdata;

	Rect Save(500, 0, 200, 100);
	Rect Load(500, 100, 200, 100);
	Rect Clear(500, 200, 200, 100);
	Rect Run(500, 300, 200, 100);
	Rect Exit(500, 400, 200, 100);
	Rect contours(700, 0, 250, 100);
	Rect feature2(700, 100, 250, 100);
	Rect feature3(700, 200, 250, 100);
	Rect feature4(700, 300, 250, 100);
	Rect feature5(700, 400, 250, 100);

	switch (event) {
	case EVENT_LBUTTONDOWN:
		pt = Point(x, y);
		if (Save.contains(Point(x, y))) {
			string name;
			Mat save = src(Rect(1, 1, 497, 497)).clone();
			resize(save, save, Size(500, 500));
			cout << "파일명 입력 : ";
			cin >> name;
			name += ".jpg";
			imwrite(name, save);
			cout << name << "파일이 저장됨" << endl;
		}
		else if (Load.contains(Point(x, y))) {
			string name;
			cout << "파일명 입력 : ";
			cin >> name;
			name += ".jpg";
			Mat number = imread(name);
			number.copyTo(src(Rect(0, 0, 500, 500)));
			cout << name << "파일을 불러옴" << endl;
		}
		else if (Clear.contains(Point(x, y))) {
			rectangle(src, Rect(0, 0, 500, 500), Scalar(255, 255, 255), -1);
			cout << "입력창 삭제됨" << endl;
		}
		else if (Exit.contains(Point(x, y))) {
			cout << "프로그램 종료" << endl;
			exit(1);
		}
		else if (Run.contains(Point(x, y))) {

		}
		else if (contours.contains(Point(x, y))) {
			Mat gray, thresh, morph;
			vector<vector<Point>> contours;
			Mat dst = src(Rect(1, 1, 497, 497)).clone();
			resize(dst, dst, Size(500, 500));
			cvtColor(dst, gray, COLOR_BGR2GRAY);
			threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
			morphologyEx(thresh, morph, MORPH_CLOSE, Mat(), Point(-1, -1), 10);
			findContours(morph, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
			cout << "외곽선의 갯수:" << contours.size() << endl;
		}
		else if (feature2.contains(Point(x, y))) {

		}
	case EVENT_LBUTTONUP:
		pt = Point(x, y);
		break;

	case EVENT_MOUSEMOVE:
		if (flags & EVENT_FLAG_LBUTTON) {
			if (x < 500) {
				line(src, pt, Point(x, y), Scalar(0, 0, 0), 10);
				pt = Point(x, y);
			}
		}
	}
	imshow("src", src);
}
void box(Mat& src)
{
	Rect Save(500, 0, 200, 100);
	Rect Load(500, 100, 200, 100);
	Rect Clear(500, 200, 200, 100);
	Rect Run(500, 300, 200, 100);
	Rect Exit(500, 400, 200, 100);
	Rect contours(700, 0, 250, 100);
	Rect feature2(700, 100, 250, 100);
	Rect feature3(700, 200, 250, 100);
	Rect feature4(700, 300, 250, 100);
	Rect feature5(700, 400, 250, 100);

	rectangle(src, Save, Scalar(0, 0, 0), 3);
	rectangle(src, Load, Scalar(0, 0, 0), 3);
	rectangle(src, Clear, Scalar(0, 0, 0), 3);
	rectangle(src, Run, Scalar(0, 0, 0), 3);
	rectangle(src, Exit, Scalar(0, 0, 0), 3);
	rectangle(src, contours, Scalar(0, 0, 0), 3);
	rectangle(src, feature2, Scalar(0, 0, 0), 3);
	rectangle(src, feature3, Scalar(0, 0, 0), 3);
	rectangle(src, feature4, Scalar(0, 0, 0), 3);
	rectangle(src, feature5, Scalar(0, 0, 0), 3);

	imshow("src", src);
}
void txt(Mat& src)
{
	putText(src, "Save", Point(525, 70), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "Load", Point(525, 170), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "Clear", Point(510, 270), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "Run", Point(535, 370), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "Exit", Point(530, 470), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "contours", Point(705, 65), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);
	putText(src, "feature2", Point(705, 165), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);
	putText(src, "feature3", Point(705, 265), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);
	putText(src, "feature4", Point(705, 365), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);
	putText(src, "feature5", Point(705, 465), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);

	imshow("src", src);
}