
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <map>

using namespace std;

const int pageSize = 16;	//ҳ���С
const int instrCnt = pageSize * 150;		//ָ����Ŀ
const int pageFrameCnt = 3;		//ҳ����

int orderCnt;		//���ʴ���
int cnt = 0;	//��ǰҳ����
int pageMissCnt = 0;	//ȱҳ��
int processArr[instrCnt];		//��������
int pageFrame[pageFrameCnt][pageSize];		//ҳ������
int orderArr[100];		//ָ���������
int pageIdx[pageFrameCnt];	//ҳ���д��ҳ��

//��������
const int order1[] = {
	112,455,600,1100,112,456,2000,112,455,600,1100,2001
};
const int order2[] = {
	112,113,455,456,600,601,1100,1101,1102,112,113,114,115,
	456,457,458,2000,2001,2002,2003,112,113,455,456,600,601,
	1100,1101,1102,2001,2002,2003
};


//��ʼ������
inline void initProcess() {
	for (int i = 0; i < instrCnt; ++i) {
		processArr[i] = rand() % 2000;
	}
}


//��ҳ��ΪpageNoҳ������ݸ��Ƶ���pfIdx��ҳ����
inline void copyPage(int pfIdx, int pageNo) {
	//��������
	memcpy(pageFrame[pfIdx],
		processArr + pageNo * pageSize, pageSize * sizeof(int));
	pageIdx[pfIdx] = pageNo;	//��¼ҳ��
}


//�����������
inline void showOrderArray() {
	cout << "[ҳ��]��������:";
	for (int i = 0; i < orderCnt; ++i)
		cout << '[' << orderArr[i] / pageSize << ']'
		<< orderArr[i] << ' ';
	putchar('\n');
}

void showPageFrame();
void eliminateFIFO();
void eliminateLRU();
void eliminateOPT();
void initInstrOrder(const int* srcArr = nullptr, int size=0);

int main() {
	srand(time(nullptr));
	initProcess();	//��ʼ������
	//��ʼ��ָ���������
	initInstrOrder(order1, sizeof(order1)/sizeof(int));
	//�����������
	showOrderArray();
	//��̭�㷨
	//eliminateFIFO();
	eliminateLRU();
	//eliminateOPT();
	return 0;
}

//��ʼ����������
void initInstrOrder(const int* srcArr,int size) {
	//���д��������������
	if (srcArr) {
		orderCnt = size;
		for (int i = 0; i < orderCnt; ++i) orderArr[i] = srcArr[i];
		return;
	}
	//�����������
	orderCnt = 20;
	for (int i = 0; i < orderCnt; ++i) {
		orderArr[i] = rand() % instrCnt;
	}
}

//���ҳ��״̬
void showPageFrame() {
	cout << "ҳ��״̬:\n";
	int i;
	for (i = 0; i < cnt; ++i) {
		cout << "ҳ��" << i + 1 << "  ҳ��:" << pageIdx[i] << "  ����:";
		for (int j = 0; j < pageSize; ++j) {
			cout << pageFrame[i][j] << ' ';
		}
		putchar('\n');
	}
	for (; i < pageFrameCnt; ++i) {
		cout << "ҳ��" << i + 1 << " ��\n";
	}
	putchar('\n');
}

//FIFO��̭�㷨
void eliminateFIFO() {
	int front = 0;	//��¼����ҳ���
	for (int i = 0; i < orderCnt; ++i) {
		auto pageNo = orderArr[i] / pageSize;	//ҳ��
		auto offset = orderArr[i] % pageSize;	//ҳ��ƫ��
		cout << "��ǰ���������: " << orderArr[i]
			<< "\t����ҳ: " << pageNo
			<< "\tֵ: " << processArr[orderArr[i]] << endl;

		int j;
		//����ҳ��������
		for (j = 0; j < cnt; ++j) {
			if (pageIdx[j] == pageNo) {
				cout << "���� " << pageFrame[j][offset] << endl;
				break;
			}
		}
		//��δ����
		if (j == cnt) {
			cout << "ȱҳ ";
			++pageMissCnt;	//ȱҳ����+1
			//��ҳ����ȫռ��
			if (cnt == pageFrameCnt) {
				copyPage(front, pageNo);
				cout << pageFrame[front][offset] << endl;
				front = (front + 1) % pageFrameCnt;
			}
			//ҳ��δȫ��ռ����ֱ�ӽ�ҳ���Ƶ���ҳ��
			else {
				copyPage(cnt, pageNo);
				cout << pageFrame[cnt][offset] << endl;
				++cnt;
			}
		}
		showPageFrame();
	}
	cout << "�����ܴ���:" << orderCnt << " ȱҳ����:" << pageMissCnt
		<< " ȱҳ��:" << float(pageMissCnt) / orderCnt << endl;
}

