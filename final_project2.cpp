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
			cout << "���ϸ� �Է� : ";
			cin >> name;
			name += ".jpg";
			imwrite(name, save);
			cout << name << "������ �����" << endl;
		}
		else if (Load.contains(Point(x, y))) {
			string name;
			cout << "���ϸ� �Է� : ";
			cin >> name;
			name += ".jpg";
			Mat number = imread(name);
			number.copyTo(src(Rect(0, 0, 500, 500)));
			cout << name << "������ �ҷ���" << endl;
		}
		else if (Clear.contains(Point(x, y))) {
			rectangle(src, Rect(0, 0, 500, 500), Scalar(255, 255, 255), -1);
			cout << "�Է�â ������" << endl;
		}
		else if (Exit.contains(Point(x, y))) {
			cout << "���α׷� ����" << endl;
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
				if (Ratio1 > 3.5) { cout << "1" << endl; } //�ܰ����� 1���� ���� 1, 2, 3, 4, 5, 7 �� ���� ���̰� ��Ȯ�� ���� 1��(���� 3.5 �̻��̸� 1)
				else {
					Mat temp1 = cen(Rect(1, 1, 497, 497)).clone();
					resize(temp1, temp1, Size(500, 500));
					Mat limg1(Size(temp1.cols, temp1.rows), CV_8UC3, Scalar(0, 0, 0));
					temp1 /= 2;
					line(limg1, Point(Box[0].x + 5, Box[0].y + Box[0].height / 2 - 20), Point(Box[0].x + Box[0].width - 5, Box[0].y + Box[0].height / 2 - 20), Scalar(200, 200, 200), 2); //���μ�
					cvtColor(temp1, temp1, COLOR_BGR2GRAY);
					cvtColor(limg1, limg1, COLOR_BGR2GRAY);
					addWeighted(temp1, 1, limg1, 1, 0, temp1);
					threshold(temp1, temp1, 250, 255, THRESH_BINARY);
					Mat label1, stat1, centroid1;
					int count1 = connectedComponentsWithStats(temp1, label1, stat1, centroid1);
					count1 -= 1;
					if (count1 == 2) {
						cout << "4" << endl; // ���� 2, 3, 4, 5, 7 �� ���μ��� ������ 2���� ����Ǵ� ���� 4�� (�ٿ���ڽ� �߾ӿ��� 15�ȼ� �� ���μ��� �´ݴ� ������ 2���Ͻ� 4)
					}
					else if (count1 == 1) {
						// ���μ����� �Ǻ��� �ȵǸ� ���μ��� �׾ �Ǻ�
						Mat temp2 = cen(Rect(1, 1, 497, 497)).clone();
						resize(temp2, temp2, Size(500, 500));
						Mat limg2(Size(temp2.cols, temp2.rows), CV_8UC3, Scalar(0, 0, 0));
						temp2 /= 2;
						line(limg2, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + Box[0].width / 2, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); //���μ�
						cvtColor(temp2, temp2, COLOR_BGR2GRAY);
						cvtColor(limg2, limg2, COLOR_BGR2GRAY);
						addWeighted(temp2, 1, limg2, 1, 0, temp2);
						threshold(temp2, temp2, 250, 255, THRESH_BINARY);
						Mat label2, stat2, centroid2;
						int count2 = connectedComponentsWithStats(temp2, label2, stat2, centroid2);
						count2 -= 1;
						if (count2 == 1) {
							cout << "7" << endl; //���� 2, 3, 5, 7�� ���μ��� �׾����� ������ 1���� ����Ǵ� ���� 7��(�߾� ���μ��� �´ݴ� ������ 1���Ͻ� 7) 							
						}
						else if (count2 == 2) {
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // �� > �� �밢��
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1; //�߾� ���μ��� �׾����� �´ݴ� ������ 2���� ���� ���������� 2�� 7 
							if (count3 == 2) {
								cout << "2" << endl; //�����ʿ��� �������� �밢���� �׾����� �밢���� �´ݴ� ���� 2���� 2
							}
							else if (count3 == 1) {
								cout << "7" << endl; // �����ʿ��� �������� �밢���� �׾����� �밢���� �´ݴ� ���� 1���� 7
							}
						}
						else if (count2 == 3) {
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // �� > �� �밢��
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1;
							if (count3 == 2) {
								cout << "2" << endl; //2�ǰ�� ���μ��� �׾����� ������ 3���� ����� ���ɼ��� ����, ���μ��� �׾����� 3��, �����ʿ��� ���� �밢���� �׾����� 2���� 2
							}
							else if (count3 == 3) {
								Mat temp4 = cen(Rect(1, 1, 497, 497)).clone();
								resize(temp4, temp4, Size(500, 500));
								Mat limg4(Size(temp4.cols, temp4.rows), CV_8UC3, Scalar(0, 0, 0));
								temp4 /= 2;
								line(limg4, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + 5, Box[0].y + Box[0].height / 2), Scalar(200, 200, 200), 2); //x(�߾�), y(�ֻ��) > x(����), y(�߾�)
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
			cout << "�ܰ����� ����:" << contours.size() << endl;
			for (int i = 0; i < contours.size(); i++) {
				Scalar c(rand() & 255, rand() & 255, rand() & 255);
				drawContours(draw, contours, i, c, 2);
			}
			if (contours.size() == 3) { cout << "8" << endl; }
			else if (contours.size() == 2) { cout << "���� ���� : 0, 4, 6, 9" << endl; }
			else if (contours.size() == 1) { cout << "���� ���� : 1, 2, 3, 4, 5, 7" << endl; }
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
				cout << "�ܰ��� " << i + 1 << "�� �����߽�: (" << center[i].x << ", " << center[i].y << ")" << endl;
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
				cout << "���� ���� ���� : " << (double)Ratio1 << endl;
				if (Ratio1 > 3.5) { cout << "1" << endl; }
				else { cout << " ���� ����: 2, 3, 4, 5, 7" << endl; }
			}
			else if (contours.size() == 2) {
				double Ratio2 = (double)Box[1].height / (double)Box[1].width;
				cout << "���� : " << (double)Ratio2 << endl;
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
				cout << "�ܰ��� " << i + 1 << "�� �����߽�: (" << center[i].x << ", " << center[i].y << ")" << endl;
				circle(cen, center[i], 5, Scalar(0, 0, 255), -1);
			}
			if (contours.size() == 1) {
				double Ratio1 = (double)Box[0].height / (double)Box[0].width;
				cout << "���� ���� ���� : " << (double)Ratio1 << endl;
				if (Ratio1 > 3.5) { cout << "1" << endl; } //�ܰ����� 1���� ���� 1, 2, 3, 4, 5, 7 �� ���� ���̰� ��Ȯ�� ���� 1��(���� 3.5 �̻��̸� 1)
				else {
					Mat temp1 = cen(Rect(1, 1, 497, 497)).clone();
					resize(temp1, temp1, Size(500, 500));
					Mat limg1(Size(temp1.cols, temp1.rows), CV_8UC3, Scalar(0, 0, 0));
					temp1 /= 2;
					line(limg1, Point(Box[0].x + 5, Box[0].y + Box[0].height / 2 - 20), Point(Box[0].x + Box[0].width - 5, Box[0].y + Box[0].height / 2 - 20), Scalar(200, 200, 200), 2); //���μ�
					cvtColor(temp1, temp1, COLOR_BGR2GRAY);
					cvtColor(limg1, limg1, COLOR_BGR2GRAY);
					addWeighted(temp1, 1, limg1, 1, 0, temp1);
					threshold(temp1, temp1, 250, 255, THRESH_BINARY);
					Mat label1, stat1, centroid1;
					int count1 = connectedComponentsWithStats(temp1, label1, stat1, centroid1);
					count1 -= 1;
					if (count1 == 2) {
						cout << "���μ��� �����ϴ� ������ �� : 2 " << endl;
						cout << "4" << endl; // ���� 2, 3, 4, 5, 7 �� ���μ��� ������ 2���� ����Ǵ� ���� 4�� (�ٿ���ڽ� �߾ӿ��� 15�ȼ� �� ���μ��� �´ݴ� ������ 2���Ͻ� 4)
						imshow("temp1", temp1);
					}
					else if (count1 == 1) {
						// ���μ����� �Ǻ��� �ȵǸ� ���μ��� �׾ �Ǻ�
						Mat temp2 = cen(Rect(1, 1, 497, 497)).clone();
						resize(temp2, temp2, Size(500, 500));
						Mat limg2(Size(temp2.cols, temp2.rows), CV_8UC3, Scalar(0, 0, 0));
						temp2 /= 2;
						line(limg2, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + Box[0].width / 2, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); //���μ�
						cvtColor(temp2, temp2, COLOR_BGR2GRAY);
						cvtColor(limg2, limg2, COLOR_BGR2GRAY);
						addWeighted(temp2, 1, limg2, 1, 0, temp2);
						threshold(temp2, temp2, 250, 255, THRESH_BINARY);
						Mat label2, stat2, centroid2;
						int count2 = connectedComponentsWithStats(temp2, label2, stat2, centroid2);
						count2 -= 1;
						if (count2 == 1) {
							cout << "���μ��� �����ϴ� ������ �� : 1 " << endl;
							cout << "7" << endl; //���� 2, 3, 5, 7�� ���μ��� �׾����� ������ 1���� ����Ǵ� ���� 7��(�߾� ���μ��� �´ݴ� ������ 1���Ͻ� 7) 							
							imshow("temp2", temp2);
						}
						else if (count2 == 2) {
							cout << "���μ��� �����ϴ� ������ �� : 2 " << endl;
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // �� > �� �밢��
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1; //�߾� ���μ��� �׾����� �´ݴ� ������ 2���� ���� ���������� 2�� 7 
							if (count3 == 2) {
								cout << "2" << endl; //�����ʿ��� �������� �밢���� �׾����� �밢���� �´ݴ� ���� 2���� 2
								imshow("temp3", temp3);
							}
							else if (count3 == 1) {
								cout << "7" << endl; // �����ʿ��� �������� �밢���� �׾����� �밢���� �´ݴ� ���� 1���� 7
								imshow("temp3", temp3);
							}
						}
						else if (count2 == 3) {
							cout << "���μ��� �����ϴ� ������ �� : 3 " << endl;
							Mat temp3 = cen(Rect(1, 1, 497, 497)).clone();
							resize(temp3, temp3, Size(500, 500));
							Mat limg3(Size(temp3.cols, temp3.rows), CV_8UC3, Scalar(0, 0, 0));
							temp3 /= 2;
							line(limg3, Point(Box[0].x + Box[0].width * 3 / 4, Box[0].y + 5), Point(Box[0].x + Box[0].width * 1 / 4, Box[0].y + Box[0].height - 5), Scalar(200, 200, 200), 2); // �� > �� �밢��
							cvtColor(temp3, temp3, COLOR_BGR2GRAY);
							cvtColor(limg3, limg3, COLOR_BGR2GRAY);
							addWeighted(temp3, 1, limg3, 1, 0, temp3);
							threshold(temp3, temp3, 250, 255, THRESH_BINARY);
							Mat label3, stat3, centroid3;
							int count3 = connectedComponentsWithStats(temp3, label3, stat3, centroid3);
							count3 -= 1;
							if (count3 == 2) {
								cout << "�밢���� �����ϴ� ������ �� : 2 " << endl;
								cout << "2" << endl; //2�ǰ�� ���μ��� �׾����� ������ 3���� ����� ���ɼ��� ����, ���μ��� �׾����� 3��, �����ʿ��� ���� �밢���� �׾����� 2���� 2
								imshow("temp3", temp3);
							}
							else if (count3 == 3) {
								cout << "�밢���� �����ϴ� ������ �� : 3 " << endl;
								Mat temp4 = cen(Rect(1, 1, 497, 497)).clone();
								resize(temp4, temp4, Size(500, 500));
								Mat limg4(Size(temp4.cols, temp4.rows), CV_8UC3, Scalar(0, 0, 0));
								temp4 /= 2;
								line(limg4, Point(Box[0].x + Box[0].width / 2, Box[0].y + 5), Point(Box[0].x + 5, Box[0].y + Box[0].height / 2), Scalar(200, 200, 200), 2); //x(�߾�), y(�ֻ��) > x(����), y(�߾�)
								cvtColor(temp4, temp4, COLOR_BGR2GRAY);
								cvtColor(limg4, limg4, COLOR_BGR2GRAY);
								addWeighted(temp4, 1, limg4, 1, 0, temp4);
								threshold(temp4, temp4, 250, 255, THRESH_BINARY);
								Mat label4, stat4, centroid4;
								int count4 = connectedComponentsWithStats(temp4, label4, stat4, centroid3);
								count4 -= 1;
								if (count4 == 1) {
									cout << "���� ��� �밢���� �����ϴ� ������ �� : 1 " << endl;
									cout << "3" << endl; //���� �밢���� �����ϴ� ������ �� 1
									imshow("temp4", temp4);
								}
								else if (count4 == 2) {
									cout << "���� ��� �밢���� �����ϴ� ������ �� : 2 " << endl;
									cout << "5" << endl; //���� �밢���� �����ϴ� ������ �� 2
									imshow("temp4", temp4);
								}
							}
						}
					}
				}
			}
			else if (contours.size() == 2) {
				double Ratio2 = (double)Box[1].height / (double)Box[1].width;
				cout << "���� : " << (double)Ratio2 << endl;
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