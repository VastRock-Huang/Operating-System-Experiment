
#include <iostream>
#include <ctime>
using namespace std;

const int times = 1;	//����
const int row = 2048 * times;	//����
const int col = 1024 * times;	//����
int myArray[row][col];	//������

void travel1();
void travel2();

int main() {
	auto s = clock();
	travel2();
	auto e = clock();
	cout << "Cost " << e - s << "ms\n";	//������ʱ
	return 0;
}

//���к��б�������
void travel1() {
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			myArray[i][j] = 0;
		}
	}
}
//���к��б�������
void travel2() {
	for (int i = 0; i < col; ++i) {
		for (int j = 0; j < row; ++j) {
			myArray[j][i] = 0;
		}
	}
}


/**/