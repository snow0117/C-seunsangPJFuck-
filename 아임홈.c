#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

//�� ��

struct Object {
	int x;
	int y;
	int allowed[4]; //�� �� �ִ� �и�  
};

struct Object drinker;
struct Object cops[4];
struct Object home;
struct Object pub;

int m_t = 0, r_t = 0, speed = 0;


int map[10][10] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 1, 0, 1, 1,
				   0, 1, 1, 1, 1, 0, 1, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
				   0, 1, 1, 0, 1, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				   0, 1, 1, 1, 1, 0, 1, 0, 1, 0,
				   0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
				   0, 1, 1, 0, 1, 0, 1, 0, 1, 0,
				   0, 0, 0, 0, 0, 0, 0, 0, 0, 0};





struct Object spawner(int n); //���� n���� �и鼱��, ������ �и��� ������ ��ġ�� ��ǥ ����
int quadrantChecker(int x, int y); //��ǥ �ָ� ��� ��и����� ��ȯ 
void displayMap(); //���� ȭ�鿡 ��� 
void walk(struct Object* mob, int key); //�̵��� �� ��ü�� �ּҿ�  ������ ���ڷ� �Ͽ� �����¿� �̵�
void randomWalk(struct Object* mob); //�̵� �� ��ü�� �ּҸ� �޾� ���� �̵�
void drunk(int n); //���� �����ִ� ����
void waking(int n); //������ ������ ���� �� ���� 


int main()
{
	int i; 
	int quadrant;

	srand(time(NULL));

	printf("ù�и� �̵� Ƚ�� : ");
	scanf("%d", &m_t);

	printf("���� �̵� Ƚ�� : ");
	scanf("%d", &r_t);

	printf("�ڵ� �̵� ����� �̵� �ӵ� : ");
	scanf("%d", &speed);
	system("cls");
	
	quadrant = rand()%4;

	pub = spawner(quadrant);
	home = spawner(3-quadrant);

	drinker = pub;

	for (i = 0; i < 4; i++)  
	{
		cops[i].x = -5; //�������� ��ǥ(��и�) ������ ��  
		cops[i].y = -5;

		drinker.allowed[i] = 1; 
	}
	
	while (1)
	{
		drunk(m_t);
		waking(r_t);
	}

	return 0;
}

/* ---------------------- */


//���� n���� �и鼱��, ������ �и��� ������ ��ġ�� ��ǥ ����
struct Object spawner(int n)
{
	struct Object object = { 0, };
	
	//srand(time(NULL));

	do
	{
		if (n == 0)
		{
			object.x = rand() % 5;
			object.y = rand() % 5;
		}
		else if (n == 1)
		{
			object.x = rand() % 5 + 5;
			object.y = rand() % 5;
		}
		else if (n == 2)
		{
			object.x = rand() % 5;
			object.y = rand() % 5 + 5;
		}
		else
		{
			object.x = rand() % 5 + 5;
			object.y = rand() % 5 + 5;
		}
	} while (map[object.y][object.x] == 1); //������ ��ǥ�� ���� ������ ������  


	return object;
}

//��ǥ �ָ� ��� ��и����� ��ȯ
int quadrantChecker(int x, int y)
{
	if (0<=x && x<5 && 0<=y && y<5) return 0;
	if (5<=x && x<10 && 0<=y && y<5) return 1;
	if (0<=x && x<5 && 5<=y && y<10) return 2;
	if (5<=x && x<10 && 5<=y && y<10) return 3;
	
	return -1; //��ǥ ��(���� �������� ���� ����) 
}

