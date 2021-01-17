
#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

const unsigned short BufLen = 10;	//����������
const unsigned short ProducerCnt = 2;	//�����߸���
const unsigned short ConsumerCnt = 3;	//�����߸���
const unsigned sleepTime = 1000;	//ÿ������������ʱ��

int produnctId1 = 1000, produnctId2 = 2000;	//2�������ߵ�������Ʒ��ʼ��
int consumeId;	//���ѵĲ�Ʒ��
int buf[BufLen];	//������
int pIdx = 0, cIdx = 0;	//�ֱ��¼��������ź����ѵ����
bool goon = true;	//�����˳��ı�־
HANDLE sFull, sEmpty;	//�ź���
CRITICAL_SECTION cs;	//�ٽ���

void produce(int no);
void consume();
DWORD WINAPI producer(LPVOID lpPara);
DWORD WINAPI consumer(LPVOID lpPara);

//������
DWORD WINAPI producer(LPVOID lpPara) {
	int no = int(lpPara);	//�����߱��
	while (goon) {
		auto step = rand() % 900 + 100;
		WaitForSingleObject(sEmpty, INFINITE);	//Ҫ�󻺳������п�λ
		//INFINITE:���󱻴����źź����Ż᷵��
		EnterCriticalSection(&cs);		//����ʹ�û�����	
		Sleep(step);
		produce(no);
		LeaveCriticalSection(&cs);
		ReleaseSemaphore(sFull, 1, NULL);	//���������ݸ���+1
	}
	return 0;
}

//������
DWORD WINAPI consumer(LPVOID lpPara) {
	while (goon) {
		auto step = rand() % 900 + 100;
		WaitForSingleObject(sFull, INFINITE);
		EnterCriticalSection(&cs);
		Sleep(step);
		consume();
		LeaveCriticalSection(&cs);
		ReleaseSemaphore(sEmpty, 1, NULL);
	}
	return 0;
}

//����
void produce(int no) {
	auto produnctId = no == 0 ? produnctId1++ : produnctId2++;
	cout << "������Ʒ: " << ++produnctId << endl;
	cout << "����Ʒ���뻺����\n";
	buf[pIdx] = produnctId;
	cout << "������״̬:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == pIdx) cout << "<-����";
	}
	cout << endl << endl;
	pIdx = (pIdx + 1) % BufLen;
	Sleep(sleepTime);
}

//����
void consume() {
	cout << "�ӻ�����ȡ����Ʒ\n";
	consumeId = buf[cIdx];
	cout << "������״̬:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == cIdx) cout << "<-����";
	}
	cout << endl;
	buf[cIdx] = 0;
	cIdx = (cIdx + 1) % BufLen;
	cout << "���Ѳ�Ʒ: " << consumeId << endl << endl;
	Sleep(sleepTime);
}

int main() {
	srand(unsigned(time(nullptr)));
	//�����ź������ٽ���
	sFull = CreateSemaphore(NULL, 0, BufLen, NULL);		//���������ݸ���
	sEmpty = CreateSemaphore(NULL, BufLen, BufLen, NULL);	//��������λ����
	//sMutex = CreateSemaphore(NULL, 1, 1, NULL);
	InitializeCriticalSection(&cs);
	//�����������߳�
	HANDLE hThread[ProducerCnt+ConsumerCnt];
	DWORD producers[ProducerCnt], consumers[ConsumerCnt];
	for (int i = 0; i < ProducerCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, producer, LPVOID(i), 0, &producers[i]);
		if (!hThread[i]) return -1;
	}
	//�����������߳�
	for (int i = 0; i < ConsumerCnt; ++i) {
		hThread[ProducerCnt + i] 
			= CreateThread(NULL, 0, consumer, NULL, 0, &consumers[i]);
		if (!hThread[ProducerCnt + i]) return -1;
	}
	//���������ַ���ֹ
	while (goon) {
		if (getchar()) goon = false;
	}
	return 0;
}

/**/