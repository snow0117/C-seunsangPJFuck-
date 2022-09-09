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

//□ ■

struct Object {
	int x;
	int y;
	int allowed[4]; //갈 수 있는 분면  
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





struct Object spawner(int n); //인자 n으로 분면선택, 선택한 분면의 임의의 위치에 좌표 지정
int quadrantChecker(int x, int y); //좌표 주면 어디 사분면인지 반환 
void displayMap(); //맵을 화면에 출력 
void walk(struct Object* mob, int key); //이동을 할 객체의 주소와  방향을 인자로 하여 상하좌우 이동
void randomWalk(struct Object* mob); //이동 할 객체의 주소를 받아 랜덤 이동
void drunk(int n); //술에 취해있는 상태
void waking(int n); //경찰을 만나서 술이 깬 상태 


int main()
{
	int i; 
	int quadrant;

	srand(time(NULL));

	printf("첫분면 이동 횟수 : ");
	scanf("%d", &m_t);

	printf("수동 이동 횟수 : ");
	scanf("%d", &r_t);

	printf("자동 이동 모드의 이동 속도 : ");
	scanf("%d", &speed);
	system("cls");
	
	quadrant = rand()%4;

	pub = spawner(quadrant);
	home = spawner(3-quadrant);

	drinker = pub;

	for (i = 0; i < 4; i++)  
	{
		cops[i].x = -5; //경찰들을 좌표(사분면) 밖으로 뺌  
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


//인자 n으로 분면선택, 선택한 분면의 임의의 위치에 좌표 지정
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
	} while (map[object.y][object.x] == 1); //선택한 좌표에 벽이 있으면 재지정  


	return object;
}

//좌표 주면 어디 사분면인지 반환
int quadrantChecker(int x, int y)
{
	if (0<=x && x<5 && 0<=y && y<5) return 0;
	if (5<=x && x<10 && 0<=y && y<5) return 1;
	if (0<=x && x<5 && 5<=y && y<10) return 2;
	if (5<=x && x<10 && 5<=y && y<10) return 3;
	
	return -1; //좌표 밖(아직 생성되지 않은 경찰) 
}

//맵을 화면에 출력 
void displayMap()
{
	int i, j;
	int drinkerQuadrant = quadrantChecker(drinker.x, drinker.y); //취객의 위치한 현재 분면 
	
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			
			if (i == drinker.y && j == drinker.x) 
			{
				if (drinker.x == pub.x && drinker.y == pub.y) printf("DP");
				else if (drinker.x == home.x && drinker.y == home.y) printf("DH");
				else if (drinker.x == cops[drinkerQuadrant].x && drinker.y == cops[drinkerQuadrant].y) printf("OD");
				else printf("ⓓ");
			}
			else if (i == cops[0].y && j == cops[0].x) printf("ⓞ");
			else if (i == cops[1].y && j == cops[1].x) printf("ⓞ");
			else if (i == cops[2].y && j == cops[2].x) printf("ⓞ");
			else if (i == cops[3].y && j == cops[3].x) printf("ⓞ");
			else if (i == pub.y && j == pub.x) printf("ⓟ");
			else if (i == home.y && j == home.x) printf("ⓗ");
			else if (map[i][j] == 0) printf("□");
			else printf("■");
		}
		printf("\n");

	}
	
	//취객이 집에 도착 
	if (drinker.x == home.x && drinker.y == home.y)   
	{
		printf("성공!\n");
		_getch();
		exit(0);
	}
	
	//취객이 경찰한테 잡힘 (같은 좌표에 있음) 
	if (	drinker.x == cops[drinkerQuadrant].x && drinker.y == cops[drinkerQuadrant].y	) 
	{
		printf("실패! (경찰한테 잡힘)\n");
		_getch();
		exit(0);
	}
	
}

//이동을 할 객체의 주소와  방향을 인자로 하여 상하좌우 이동
void walk(struct Object* mob, int key)
{
	if (key == UP) mob->y -= 1;
	else if (key == DOWN) mob->y += 1;
	else if (key == LEFT) mob->x -= 1;
	else if (key == RIGHT) mob->x += 1;
}

//이동 할 객체의 주소를 받아 랜덤 이동
void randomWalk(struct Object* mob)
{
	
	int mX = mob->x;
	int mY = mob->y;
	int arr[4] = { 0, };
	int num = 0;

	
	/*
	이동 할 수 있는 방향이 몇 개인지 확인 
	좌표 밖이 아니고, 길이 열려 있고, 갈 수 있는 분면  인지 체크  
	조건이 모두 맞으면 배열에 방향 저장  
	*/

	//상 체크   
	if (mY - 1 >= 0 && map[mY - 1][mX] == 0 && mob->allowed[quadrantChecker(mX, mY - 1)] == 1) 
	{
		arr[num] = UP;
		++num;
	}
	//하 체크 
	if (mY + 1 < 10 && map[mY + 1][mX] == 0 && mob->allowed[quadrantChecker(mX, mY + 1)] == 1) 
	{
		arr[num] = DOWN;
		++num;
	}
	//좌 체크 
	if (mX - 1 >= 0 && map[mY][mX - 1] == 0 && mob->allowed[quadrantChecker(mX - 1, mY)] == 1)  
	{
		arr[num] = LEFT;
		++num;
	}
	//우 체크 
	if (mX + 1 < 10 && map[mY][mX + 1] == 0 && mob->allowed[quadrantChecker(mX + 1, mY)] == 1)  
	{
		arr[num] = RIGHT;
		++num;
	}
 
	if (num != 0)
		walk(mob, arr[rand() % num]);
}

