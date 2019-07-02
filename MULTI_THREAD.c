#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/times.h>
#include <limits.h>
#define CLK_TCK sysconf(_SC_CLK_TCK)
#define PI 3.141592

int g_count = 0;  // 쓰레드 공유자원
pthread_mutex_t mutex_lock;// 뮤텍스 객체 선언_상호배제

						   //쓰레드 sin 그래프 그리기 함수
void *t_function(void *data)
{
	//char* thread_name = (char*)data;

	//뮤텍스 잠금 설정
	pthread_mutex_lock(&mutex_lock);

	//sine값을 받을 변수
	double temp;

	//공유 메모리 범위 360000 이내로 지정
	while (g_count <= 3600000) {
		//임시 변수에 sin 라디안 값 삽입
		temp = sin((g_count*(PI / 180)));
		//sin 라디안 값 출력 
		printf("sin(%3d)", g_count);
		//sin값 만큼 줄 띄우기(그래프 그리기 위함, 라디안 값만큼 줄 띄우기)
		for (int j = -10; j < temp * 10; j++) {
			printf(" ");
		}

		//그래프 찍고 줄 바꿈
		printf("*\n");
		//라디안 값 10 증가 
		g_count += 10;
	}
	//뮤텍스 잠금 해제
	pthread_mutex_unlock(&mutex_lock);
}

int main()
{
	time_t t; //User_time, System_time 시간 측정 변수
	struct tms mytms; //user, system time 계산을 위한 tms형식의 구조체 선언
	clock_t t1, t2; //Real time 시간 측정 변수

					//수행시간 및 수행 오류
	if ((t1 = times(&mytms)) == -1) {
		perror("times 1");
		exit(1);
	}
	//쓰레드 1,2 선언
	pthread_t p_thread1, p_thread2;
	int status;

	// 뮤텍스 객체 초기화, 기본 특성으로 초기화 했음
	pthread_mutex_init(&mutex_lock, NULL);

	//쓰레드1,2 생성
	pthread_create(&p_thread1, NULL, t_function, (void *)"Thread1");
	pthread_create(&p_thread2, NULL, t_function, (void *)"Thread2");

	//쓰레드1_시작된 다른 쓰레드 종료 대기
	pthread_join(p_thread1, (void *)&status);

	//쓰레드2_시작된 다른 쓰레드 종료 대기
	pthread_join(p_thread2, (void *)&status);

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