//���� ȭ�鿡 ��� 
void displayMap()
{
	int i, j;
	int drinkerQuadrant = quadrantChecker(drinker.x, drinker.y); //�밴�� ��ġ�� ���� �и� 
	
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			
			if (i == drinker.y && j == drinker.x) 
			{
				if (drinker.x == pub.x && drinker.y == pub.y) printf("DP");
				else if (drinker.x == home.x && drinker.y == home.y) printf("DH");
				else if (drinker.x == cops[drinkerQuadrant].x && drinker.y == cops[drinkerQuadrant].y) printf("OD");
				else printf("��");
			}
			else if (i == cops[0].y && j == cops[0].x) printf("��");
			else if (i == cops[1].y && j == cops[1].x) printf("��");
			else if (i == cops[2].y && j == cops[2].x) printf("��");
			else if (i == cops[3].y && j == cops[3].x) printf("��");
			else if (i == pub.y && j == pub.x) printf("��");
			else if (i == home.y && j == home.x) printf("��");
			else if (map[i][j] == 0) printf("��");
			else printf("��");
		}
		printf("\n");

	}
	
	//�밴�� ���� ���� 
	if (drinker.x == home.x && drinker.y == home.y)   
	{
		printf("����!\n");
		_getch();
		exit(0);
	}
	
	//�밴�� �������� ���� (���� ��ǥ�� ����) 
	if (	drinker.x == cops[drinkerQuadrant].x && drinker.y == cops[drinkerQuadrant].y	) 
	{
		printf("����! (�������� ����)\n");
		_getch();
		exit(0);
	}
	
}

//�̵��� �� ��ü�� �ּҿ�  ������ ���ڷ� �Ͽ� �����¿� �̵�
void walk(struct Object* mob, int key)
{
	if (key == UP) mob->y -= 1;
	else if (key == DOWN) mob->y += 1;
	else if (key == LEFT) mob->x -= 1;
	else if (key == RIGHT) mob->x += 1;
}

//�̵� �� ��ü�� �ּҸ� �޾� ���� �̵�
void randomWalk(struct Object* mob)
{
	
	int mX = mob->x;
	int mY = mob->y;
	int arr[4] = { 0, };
	int num = 0;

	
	/*
	�̵� �� �� �ִ� ������ �� ������ Ȯ�� 
	��ǥ ���� �ƴϰ�, ���� ���� �ְ�, �� �� �ִ� �и�  ���� üũ  
	������ ��� ������ �迭�� ���� ����  
	*/

	//�� üũ   
	if (mY - 1 >= 0 && map[mY - 1][mX] == 0 && mob->allowed[quadrantChecker(mX, mY - 1)] == 1) 
	{
		arr[num] = UP;
		++num;
	}
	//�� üũ 
	if (mY + 1 < 10 && map[mY + 1][mX] == 0 && mob->allowed[quadrantChecker(mX, mY + 1)] == 1) 
	{
		arr[num] = DOWN;
		++num;
	}
	//�� üũ 
	if (mX - 1 >= 0 && map[mY][mX - 1] == 0 && mob->allowed[quadrantChecker(mX - 1, mY)] == 1)  
	{
		arr[num] = LEFT;
		++num;
	}
	//�� üũ 
	if (mX + 1 < 10 && map[mY][mX + 1] == 0 && mob->allowed[quadrantChecker(mX + 1, mY)] == 1)  
	{
		arr[num] = RIGHT;
		++num;
	}
 
	if (num != 0)
		walk(mob, arr[rand() % num]);
}

