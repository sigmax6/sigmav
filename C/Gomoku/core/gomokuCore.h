/*
 * gomokuCore.h
 *
 *  Created on: 2009-10-27
 *      Author: kos
 */

#ifndef GOMOKUCORE_H_
#define GOMOKUCORE_H_
#include "statement.h"
#include <iostream>
using namespace std;
class GomokuCore
{
public:
	GomokuCore();
	/*
	 * !!!!!!!!!注意，数组下标是从1开始，而不是0;
	 */
	char board[GO_SIZE][GO_SIZE]; //*代表电脑,#代表人,NULL代表空
	pair<int, int> PC();
	void man(int mouse_x ,int mouse_y);
	void outputArray();

private:
	int man_x;
	int man_y;

	int PC_x;
	int PC_y;

	void init();
	int judge();
	/**
	 * 判断同一行最多的个数
	 *  注意此处不能用[][]和**
	 */
	int maxSize(char *board ,int x ,int y ,char sign);
	void boardInit();
};
#endif /* GOMOKUCORE_H_ */
