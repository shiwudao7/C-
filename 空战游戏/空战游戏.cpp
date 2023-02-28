#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define High 18  // 定义游戏画面尺寸 
#define Width 40

#define EnemyNum 5 //敌机数目 
//函数外全局变量定义
int canvas[High][Width] = {0}; //二维数组记录游戏画面中对应的元素
						// 0输出空格，1输出飞机，2输出子弹 ,3是敌机 
int position_x,position_y;//飞机坐标位置
int enemy_x[EnemyNum],enemy_y[EnemyNum];//敌机坐标 
int score;//得分
int BulletWidth;//子弹宽度  

void gotoxy(int x,int y)  //光标移动到(x,y)位置
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle,pos);
}

void HideCursor() // 用于隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // 第二个值为0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void startup()//数据初始化
{
	position_x = High / 2; 
	position_y = Width / 2;
	
	canvas[position_x][position_y] = 1;
	
	int k;
	for(k = 0 ; k < EnemyNum ; k++)
	{
		enemy_x[k] = rand() % 3;
		enemy_y[k] = rand() % Width;
		canvas[enemy_x[k]][enemy_y[k]] = 3;
	}
	BulletWidth = 5;
	
	score = 0; 
	
	HideCursor(); // 隐藏光标 
}

void show()//显示画面
{
	
	int i , j;
	
	gotoxy(0,0);  // 光标移动到原点位置，以下重画清屏 
	
	for(i = 0 ; i < High ; i++)
	{
		for(j = 0 ; j < Width ; j++)
		{
			if(canvas[i][j] == 1)
				printf("*");//输出飞机 *
			else if(canvas[i][j] == 2)
				printf("|");//输出子弹 |
			else if(canvas[i][j] == 3)
				printf("@");//输出敌机 @  
			else
				printf(" ");//输出空格  
		}
		printf("\n");
	}
		printf("得分：%d\n",score);
}

void updateWithoutlnput()//与用户输入无关的更新
{
	int i,j,k;
	
	for(i = 0 ; i < High ; i++)
	{
		for(j = 0 ; j < Width ; j++)
		{
			if(canvas[i][j] == 2)
			{
				for(k = 0 ; k < EnemyNum ; k++)
				{
					if(i == enemy_x[k] && j == enemy_y[k])//击中敌机 
					{
						canvas[enemy_x[k]][enemy_y[k]] = 0;
						enemy_x[k] = 0;
						enemy_y[k] = rand() % Width; 
						canvas[enemy_x[k]][enemy_y[k]] = 3;
						
						score++;
					}	
				}
				
				
				//使子弹自动向上移动 
				canvas[i][j] = 0;
				if(i > 0)
				canvas[i-1][j] = 2;	
			}
		}
	}
	
	for(k = 0 ; k < EnemyNum ; k++)
	{
		if(enemy_x[k] > High)//敌机跑出下边界，重新生成敌机 
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k] = 0;
			enemy_y[k] = rand() % Width; 
			canvas[enemy_x[k]][enemy_y[k]] = 3;
			
			score--;		
		}		
	}

	
	static int speed = 20;
	if(speed < 20)
		speed++;
	if(speed == 20)
	{
		//敌机自动下落 

		for(k = 0 ; k < EnemyNum ; k++)
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k]++;
			canvas[enemy_x[k]][enemy_y[k]] = 3;
		}

		
		speed = 0;	
	}

}

void updateWithlnput()//与用户有关的更新 
{
	char input;
	if(kbhit())//当按键时
	{
		input = getch();
		if(input == 'a')
		{
			canvas[position_x][position_y] = 0;
			position_y--;
			canvas[position_x][position_y] = 1;
		}
		if(input == 'd')
		{
			canvas[position_x][position_y] = 0;
			position_y++;
			canvas[position_x][position_y] = 1;
		}
		if(input == 'w')
		{
			canvas[position_x][position_y] = 0;
			position_x--;
			canvas[position_x][position_y] = 1;
		}
		if(input == 's')
		{
			canvas[position_x][position_y] = 0;
			position_x++;
			canvas[position_x][position_y] = 1;
		}
		if(input == ' ')
		{
			int left,right;
			left = position_y - BulletWidth;
			if(left < 0)
				left = 0;
			right = position_y + BulletWidth;
			if(right > Width - 1)
				right = Width - 1;
			
			int x;
			for(x = left ; x <= right ; x++)
			canvas[position_x - 1][x] = 2;
		}
	}

} 

int main()
{
	startup();//数据初始化
	while(1)//游戏循环执行
	{
		show();//显示画面
		updateWithoutlnput();//与用户输入无关的更新
		updateWithlnput();//与用户有关的更新 
	} 
	return 0; 
}
