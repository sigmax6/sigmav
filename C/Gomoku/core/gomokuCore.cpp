/*
 * gomokuCore.cpp
 *
 *  Created on: 2009-10-27
 *      Author: kos
 */

#include "gomokuCore.h"

GomokuCore::GomokuCore()
{
	init();
}

void GomokuCore::init()
{
	boardInit();
}

void GomokuCore::boardInit()
{
	for (int i = 0; i < GO_SIZE; i++)
	{
		for (int j = 0; j < GO_SIZE; j++)
		{
			if (i == 0)
			{
				if (j < 10)
				{
					this->board[i][j] = (char) (j + 48);
				}
				else
				{
					this->board[i][j] = (char) (j + 65 - 10);
				}
			}
			if (j == 0)
			{
				if (i < 10)
				{
					this->board[i][j] = (char) (i + 48);
				}
				else
				{
					this->board[i][j] = (char) (i + 65 - 10);
				}
			}
			if ((i != 0) && (j != 0))
			{
				this->board[i][j] = ' ';
			}
		}
	}
}

int GomokuCore::judge()
{
	if (maxSize(&this->board[0][0], GO_SIZE, GO_SIZE, '*') == 5)
		return 1;
	else if (maxSize(&this->board[0][0], GO_SIZE, GO_SIZE, '#') == 5)
		return 2;

	return 0;
}

int GomokuCore::maxSize(char *board ,int x ,int y ,char sign)//注意数组越界！！！返回的最大数值为5
{
	int max_temp; //初始化为1，不是0
	int max = 1;
	for (int i = 1; i < x; i++)
	{
		for (int j = 1; j < y; j++)
		{
			if (board[i * y + j] == sign)
			{
				max_temp = 1;
				for (int m = j + 1; m < x && m < j + 5; m++) //右边
				{
					if (board[i * y + m] == sign)
					{
						max_temp++;
						if (max < max_temp)
						{
							max = max_temp;
						}
					}
					else
					{
						max_temp = 1;
						break;
					}
				}

				for (int n = i + 1; n < y && n < i + 5; n++) //下边
				{
					if (board[n * y + j] == sign)
					{
						max_temp++;
						if (max < max_temp)
						{
							max = max_temp;
						}
					}
					else
					{
						max_temp = 1;
						break;
					}
				}

				for (int m = i + 1, n = j + 1; n < y && m < x && m < i + 5 && n
						< j + 5; m++, n++)//东南边
				{

					if (board[m * y + n] == sign)
					{
						max_temp++;
						if (max < max_temp)
						{
							max = max_temp;
						}
					}
					else
					{
						max_temp = 1;
						break;
					}
				}

				for (int m = i + 1, n = j - 1; n >= 1 && m < x && m < i + 5
						&& n > j - 5; m++, n--)//西南边
				{

					if (board[m * y + n] == sign)
					{
						max_temp++;
						if (max < max_temp)
						{
							max = max_temp;
						}
					}
					else
					{
						max_temp = 1;
						break;
					}
				}

			}
		}
	}
	return max;
}

void GomokuCore::man(int x ,int y)
{
	board[x][y] = '#';
}

pair<int, int> GomokuCore::PC()
{
	int i, j;
	for (i = 1; i < GO_SIZE; i++)
		for (j = 1; j < GO_SIZE; j++)
			if (board[i][j] == ' ')
			{
				board[i][j] = '*';
				goto rtn;
			}
	rtn: cout << "first = " << i << " second = " << j << endl;
	return pair<int, int> (i, j);
}

//void GomokuCore::outputArray()
//{
//	for (int i = 0; i < GO_SIZE; i++)
//	{
//		for (int j = 0; j < GO_SIZE; j++)
//		{
//			cout << board[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
