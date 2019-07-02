#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/times.h>
#include <limits.h>
#define CLK_TCK sysconf(_SC_CLK_TCK)
#define PI 3.141592

int g_count = 0;  // ������ �����ڿ�
pthread_mutex_t mutex_lock;// ���ؽ� ��ü ����_��ȣ����

						   //������ sin �׷��� �׸��� �Լ�
void *t_function(void *data)
{
	//char* thread_name = (char*)data;

	//���ؽ� ��� ����
	pthread_mutex_lock(&mutex_lock);

	//sine���� ���� ����
	double temp;

	//���� �޸� ���� 360000 �̳��� ����
	while (g_count <= 3600000) {
		//�ӽ� ������ sin ���� �� ����
		temp = sin((g_count*(PI / 180)));
		//sin ���� �� ��� 
		printf("sin(%3d)", g_count);
		//sin�� ��ŭ �� ����(�׷��� �׸��� ����, ���� ����ŭ �� ����)
		for (int j = -10; j < temp * 10; j++) {
			printf(" ");
		}

		//�׷��� ��� �� �ٲ�
		printf("*\n");
		//���� �� 10 ���� 
		g_count += 10;
	}
	//���ؽ� ��� ����
	pthread_mutex_unlock(&mutex_lock);
}

int main()
{
	time_t t; //User_time, System_time �ð� ���� ����
	struct tms mytms; //user, system time ����� ���� tms������ ����ü ����
	clock_t t1, t2; //Real time �ð� ���� ����

					//����ð� �� ���� ����
	if ((t1 = times(&mytms)) == -1) {
		perror("times 1");
		exit(1);
	}
	//������ 1,2 ����
	pthread_t p_thread1, p_thread2;
	int status;

	// ���ؽ� ��ü �ʱ�ȭ, �⺻ Ư������ �ʱ�ȭ ����
	pthread_mutex_init(&mutex_lock, NULL);

	//������1,2 ����
	pthread_create(&p_thread1, NULL, t_function, (void *)"Thread1");
	pthread_create(&p_thread2, NULL, t_function, (void *)"Thread2");

	//������1_���۵� �ٸ� ������ ���� ���
	pthread_join(p_thread1, (void *)&status);

	//������2_���۵� �ٸ� ������ ���� ���
	pthread_join(p_thread2, (void *)&status);

	if ((t2 = times(&mytms)) == -1) {
		perror("times 2");
		exit(1);
	}
	//sleep(3);

	/*Real time , User time, System time ��� */
	printf("Real time : %.2f sec\n", (double)(t2 - t1) / CLK_TCK);
	printf("User time : %.2f sec\n", (double)mytms.tms_utime / CLK_TCK);
	printf("System time : %.2f sec\n", (double)mytms.tms_stime / CLK_TCK);

	return 0;
}