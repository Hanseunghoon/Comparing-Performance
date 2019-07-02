#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/times.h>
#include <limits.h>
#include <sys/wait.h> 
#include <sys/mman.h> 

#define CLK_TCK sysconf(_SC_CLK_TCK)
#define PI 3.141592
int(*g_count) = 0;  //공유메모리 자원

					//멀티 프로세스 sin 그래프 그리기 함수
void function()
{
	double temp; //sine 값을 받을 변수

				 //공유 메모리 범위 360000 이내로 지정
	while (*g_count <= 3600000) {
		//임시 변수에 sin 라디안 값 삽입
		temp = sin((*g_count)*(PI / 180));
		//sin 라디안 값 출력 
		printf("sin(%3d)", *g_count);
		//sin값 만큼 줄 띄우기(그래프 그리기 위함, 라디안 값만큼 줄 띄우기)
		for (int j = -10; j < temp * 10; j++) {
			printf(" ");
		}
		//그래프 찍고 줄 바꿈
		printf("*\n");
		//라디안 값 10 증가 
		*g_count += 10;
	}
}

int main(void) {
	time_t t; //User_time, System_time 시간 측정 변수
	struct tms mytms; //user, system time 계산을 위한 tms형식의 구조체 선언
	clock_t t1, t2; //Real time 시간 측정 변수

					//수행시간 및 수행 오류
	if ((t1 = times(&mytms)) == -1) {
		perror("times 1");
		exit(1); //종료 
	}
	//메모리 맵 방식으로 파일을 연결하여 공유메모리 사용하기 위해 선언
	g_count = mmap(NULL, sizeof *g_count, PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	pid_t pid;
	//fork() 함수를 이용한 프로세스 선택 
	switch (pid = fork()) {
	case -1: /* fork failed */
		perror("fork");
		exit(1);
		break;
	case 0: /* child process */
		function();
		exit(0);
		break;
	default: /* parent process */
		wait(NULL);
		function();
		//할당 된 메모리 영역 해제 
		munmap(g_count, sizeof *g_count);
		break;
	}
	if ((t2 = times(&mytms)) == -1) {
		perror("times 2");
		exit(1);
	}
	//sleep(3);
	/*Real time , User time, System time 출력 */
	printf("Real time : %.2f sec\n", (double)(t2 - t1) / CLK_TCK);
	printf("User time : %.2f sec\n", (double)mytms.tms_utime / CLK_TCK);
	printf("System time : %.2f sec\n", (double)mytms.tms_stime / CLK_TCK);

	return 0;
}