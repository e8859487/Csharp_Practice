#include <stdio.h>
#include <iostream>
void part3(){
	//part3
	float a, b;//��T�פp��6��  , double �i�H�O10��
	b = 1.0e12;
	b = b - 1.0;
	a = b + 1.0;
	printf("%f\n", a); //round-off error :�W�j��-���p�� �|���ͻ~�t�C  

	//��scanfŪ���B�I��
	double xx;
	scanf("%lf", &xx); //%lf�f�tdouble�~�|�����T�����G    %f�f�tfloat

	float x = 123456.0;
	double y = 2.34e12;
	printf("%.2f or %e\n", x, x);//�w�]�p���I��Ƥ���A�i�ǥ�.X �ӳ]�w�n��ܴX��A�٥i�H�Φ��\��Ӱ��|�ˤ��J�B��ܦ�ơB���+-���B�]�w��ܦ�Ƥ�����0��...
	printf("%.2f or %e\n", y, y);
	printf("%12.2f \n", y);//�`����12,�p���I2�� �ѤU���|�ˤ��J
	int i, j;
	scanf("%d  %d", &i, &j);

	printf("%*.*f", i, j, y);//���B��*(�P��)�|�Q�᭱��i j���N

}
void part2(){
	//part2
	char ch;
	char chArray[10];
	printf("Please enter an char");
	scanf("%c", &ch);
	printf("Please enter an string");
	scanf("%c", chArray);//���B���ݭn��&  �]���}�C�W�����N���V���m
	printf("ASCII code for '%c' is %d \n", ch, ch);//�r����ASCII�X������@�ӤG�i��ƭȡA���ƭȥi�H�ǥ�%d�o��
	printf("%c%c", 0xa7, 0x70);//2bit �զX���@�Ӥ���r
}

void part1(){ 
	//part1
	int x;
	int y;
	unsigned unsignJ = 4294967295;
	printf("Enter an integer:");
	scanf("%d", &x);//�i�Dscanf�n�p���Ū���쪺���  &���o�ܼƪ���}
	printf("Enter an integer:");
	scanf("%d", &y);
	//�]�i�H�g���o��::
	//scanf(%d%d,&x,&y); ��ϥΪ̿�J��  �H�ťիءBTab�Benter���j
	printf("%d\n", x + y);//%d�N�������
	printf("%u\n", unsignJ);//%u�N��unsign int ,�Y�ϥο��~�|�y����X���ŦX�w��
}
int main(){

	char ss[10] = "123456789"; //�}�C���׬�10��char �u��s9�쪺��� �̫�@��O�����Ÿ� '\0'
	ss[2] = '0';
	printf("%s", ss);
	
	system("PAUSE");
	return 0;
}

