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
	Rect center(700, 100, 250, 100);
	Rect feature3(700, 200, 250, 100);
	Rect feature4(700, 300, 250, 100);
	Rect feature5(700, 400, 250, 100);

	switch (event) {
	case EVENT_LBUTTONDOWN:
		pt = Point(x, y);
		if (Save.contains(Point(x, y))) {
			string name;
			Mat save = src(Rect(1, 1, 498, 498)).clone();
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
			Mat gray, thresh, morph, cen;
			vector<vector<Point>> contours;
			Mat dst = src(Rect(1, 1, 497, 497)).clone();
			resize(dst, dst, Size(500, 500));
			cvtColor(dst, gray, COLOR_BGR2GRAY);
			threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
			morphologyEx(thresh, morph, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
			Mat labels, stats, centroids;
			int cnt = connectedComponentsWithStats(morph, labels, stats, centroids);
			findContours(morph, contours, RETR_LIST, CHAIN_APPROX_NONE);
			cvtColor(morph, cen, COLOR_GRAY2BGR);
			vector<Point> center;
			vector<Rect> Box;
			for (int i = 0; i < contours.size(); i++) {
				Point centroid(0, 0);
				Rect bound = boundingRect(contours[i]);
				for (int j = 0; j < contours[i].size(); j++) {
					centroid.x += contours[i][j].x;
					centroid.y += contours[i][j].y;
				}
				centroid.x /= contours[i].size();
				centroid.y /= contours[i].size();
				center.push_back(centroid);
				Box.push_back(bound);
				rectangle(cen, bound, Scalar(0, 255, 0), 2);
				circle(cen, center[i], 5, Scalar(0, 0, 255), -1);
			}
			if (contours.size() == 1) {
				double Ratio1 = (double)Box[0].height / (double)Box[0].width;
				if (Ratio1 > 3.5) { cout << "1" << endl; } //외곽선이 1개인 숫자 1, 2, 3, 4, 5, 7 중 비율 차이가 명확한 수는 1뿐(비율 3.5 이상이면 1)
				else {
					Mat temp1 = cen(Rect(1, 1, 497, 497)).clone();
					resize(temp1, temp1, Size(500, 500));
					Mat limg1(Size(temp1.cols, temp1.rows), CV_8UC3, Scalar(0, 0, 0));
					temp1 /= 2;
					line(limg1, Point(Box[0].x + 5, Box[0].y + Box[0].height / 2 - 20), Point(Box[0].x + Box[0].width - 5, Box[0].y + Box[0].height / 2 - 20), Scalar(200, 200, 200), 2); //가로선
					cvtColor(temp1, temp1, COLOR_BGR2GRAY);
					cvtColor(limg1, limg1, COLOR_BGR2GRAY);
					addWeighted(temp1, 1, limg1, 1, 0, temp1);
					threshold(temp1, temp1, 250, 255, THRESH_BINARY);
					Mat label1, stat1, centroid1;
					int count1 = connectedComponentsWithStats(temp1, label1, stat1, centroid1);
					count1 -= 1;
					if (count1 == 2) {
						cout << "4" << endl; // 숫자 2, 3, 4, 5, 7 중 가로선에 면적이 2개가 검출되는 수는 4뿐 (바운딩박스 중앙에서 15픽셀 위 가로선에 맞닫는 면적이 2개일시 4)
					}
					else if (count1 == 1) {
						// 가로선으로 판별이 안되면 세로선을 그어서 판별
						Mat temp2 = cen(Rect(1, 1, 497, 497)).clone();
						resize(temp2, temp2, Size(500, 500));
						Mat limg2(Size(temp2.cols, temp2.rows), CV_8UC3, Scalar(0, 0, 0));
						temp2 /= 2;
						line(limg2, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + Box[0].width / 2, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); //세로선
						cvtColor(temp2, temp2, COLOR_BGR2GRAY);
						cvtColor(limg2, limg2, COLOR_BGR2GRAY);
						addWeighted(temp2, 1, limg2, 1, 0, temp2);
						threshold(temp2, temp2, 250, 255, THRESH_BINARY);
						Mat label2, stat2, centroid2;
						int count2 = connectedComponentsWithStats(temp2, label2, stat2, centroid2);
						count2 -= 1;
						if (count2 == 1) {
							cout << "7" << endl; //숫자 2, 3, 5, 7중 세로선을 그었을때 면적이 1개가 검출되는 수는 7뿐(중앙 세로선에 맞닫는 면적이 1개일시 7) 							
						}
						else if (count2 == 2) {
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // 오 > 왼 대각선
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1; //중앙 세로선을 그었을때 맞닫는 면적이 2개인 숫자 모폴로지된 2와 7 
							if (count3 == 2) {
								cout << "2" << endl; //오른쪽에서 왼쪽으로 대각선을 그었을때 대각선과 맞닫는 면적 2개면 2
							}
							else if (count3 == 1) {
								cout << "7" << endl; // 오른쪽에서 왼쪽으로 대각선을 그었을때 대각선과 맞닫는 면적 1개면 7
							}
						}
						else if (count2 == 3) {
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // 오 > 왼 대각선
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1;
							if (count3 == 2) {
								cout << "2" << endl; //2의경우 세로선을 그었을때 면적이 3개가 검출될 가능성도 있음, 세로선을 그었을때 3개, 오른쪽에서 왼쪽 대각선을 그었을때 2개면 2
							}
							else if (count3 == 3) {
								Mat temp4 = cen(Rect(1, 1, 497, 497)).clone();
								resize(temp4, temp4, Size(500, 500));
								Mat limg4(Size(temp4.cols, temp4.rows), CV_8UC3, Scalar(0, 0, 0));
								temp4 /= 2;
								line(limg4, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + 5, Box[0].y + Box[0].height / 2), Scalar(200, 200, 200), 2); //x(중앙), y(최상단) > x(좌측), y(중앙)
								cvtColor(temp4, temp4, COLOR_BGR2GRAY);
								cvtColor(limg4, limg4, COLOR_BGR2GRAY);
								addWeighted(temp4, 1, limg4, 1, 0, temp4);
								threshold(temp4, temp4, 250, 255, THRESH_BINARY);
								Mat label4, stat4, centroid4;
								int count4 = connectedComponentsWithStats(temp4, label4, stat4, centroid3);
								count4 -= 1;
								if (count4 == 1) {
									cout << "3" << endl; // 
								}
								else if (count4 == 2) {
									cout << "5" << endl;

								}
							}
						}
					}
				}
			}
			else if (contours.size() == 2) {
				double Ratio2 = (double)Box[1].height / (double)Box[1].width;
				if ((center[0].y - center[1].y) < 10 && center[0].y - center[1].y > -10) { cout << "0" << endl; }
				else if (center[0].y > center[1].y) { cout << "6" << endl; }
				else if (center[0].y < center[1].y) {
					if (Ratio2 < 1.4) { cout << "4" << endl; }
					else { cout << "9" << endl; }
				}
			}
			else if (contours.size() == 3) { cout << "8" << endl; }

		}
		else if (contours.contains(Point(x, y))) {
			Mat gray, thresh, morph, draw;
			vector<vector<Point>> contours;
			Mat dst = src(Rect(1, 1, 497, 497)).clone();
			resize(dst, dst, Size(500, 500));
			cvtColor(dst, gray, COLOR_BGR2GRAY);
			threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
			morphologyEx(thresh, morph, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
			findContours(morph, contours, RETR_LIST, CHAIN_APPROX_NONE);
			cvtColor(morph, draw, COLOR_GRAY2BGR);
			cout << "외곽선의 갯수:" << contours.size() << endl;
			for (int i = 0; i < contours.size(); i++) {
				Scalar c(rand() & 255, rand() & 255, rand() & 255);
				drawContours(draw, contours, i, c, 2);
			}
			if (contours.size() == 3) { cout << "8" << endl; }
			else if (contours.size() == 2) { cout << "예상 숫자 : 0, 4, 6, 9" << endl; }
			else if (contours.size() == 1) { cout << "예상 숫자 : 1, 2, 3, 4, 5, 7" << endl; }
			imshow("count", draw);
		}
		else if (center.contains(Point(x, y))) {
			Mat gray, thresh, morph, cen;
			vector<vector<Point>> contours;
			Mat dst = src(Rect(1, 1, 497, 497)).clone();
			resize(dst, dst, Size(500, 500));
			cvtColor(dst, gray, COLOR_BGR2GRAY);
			threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
			morphologyEx(thresh, morph, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
			Mat labels, stats, centroids;
			int cnt = connectedComponentsWithStats(morph, labels, stats, centroids);
			findContours(morph, contours, RETR_LIST, CHAIN_APPROX_NONE);
			cvtColor(morph, cen, COLOR_GRAY2BGR);
			vector<Point> center;
			for (int i = 0; i < contours.size(); i++) {
				Point centroid(0, 0);
				for (int j = 0; j < contours[i].size(); j++) {
					centroid.x += contours[i][j].x;
					centroid.y += contours[i][j].y;
				}
				centroid.x /= contours[i].size();
				centroid.y /= contours[i].size();
				center.push_back(centroid);
				cout << "외곽선 " << i + 1 << "의 무게중심: (" << center[i].x << ", " << center[i].y << ")" << endl;
				circle(cen, center[i], 5, Scalar(0, 0, 255), -1);
			}
			if (contours.size() == 2) {
				if ((center[0].y - center[1].y) < 10 && center[0].y - center[1].y > -10) { cout << "0" << endl; }
				else if (center[0].y > center[1].y) { cout << "6" << endl; }
				else if (center[0].y < center[1].y) { cout << "9" << endl; }
			}
			imshow("center", cen);
		}
		else if (feature3.contains(Point(x, y))) {
			Mat gray, thresh, morph, cen;
			vector<vector<Point>> contours;
			Mat dst = src(Rect(1, 1, 497, 497)).clone();
			resize(dst, dst, Size(500, 500));
			cvtColor(dst, gray, COLOR_BGR2GRAY);
			threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
			morphologyEx(thresh, morph, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
			Mat labels, stats, centroids;
			int cnt = connectedComponentsWithStats(morph, labels, stats, centroids);
			findContours(morph, contours, RETR_LIST, CHAIN_APPROX_NONE);
			cvtColor(morph, cen, COLOR_GRAY2BGR);
			vector<Point> center;
			vector<Rect> Box;
			for (int i = 0; i < contours.size(); i++) {
				Point centroid(0, 0);
				Rect bound = boundingRect(contours[i]);
				for (int j = 0; j < contours[i].size(); j++) {
					centroid.x += contours[i][j].x;
					centroid.y += contours[i][j].y;
				}
				centroid.x /= contours[i].size();
				centroid.y /= contours[i].size();
				center.push_back(centroid);
				Box.push_back(bound);
				rectangle(cen, bound, Scalar(0, 255, 0), 2);
			}
			if (contours.size() == 1) {
				double Ratio1 = (double)Box[0].height / (double)Box[0].width;
				cout << "가로 세로 비율 : " << (double)Ratio1 << endl;
				if (Ratio1 > 3.5) { cout << "1" << endl; }
				else { cout << " 예상 숫자: 2, 3, 4, 5, 7" << endl; }
			}
			else if (contours.size() == 2) {
				double Ratio2 = (double)Box[1].height / (double)Box[1].width;
				cout << "비율 : " << (double)Ratio2 << endl;
				if ((center[0].y - center[1].y) < 10 && center[0].y - center[1].y > -10) { cout << "0" << endl; }
				else if (center[0].y > center[1].y) { cout << "6" << endl; }
				else if (center[0].y < center[1].y) {
					if (Ratio2 < 1.4) { cout << "4" << endl; }
					else { cout << "9" << endl; }
				}
			}
			imshow("Ratio", cen);
		}
		else if (feature4.contains(Point(x, y))) {
			Mat gray, thresh, morph, cen;
			vector<vector<Point>> contours;
			Mat dst = src(Rect(1, 1, 497, 497)).clone();
			resize(dst, dst, Size(500, 500));
			cvtColor(dst, gray, COLOR_BGR2GRAY);
			threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
			morphologyEx(thresh, morph, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
			Mat labels, stats, centroids;
			int cnt = connectedComponentsWithStats(morph, labels, stats, centroids);
			findContours(morph, contours, RETR_LIST, CHAIN_APPROX_NONE);
			cvtColor(morph, cen, COLOR_GRAY2BGR);
			vector<Point> center;
			vector<Rect> Box;
			for (int i = 0; i < contours.size(); i++) {
				Point centroid(0, 0);
				Rect bound = boundingRect(contours[i]);
				for (int j = 0; j < contours[i].size(); j++) {
					centroid.x += contours[i][j].x;
					centroid.y += contours[i][j].y;
				}
				centroid.x /= contours[i].size();
				centroid.y /= contours[i].size();
				center.push_back(centroid);
				Box.push_back(bound);
				rectangle(cen, bound, Scalar(0, 255, 0), 2);
				cout << "외곽선 " << i + 1 << "의 무게중심: (" << center[i].x << ", " << center[i].y << ")" << endl;
				circle(cen, center[i], 5, Scalar(0, 0, 255), -1);
			}
			if (contours.size() == 1) {
				double Ratio1 = (double)Box[0].height / (double)Box[0].width;
				cout << "가로 세로 비율 : " << (double)Ratio1 << endl;
				if (Ratio1 > 3.5) { cout << "1" << endl; } //외곽선이 1개인 숫자 1, 2, 3, 4, 5, 7 중 비율 차이가 명확한 수는 1뿐(비율 3.5 이상이면 1)
				else {
					Mat temp1 = cen(Rect(1, 1, 497, 497)).clone();
					resize(temp1, temp1, Size(500, 500));
					Mat limg1(Size(temp1.cols, temp1.rows), CV_8UC3, Scalar(0, 0, 0));
					temp1 /= 2;
					line(limg1, Point(Box[0].x + 5, Box[0].y + Box[0].height / 2 - 20), Point(Box[0].x + Box[0].width - 5, Box[0].y + Box[0].height / 2 - 20), Scalar(200, 200, 200), 2); //가로선
					cvtColor(temp1, temp1, COLOR_BGR2GRAY);
					cvtColor(limg1, limg1, COLOR_BGR2GRAY);
					addWeighted(temp1, 1, limg1, 1, 0, temp1);
					threshold(temp1, temp1, 250, 255, THRESH_BINARY);
					Mat label1, stat1, centroid1;
					int count1 = connectedComponentsWithStats(temp1, label1, stat1, centroid1);
					count1 -= 1;
					if (count1 == 2) {
						cout << "가로선과 접촉하는 면적의 수 : 2 " << endl;
						cout << "4" << endl; // 숫자 2, 3, 4, 5, 7 중 가로선에 면적이 2개가 검출되는 수는 4뿐 (바운딩박스 중앙에서 15픽셀 위 가로선에 맞닫는 면적이 2개일시 4)
						imshow("temp1", temp1);
					}
					else if (count1 == 1) {
						// 가로선으로 판별이 안되면 세로선을 그어서 판별
						Mat temp2 = cen(Rect(1, 1, 497, 497)).clone();
						resize(temp2, temp2, Size(500, 500));
						Mat limg2(Size(temp2.cols, temp2.rows), CV_8UC3, Scalar(0, 0, 0));
						temp2 /= 2;
						line(limg2, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + Box[0].width / 2, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); //세로선
						cvtColor(temp2, temp2, COLOR_BGR2GRAY);
						cvtColor(limg2, limg2, COLOR_BGR2GRAY);
						addWeighted(temp2, 1, limg2, 1, 0, temp2);
						threshold(temp2, temp2, 250, 255, THRESH_BINARY);
						Mat label2, stat2, centroid2;
						int count2 = connectedComponentsWithStats(temp2, label2, stat2, centroid2);
						count2 -= 1;
						if (count2 == 1) {
							cout << "세로선과 접촉하는 면적의 수 : 1 " << endl;
							cout << "7" << endl; //숫자 2, 3, 5, 7중 세로선을 그었을때 면적이 1개가 검출되는 수는 7뿐(중앙 세로선에 맞닫는 면적이 1개일시 7) 							
							imshow("temp2", temp2);
						}
						else if (count2 == 2) {
							cout << "가로선과 접촉하는 면적의 수 : 2 " << endl;
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // 오 > 왼 대각선
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1; //중앙 세로선을 그었을때 맞닫는 면적이 2개인 숫자 모폴로지된 2와 7 
							if (count3 == 2) {
								cout << "2" << endl; //오른쪽에서 왼쪽으로 대각선을 그었을때 대각선과 맞닫는 면적 2개면 2
								imshow("temp3", temp3);
							}
							else if (count3 == 1) {
								cout << "7" << endl; // 오른쪽에서 왼쪽으로 대각선을 그었을때 대각선과 맞닫는 면적 1개면 7
								imshow("temp3", temp3);
							}
						}
						else if (count2 == 3) {
							cout << "가로선과 접촉하는 면적의 수 : 3 " << endl;
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // 오 > 왼 대각선
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1;
							if (count3 == 2) {
								cout << "대각선과 접촉하는 면적의 수 : 2 " << endl;
								cout << "2" << endl; //2의경우 세로선을 그었을때 면적이 3개가 검출될 가능성도 있음, 세로선을 그었을때 3개, 오른쪽에서 왼쪽 대각선을 그었을때 2개면 2
								imshow("temp3", temp3);
							}
							else if (count3 == 3) {
								cout << "대각선과 접촉하는 면적의 수 : 3 " << endl;
								Mat temp4 = cen(Rect(1, 1, 497, 497)).clone();
								resize(temp4, temp4, Size(500, 500));
								Mat limg4(Size(temp4.cols, temp4.rows), CV_8UC3, Scalar(0, 0, 0));
								temp4 /= 2;
								line(limg4, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + 5, Box[0].y + Box[0].height / 2), Scalar(200, 200, 200), 2); //x(중앙), y(최상단) > x(좌측), y(중앙)
								cvtColor(temp4, temp4, COLOR_BGR2GRAY);
								cvtColor(limg4, limg4, COLOR_BGR2GRAY);
								addWeighted(temp4, 1, limg4, 1, 0, temp4);
								threshold(temp4, temp4, 250, 255, THRESH_BINARY);
								Mat label4, stat4, centroid4;
								int count4 = connectedComponentsWithStats(temp4, label4, stat4, centroid3);
								count4 -= 1;
								if (count4 == 1) {
									cout << "좌측 상단 대각선과 접촉하는 면적의 수 : 1 " << endl;
									cout << "3" << endl; //좌측 대각선과 접촉하는 면적의 수 1
									imshow("temp4", temp4);
								}
								else if (count4 == 2) {
									cout << "좌측 상단 대각선과 접촉하는 면적의 수 : 2 " << endl;
									cout << "5" << endl; //좌측 대각선과 접촉하는 면적의 수 2
									imshow("temp4", temp4);
								}
							}
						}
					}
				}
			}
			else if (contours.size() == 2) {
				double Ratio2 = (double)Box[1].height / (double)Box[1].width;
				cout << "비율 : " << (double)Ratio2 << endl;
				if ((center[0].y - center[1].y) < 10 && center[0].y - center[1].y > -10) { cout << "0" << endl; }
				else if (center[0].y > center[1].y) { cout << "6" << endl; }
				else if (center[0].y < center[1].y) {
					if (Ratio2 < 1.4) { cout << "4" << endl; }
					else { cout << "9" << endl; }
				}
			}
			else if (contours.size() == 3) { cout << "8" << endl; }
			imshow("number", cen);
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
	Rect center(700, 100, 250, 100);
	Rect feature3(700, 200, 250, 100);
	Rect feature4(700, 300, 250, 100);
	Rect feature5(700, 400, 250, 100);

	rectangle(src, Save, Scalar(0, 0, 0), 3);
	rectangle(src, Load, Scalar(0, 0, 0), 3);
	rectangle(src, Clear, Scalar(0, 0, 0), 3);
	rectangle(src, Run, Scalar(0, 0, 0), 3);
	rectangle(src, Exit, Scalar(0, 0, 0), 3);
	rectangle(src, contours, Scalar(0, 0, 0), 3);
	rectangle(src, center, Scalar(0, 0, 0), 3);
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
	putText(src, "Contours", Point(705, 65), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);
	putText(src, "Center", Point(710, 165), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "Ratio", Point(710, 265), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "Line", Point(715, 365), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 0), 2);
	putText(src, "feature5", Point(705, 465), FONT_HERSHEY_TRIPLEX, 1.5, Scalar(0, 0, 0), 2);

	imshow("src", src);
}