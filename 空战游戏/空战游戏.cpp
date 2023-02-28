#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define High 18  // ������Ϸ����ߴ� 
#define Width 40

#define EnemyNum 5 //�л���Ŀ 
//������ȫ�ֱ�������
int canvas[High][Width] = {0}; //��ά�����¼��Ϸ�����ж�Ӧ��Ԫ��
						// 0����ո�1����ɻ���2����ӵ� ,3�ǵл� 
int position_x,position_y;//�ɻ�����λ��
int enemy_x[EnemyNum],enemy_y[EnemyNum];//�л����� 
int score;//�÷�
int BulletWidth;//�ӵ����  

void gotoxy(int x,int y)  //����ƶ���(x,y)λ��
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle,pos);
}

void HideCursor() // �������ع��
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // �ڶ���ֵΪ0��ʾ���ع��
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void startup()//���ݳ�ʼ��
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
	
	HideCursor(); // ���ع�� 
}

void show()//��ʾ����
{
	
	int i , j;
	
	gotoxy(0,0);  // ����ƶ���ԭ��λ�ã������ػ����� 
	
	for(i = 0 ; i < High ; i++)
	{
		for(j = 0 ; j < Width ; j++)
		{
			if(canvas[i][j] == 1)
				printf("*");//����ɻ� *
			else if(canvas[i][j] == 2)
				printf("|");//����ӵ� |
			else if(canvas[i][j] == 3)
				printf("@");//����л� @  
			else
				printf(" ");//����ո�  
		}
		printf("\n");
	}
		printf("�÷֣�%d\n",score);
}

void updateWithoutlnput()//���û������޹صĸ���
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
					if(i == enemy_x[k] && j == enemy_y[k])//���ел� 
					{
						canvas[enemy_x[k]][enemy_y[k]] = 0;
						enemy_x[k] = 0;
						enemy_y[k] = rand() % Width; 
						canvas[enemy_x[k]][enemy_y[k]] = 3;
						
						score++;
					}	
				}
				
				
				//ʹ�ӵ��Զ������ƶ� 
				canvas[i][j] = 0;
				if(i > 0)
				canvas[i-1][j] = 2;	
			}
		}
	}
	
	for(k = 0 ; k < EnemyNum ; k++)
	{
		if(enemy_x[k] > High)//�л��ܳ��±߽磬�������ɵл� 
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
		//�л��Զ����� 

		for(k = 0 ; k < EnemyNum ; k++)
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k]++;
			canvas[enemy_x[k]][enemy_y[k]] = 3;
		}

		
		speed = 0;	
	}

}

void updateWithlnput()//���û��йصĸ��� 
{
	char input;
	if(kbhit())//������ʱ
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
	startup();//���ݳ�ʼ��
	while(1)//��Ϸѭ��ִ��
	{
		show();//��ʾ����
		updateWithoutlnput();//���û������޹صĸ���
		updateWithlnput();//���û��йصĸ��� 
	} 
	return 0; 
}
