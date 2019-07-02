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
int(*g_count) = 0;  //�����޸� �ڿ�

					//��Ƽ ���μ��� sin �׷��� �׸��� �Լ�
void function()
{
	double temp; //sine ���� ���� ����

				 //���� �޸� ���� 360000 �̳��� ����
	while (*g_count <= 3600000) {
		//�ӽ� ������ sin ���� �� ����
		temp = sin((*g_count)*(PI / 180));
		//sin ���� �� ��� 
		printf("sin(%3d)", *g_count);
		//sin�� ��ŭ �� ����(�׷��� �׸��� ����, ���� ����ŭ �� ����)
		for (int j = -10; j < temp * 10; j++) {
			printf(" ");
		}
		//�׷��� ��� �� �ٲ�
		printf("*\n");
		//���� �� 10 ���� 
		*g_count += 10;
	}
}

int main(void) {
	time_t t; //User_time, System_time �ð� ���� ����
	struct tms mytms; //user, system time ����� ���� tms������ ����ü ����
	clock_t t1, t2; //Real time �ð� ���� ����

					//����ð� �� ���� ����
	if ((t1 = times(&mytms)) == -1) {
		perror("times 1");
		exit(1); //���� 
	}
	//�޸� �� ������� ������ �����Ͽ� �����޸� ����ϱ� ���� ����
	g_count = mmap(NULL, sizeof *g_count, PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	pid_t pid;
	//fork() �Լ��� �̿��� ���μ��� ���� 
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
		//�Ҵ� �� �޸� ���� ���� 
		munmap(g_count, sizeof *g_count);
		break;
	}
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