//술에 취해있는 상태  
void drunk(int n)
{
	int i;
	int start, end;  
	int preQuadrant = quadrantChecker(drinker.x, drinker.y); //이동하기 전 분면 저장 
	int curQuadrant;
	int sum = 0;

  	//해당 분면에 경찰이 있을 때 들켰는지 확인 
	if (	quadrantChecker(cops[preQuadrant].x, cops[preQuadrant].y) != -1	) 
	{
		
		if (drinker.x == cops[preQuadrant].x) //x좌표가 같을때 y좌표 확인 
		{
			
			start = drinker.y < cops[preQuadrant].y ? drinker.y : cops[preQuadrant].y; //start는 경찰의 취객의 y좌표 중 더 작은 값 
			end = drinker.y > cops[preQuadrant].y ? drinker.y : cops[preQuadrant].y; //end는 경찰과 취객의 y좌표 중 더 큰 값 

			for (i = start; i < end; i++) // 경찰과 취객사이의 벽(1) 체크 
				sum += map[i][drinker.x]; //길 밖에 없으면 sum의 값은 0

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
	
	
	//출력 
	displayMap();

	if (preQuadrant == quadrantChecker(pub.x, pub.y) )
		printf("남은 횟수 (pub분면) : %d\n", n);
	else
		printf("남은 횟수 %d\n", n);

	for (i = 0; i < 4; i++)
		printf("%d ", drinker.allowed[i]);

	Sleep(speed);
	system("cls");

	/*	-------- */
	

	randomWalk(&drinker);
	
	
	if (preQuadrant == quadrantChecker(pub.x, pub.y)) --n; //술집이 위치한 분면이면 남은 이동 횟수 감소 
	curQuadrant = quadrantChecker(drinker.x, drinker.y); //이동 한 후의 분면 저장 
	


	for (i = 0; i < 4; i++)
	{
		if (quadrantChecker(cops[i].x, cops[i].y) != -1) //사분면 내에 존재하는 경찰만 randomwalk 
			randomWalk(&cops[i]);
	}


	if (preQuadrant != curQuadrant) //분면을 이동 할 경우 
	{
		drinker.allowed[preQuadrant] = 0; //지나온 분면은 못돌아가게 조정 

		// 술 집 있는 분면에서 탈출 했을 경우 
		if (preQuadrant == quadrantChecker(pub.x, pub.y) )
		{
			if (n <= 0 && quadrantChecker(cops[curQuadrant].x, cops[curQuadrant].y) == -1)
			{
				//경찰 등장 
				cops[curQuadrant] = spawner(curQuadrant);
				cops[curQuadrant].allowed[curQuadrant] = 1;
			}
			return drunk(m_t); 
		}

		if (curQuadrant == quadrantChecker(home.x, home.y)) //도착한 분면이 집이 위치한 분면일 경우 이웃한 분면 못가게 조정 
			drinker.allowed[3 - preQuadrant] = 0;
			
	}

	return drunk(n);
}

//경찰을 만나서 술이 깬 상태 
void waking(int n)
{
	int i;
	int key = 0;
	int preQuadrant = quadrantChecker(drinker.x, drinker.y); //이전 분면 저장 
	int curQuadrant;
	int dX = drinker.x; //취객의 x좌표 
	int dY = drinker.y;	//취객의 y좌표 

	system("cls");
	displayMap();

	//r_t 를 모두 소진했을시 패배 
	if (n == 0)
	{
		printf("실패(초과)");
		_getch();
		exit(0);
	}

	printf("남은 수동 조정 횟수 %d\n ", n);
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

	curQuadrant = quadrantChecker(drinker.x, drinker.y); //이동 한 후의 분면 체크 

	if (preQuadrant != curQuadrant) //분면 이동 
	{
		//drinker.allowed[preQuadrant] = 0;
		drinker.allowed[curQuadrant] = 1;
		
		if (curQuadrant == quadrantChecker(home.x, home.y)) //도착한 분면이 집이 위치한 분면일 경우 이웃한 분면 못가게 조정 
		{
			drinker.allowed[3 - preQuadrant] = 0;
			drinker.allowed[preQuadrant] = 0;
		}
		
		system("cls");
		return;
	}

	return waking(n-1);
}