//���� �����ִ� ����  
void drunk(int n)
{
	int i;
	int start, end;  
	int preQuadrant = quadrantChecker(drinker.x, drinker.y); //�̵��ϱ� �� �и� ���� 
	int curQuadrant;
	int sum = 0;

  	//�ش� �и鿡 ������ ���� �� ���״��� Ȯ�� 
	if (	quadrantChecker(cops[preQuadrant].x, cops[preQuadrant].y) != -1	) 
	{
		
		if (drinker.x == cops[preQuadrant].x) //x��ǥ�� ������ y��ǥ Ȯ�� 
		{
			
			start = drinker.y < cops[preQuadrant].y ? drinker.y : cops[preQuadrant].y; //start�� ������ �밴�� y��ǥ �� �� ���� �� 
			end = drinker.y > cops[preQuadrant].y ? drinker.y : cops[preQuadrant].y; //end�� ������ �밴�� y��ǥ �� �� ū �� 

			for (i = start; i < end; i++) // ������ �밴������ ��(1) üũ 
				sum += map[i][drinker.x]; //�� �ۿ� ������ sum�� ���� 0

			if (sum == 0) return;  // 
		}
		else if (drinker.y == cops[preQuadrant].y)
		{
			start = drinker.x < cops[preQuadrant].x ? drinker.x : cops[preQuadrant].x;
			end = drinker.x > cops[preQuadrant].x ? drinker.x : cops[preQuadrant].x;

			for (i = start; i < end; i++)
				sum += map[drinker.y][i];

			if (sum == 0) return;
		}
	}
	
	
	//��� 
	displayMap();

	if (preQuadrant == quadrantChecker(pub.x, pub.y) )
		printf("���� Ƚ�� (pub�и�) : %d\n", n);
	else
		printf("���� Ƚ�� %d\n", n);

	for (i = 0; i < 4; i++)
		printf("%d ", drinker.allowed[i]);

	Sleep(speed);
	system("cls");

	/*	-------- */
	

	randomWalk(&drinker);
	
	
	if (preQuadrant == quadrantChecker(pub.x, pub.y)) --n; //������ ��ġ�� �и��̸� ���� �̵� Ƚ�� ���� 
	curQuadrant = quadrantChecker(drinker.x, drinker.y); //�̵� �� ���� �и� ���� 
	


	for (i = 0; i < 4; i++)
	{
		if (quadrantChecker(cops[i].x, cops[i].y) != -1) //��и� ���� �����ϴ� ������ randomwalk 
			randomWalk(&cops[i]);
	}


	if (preQuadrant != curQuadrant) //�и��� �̵� �� ��� 
	{
		drinker.allowed[preQuadrant] = 0; //������ �и��� �����ư��� ���� 

		// �� �� �ִ� �и鿡�� Ż�� ���� ��� 
		if (preQuadrant == quadrantChecker(pub.x, pub.y) )
		{
			if (n <= 0 && quadrantChecker(cops[curQuadrant].x, cops[curQuadrant].y) == -1)
			{
				//���� ���� 
				cops[curQuadrant] = spawner(curQuadrant);
				cops[curQuadrant].allowed[curQuadrant] = 1;
			}
			return drunk(m_t); 
		}

		if (curQuadrant == quadrantChecker(home.x, home.y)) //������ �и��� ���� ��ġ�� �и��� ��� �̿��� �и� ������ ���� 
			drinker.allowed[3 - preQuadrant] = 0;
			
	}

	return drunk(n);
}

//������ ������ ���� �� ���� 
void waking(int n)
{
	int i;
	int key = 0;
	int preQuadrant = quadrantChecker(drinker.x, drinker.y); //���� �и� ���� 
	int curQuadrant;
	int dX = drinker.x; //�밴�� x��ǥ 
	int dY = drinker.y;	//�밴�� y��ǥ 

	system("cls");
	displayMap();

	//r_t �� ��� ���������� �й� 
	if (n == 0)
	{
		printf("����(�ʰ�)");
		_getch();
		exit(0);
	}

	printf("���� ���� ���� Ƚ�� %d\n ", n);
	for (i = 0; i < 4; i++)
		printf("%d ", drinker.allowed[i]);


	key = _getch();
	if (key != 224) return waking(n);
	else key = _getch();

	if (key == UP && dY - 1 >= 0 && map[dY - 1][dX] == 0) walk(&drinker, UP);
	else if (key == DOWN && dY + 1 < 10 && map[dY + 1][dX] == 0) walk(&drinker, DOWN);
	else if (key == LEFT && dX - 1 >= 0 && map[dY][dX - 1] == 0) walk(&drinker, LEFT);
	else if (key == RIGHT && dX + 1 < 10 && map[dY][dX + 1] == 0) walk(&drinker, RIGHT);
	else return waking(n); 

	curQuadrant = quadrantChecker(drinker.x, drinker.y); //�̵� �� ���� �и� üũ 

	if (preQuadrant != curQuadrant) //�и� �̵� 
	{
		//drinker.allowed[preQuadrant] = 0;
		drinker.allowed[curQuadrant] = 1;
		
		if (curQuadrant == quadrantChecker(home.x, home.y)) //������ �и��� ���� ��ġ�� �и��� ��� �̿��� �и� ������ ���� 
		{
			drinker.allowed[3 - preQuadrant] = 0;
			drinker.allowed[preQuadrant] = 0;
		}
		
		system("cls");
		return;
	}

	return waking(n-1);
}

