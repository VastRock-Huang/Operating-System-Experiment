
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
using namespace std;

const unsigned pCnt = 5;
const unsigned sleepTime = 800;

bool goon = true;
int pS[pCnt];
HANDLE s[pCnt];
unsigned diningCnt = 0;
CRITICAL_SECTION cs;

inline void beforeDining(int i, int k) {
	printf("��ѧ��%d ����%d�ſ��� ����%d֧���� ", i, k, ++pS[i]);
	if (pS[i] == 1) printf("���ܽ���\n");
	else if (pS[i] == 2) printf("��ʼ����\n");
}

inline void afterDining(int i, int k) {
	printf("��ѧ��%d ����%d�ſ���\n", i, k);
	--pS[i];
}

inline void dining(int i) {
	printf("��ѧ��%i �Ͳ�\n", i);
	printf("[0��:%d] [1��:%d] [2��:%d] [3��:%d] [4��:%d]\n",
		pS[0],pS[1],pS[2],pS[3], pS[4]);
	Sleep(sleepTime);

}

DWORD WINAPI philosopher(LPVOID lpPara) {
	int i = int(lpPara);
	while (goon) {
		auto step = rand() % 400 + 100;
		Sleep(step);
		//�жϵ�ǰ�ÿ��ӵ�����, ���ﵽ"����-1"�������ÿ���
		if (diningCnt == pCnt - 1) continue;

		WaitForSingleObject(s[i], INFINITE);	//�ȴ������ӿ���

		EnterCriticalSection(&cs);
		++diningCnt;	//�ÿ�������+1
		LeaveCriticalSection(&cs);

		beforeDining(i, i);
		WaitForSingleObject(s[(i + pCnt - 1) % pCnt], INFINITE);	//�ȴ��Ҳ���ӿ���
		beforeDining(i, (i + pCnt - 1) % pCnt);
		dining(i);
		ReleaseSemaphore(s[(i + pCnt - 1) % pCnt], 1, NULL);	//�����Ҳ����
		afterDining(i, (i + pCnt - 1) % pCnt);
		ReleaseSemaphore(s[i], 1, NULL);	//����������
		afterDining(i, i);

		EnterCriticalSection(&cs);
		--diningCnt;	//�ÿ�������-1
		LeaveCriticalSection(&cs);
	}
	return 0;
}


int main() {
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < pCnt; ++i) {
		s[i] = CreateSemaphore(NULL, 1, 1, NULL);
	}
	InitializeCriticalSection(&cs);
	HANDLE hThread[pCnt];
	DWORD phs[pCnt];
	for (int i = 0; i < pCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, philosopher, LPVOID(i), 0, &phs[i]);
		if (!hThread[i]) return -1;
	}
	while (goon) {
		if (getchar()) goon = false;
	}
	return 0;
}

/**/