// ConsoleApplication1.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <GL/glut.h>
#include <math.h>

#define CELLS_NO 51 //�Z���̑S�̐�
#define CELL_NEIGHBOR_NO 1 //�Z���̋ߖT��
#define RULE_NO 30 //�K��
#define TIME_LIM 50 //����
#define STATE_NO 2 //��Ԑ�
#define VIEW_SIZE 2.0 //��ʂ̕�

class Rule {
public:
	int ruleNo;
	int rulePatterns;
	Rule(int _ruleNo, int stateNo, int neighborNo) {
		ruleNo = _ruleNo;
		rulePatterns = (int)pow((double)STATE_NO, (double)2 * CELL_NEIGHBOR_NO + 1);
	}
	int getNextState(int decimalState) {
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

Rule thisRule = Rule(RULE_NO, STATE_NO, CELL_NEIGHBOR_NO);

void drawSquare(double x, double y, double width, double height) //render square
{
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y + height);
	glVertex2d(x, y + height);
	glEnd();
}

void drawCell(double x, double y, int state) //render single cell
{
	double width = VIEW_SIZE / CELLS_NO;
	glColor3d((STATE_NO - 1 - state) / (double)(STATE_NO - 1), (STATE_NO - 1 - state) / (double)(STATE_NO - 1), (STATE_NO - 1 - state) / (double)(STATE_NO - 1));
	drawSquare(x, y, width, width);
}

void drawCellsRow(int cells[CELLS_NO], int time) //render cells row
{
	int i;
	double width = VIEW_SIZE / CELLS_NO;
	double x = -VIEW_SIZE / 2.0;
	double y = (TIME_LIM / 2.0 - time) * width;
	for (i = 0; i < CELLS_NO; i++) {
		drawCell(x, y, cells[i]);
		x += width;
	}
}
//TODO
//cell update by rule
int updateCell(int neighborCells[CELL_NEIGHBOR_NO * 2 + 1])
{
	int nextCell = 0;
	for (int i = 0; i < CELL_NEIGHBOR_NO * 2 + 1; i++)
	{
		nextCell += neighborCells[i];
	}
	return nextCell% STATE_NO;
}

void display(void)
{
	for (int i = 0; i < thisRule.rulePatterns; i++)
	{
		printf("%d\n", thisRule.getNextState(i));
	}
	glClear(GL_COLOR_BUFFER_BIT);
	int cells[CELLS_NO] = { 0 }; //current cells state
	int tmpCells[CELLS_NO] = { 0 }; //temporary cells array
	cells[(int)(CELLS_NO / 2.0)] = 1;
	for (int i = 0; i < TIME_LIM; i++) {
		drawCellsRow(cells, i);
		for (int j = 0; j < CELLS_NO; j++)
		{
			int neighborCells[CELL_NEIGHBOR_NO * 2 + 1] = { 0 };
			for (int l = 0; l < CELL_NEIGHBOR_NO * 2 + 1; l++)
			{
				neighborCells[l] = cells[(j + l - CELL_NEIGHBOR_NO) % CELLS_NO];
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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}