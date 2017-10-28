// 1dCellularAutomata.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

#define CELLS_NO 51 //�Z���̑S�̐�
#define CELL_NEIGHBOR_NO 1 //�Z���̋ߖT��
#define RULE_NO 90 //�K��
#define TIME_LIM 51 //����
#define STATE_NO 2 //��Ԑ�
#define VIEW_SIZE 2.0 //��ʂ̕�

class Rule
{
public:
	int ruleNo;
	int rulePatterns;

	Rule(int _ruleNo = 0, int stateNo = 2, int neighborNo = 1)
	{
		ruleNo = _ruleNo;
		rulePatterns = int(pow(double(STATE_NO), double(2) * CELL_NEIGHBOR_NO + 1));
	}

	int getNextState(int decimalState)
	{
		int nextState = 0;
		int leftDecimal = ruleNo;
		for (int i = 0; i <= decimalState; i++)
		{
			nextState = leftDecimal % STATE_NO;
			leftDecimal /= STATE_NO;
		}
		return nextState;
	}
};

Rule thisRule = Rule();

//render square
void drawSquare(double x, double y, double width, double height)
{
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y + height);
	glVertex2d(x, y + height);
	glEnd();
}

//render single cell
void drawCell(double x, double y, int state)
{
	double width = VIEW_SIZE / CELLS_NO;
	glColor3d((STATE_NO - 1 - state) / double(STATE_NO - 1), (STATE_NO - 1 - state) / double(STATE_NO - 1),
	          (STATE_NO - 1 - state) / double(STATE_NO - 1));
	drawSquare(x, y, width, width);
}

//render cells row
void drawCellsRow(int cells[CELLS_NO], int time)
{
	double width = VIEW_SIZE / CELLS_NO;
	double x = -VIEW_SIZE / 2.0;
	double y = (TIME_LIM / 2.0 - double(time) - 1.0) * width;
	for (int i = 0; i < CELLS_NO; i++)
	{
		drawCell(x, y, cells[i]);
		x += width;
	}
}

//cell update by Rule
int updateCell(int neighborCells[CELL_NEIGHBOR_NO * 2 + 1])
{
	int decimalState = 0;
	int cellMaxNo = CELL_NEIGHBOR_NO * 2 + 1;
	for (int i = 0; i < cellMaxNo; i++)
	{
		decimalState += int(pow(double(STATE_NO), double(i))) * neighborCells[cellMaxNo - i - 1];
	}
	int n = thisRule.getNextState(decimalState);
	return n;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	int cells[CELLS_NO] = {0}; //current cells state
	int tmpCells[CELLS_NO] = {0}; //temporary cells array
	cells[int(CELLS_NO / 2.0)] = 1;
	for (int i = 0; i < TIME_LIM; i++)
	{
		drawCellsRow(cells, i);
		for (int j = 0; j < CELLS_NO; j++)
		{
			int neighborCells[CELL_NEIGHBOR_NO * 2 + 1] = {0};
			for (int l = 0; l < CELL_NEIGHBOR_NO * 2 + 1; l++)
			{
				int n = (j + l - CELL_NEIGHBOR_NO) % CELLS_NO;
				if (n < 0)
				{
					n += CELLS_NO;
				}
				neighborCells[l] = cells[n];
			}
			tmpCells[j] = updateCell(neighborCells);
		}
		for (int k = 0; k < CELLS_NO; k++)
		{
			cells[k] = tmpCells[k];
		}
	}
	glFlush();
}

void init(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
}

int main(int argc, char* argv[])
{
	int ruleNo = RULE_NO;
	std::cout << "rule no." << ruleNo << std::endl;
	//	std::cout << "rule no. up to " << pow(STATE_NO, pow(STATE_NO, CELL_NEIGHBOR_NO * 2 + 1)) << std::endl;
	//	std::cin >> ruleNo;
	//	while (ruleNo >= pow(STATE_NO, pow(STATE_NO, CELL_NEIGHBOR_NO * 2 + 1)) || ruleNo < 0)
	//	{
	//		std::cout << "out of possible rule No " << pow(STATE_NO, pow(STATE_NO, CELL_NEIGHBOR_NO * 2 + 1)) << ". type again... " << std::endl;
	//		std::cin >> ruleNo;
	//	}
	thisRule = Rule(ruleNo, STATE_NO, CELL_NEIGHBOR_NO);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