//LRU��̭�㷨
void eliminateLRU() {
	int timer[pageFrameCnt];
	memset(timer, 0, sizeof(timer));
	for (int i = 0; i < orderCnt; ++i) {
		auto pageNo = orderArr[i] / pageSize;	//ҳ��
		auto offset = orderArr[i] % pageSize;	//ҳ��ƫ��
		cout << "��ǰ���������: " << orderArr[i]
			<< "\t����ҳ: " << pageNo
			<< "\tֵ: " << processArr[orderArr[i]] << endl;

		int j;
		//����ҳ��������
		for (j = 0; j < cnt; ++j) {
			if (pageIdx[j] == pageNo) {
				cout << "���� " << pageFrame[j][offset] << endl;
				timer[j] = 0;
				break;
			}
		}
		//��δ����
		if (j == cnt) {
			cout << "ȱҳ ";
			++pageMissCnt;	//ȱҳ����+1
			//��ҳ����ȫռ��
			if (cnt == pageFrameCnt) {
				auto maxT = 0;
				//�ҵ�δʹ��ʱ�����ҳ�������̭
				for (int k = 0; k < pageFrameCnt; ++k) {
					if (timer[k] > timer[maxT]) maxT = k;
				}
				copyPage(maxT, pageNo);
				timer[maxT] = 0;
				cout << pageFrame[maxT][offset] << endl;
			}
			//ҳ��δȫ��ռ����ֱ�ӽ�ҳ���Ƶ���ҳ��
			else {
				copyPage(cnt, pageNo);
				cout << pageFrame[cnt][offset] << endl;
				++cnt;
			}
		}
		for (int j = 0; j < cnt; ++j) ++timer[j];
		showPageFrame();
	}
	cout << "�����ܴ���:" << orderCnt << " ȱҳ����:" << pageMissCnt
		<< " ȱҳ��:" << float(pageMissCnt) / orderCnt << endl;
}

//OPT��̭�㷨
void eliminateOPT() {
	//������Ϣ��: ����Ϊ�����ʵ�ҳ��,��ֵΪһ�����и�ҳ�ŷ��ʴ���Ķ�ջ
	map<int,stack<int>> ms;
	//���������������
	for (int i = orderCnt - 1; i >= 0; --i) {
		auto pageNo = orderArr[i] / pageSize;	//ҳ��
		//����ҳδ������
		if (ms.count(pageNo) == 0) {
			stack<int> tmp;		//������ջ
			tmp.push(i);	//�ڶ�ջ����ӷ��ʴ���
			ms.insert(pair<int,stack<int>>(pageNo, tmp));
		}
		else {
			ms.at(pageNo).push(i);	//�ڶ�ջ����ӷ��ʴ���
		}
	}
	//˳��ִ��
	for (int i = 0; i < orderCnt; ++i) {
		auto pageNo = orderArr[i] / pageSize;	//ҳ��
		auto offset = orderArr[i] % pageSize;	//ҳ��ƫ��
		cout << "��ǰ���������: " << orderArr[i]
			<< "\t����ҳ: " << pageNo
			<< "\tֵ: " << processArr[orderArr[i]] << endl;

		//ÿ��ִ���꽫��ҳջ���Ĵ�����Ϣ��ջ
		ms.at(pageNo).pop();
		int j;
		//����ҳ��������
		for (j = 0; j < cnt; ++j) {
			if (pageIdx[j] == pageNo) {
				cout << "���� " << pageFrame[j][offset] << endl;
				break;
			}
		}
		//��δ����
		if (j == cnt) {
			cout << "ȱҳ ";
			++pageMissCnt;	//ȱҳ����+1
			//��ҳ����ȫռ��
			if (cnt == pageFrameCnt) {
				auto maxT = 0;
				//����ҳ����ݴ�����ϢѰ�Ҳ�����Ҫ����Զ�Ľ������õ�ҳ��
				for (int k = 0; k < pageFrameCnt; ++k) {
					if (ms.at(pageIdx[k]).size() == 0) {
						maxT = k;
						break;
					}
					else if(ms.at(pageIdx[k]).top() > ms.at(pageIdx[maxT]).top()) {
						maxT = k;
					}
				}
				//��̭ҳ�渴��������
				copyPage(maxT, pageNo);
				cout << pageFrame[maxT][offset] << endl;
			}
			//ҳ��δȫ��ռ����ֱ�ӽ�ҳ���Ƶ���ҳ��
			else {
				copyPage(cnt, pageNo);
				cout << pageFrame[cnt][offset] << endl;
				++cnt;
			}
		}
		//���ÿ�ε�ҳ����Ϣ
		showPageFrame();
	}
	cout << "�����ܴ���:" << orderCnt << " ȱҳ����:" << pageMissCnt
		<< " ȱҳ��:" << float(pageMissCnt) / orderCnt << endl;
}

/**/