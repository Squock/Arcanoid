#include <glut.h>
#include <windows.h>
#include <iostream>
#include <string> 
#include <math.h>
#include "ball.h"
#include "vars.h"
#include "Brick.h"
#include <fstream>
using namespace std;

//#include <stdafx.h>
float r_x=50.0, r_y=290.0, r_w=80.0, r_h=8.0;
int mouse_x;
Ball ball(r_y, 6);
const int iCol=10, jCol=15;
Brick brick[iCol][jCol];
int score = 0;

bool isCollision(Ball &b, Brick &br) {
	float x = b.x, y = b.y, r = b.R, c = br.col, w = br.w, row = br.row, h = br.h;
	if (br.active)
	{
		if (abs(x - c * w - w / 2 - 2) <= r + w / 2 && abs(y - row * h - h / 2 - 2) <= r + h / 2)
		{
			if (sqrt((c*w + w / 2 - x)*(c*w + w / 2 - x) + (row*h + h / 2 - y)*(row*h + h / 2 - y))
				- (w / 2 - 1)*sqrt(2.0) - r > r*(sqrt(2.0) - 1))
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

void drawScore(const char *text, int length, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.2, 0.2, 0.1);
	glRectf(r_x, r_y, r_x + r_w, r_y + r_h);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (float i = 0; i < 2 * 3.14; i += 3.14 / 4) {
		glVertex2f(ball.x + ball.R*sin(i), ball.y + ball.R*cos(i));
	}
	glEnd();
	glColor3f(0.1, 0.2, 0.8);
	glBegin(GL_QUADS);
	for (int i = 0; i < iCol; i++){
		for (int j = 0; j < jCol; j++) {
			Brick &b = brick[i][j];
			if (b.active) {
				glVertex2f(b.col*b.w+1, b.row*b.h+1);
				glVertex2f(b.col*b.w + b.w-1, b.row*b.h+1);
				glVertex2f(b.col*b.w + b.w-1, b.row*b.h + b.h-1);
				glVertex2f(b.col*b.w+1, b.row*b.h + b.h-1);
			}
		}
	}
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	string scoreText; 
	scoreText = "Score=" + std::to_string(score);
	drawScore(scoreText.data(), scoreText.size(), 0, 20);
	string str;
	string highScore;
	ifstream fin("file.txt");
	while (getline(fin, str)) {
		highScore = "HighScore=" + str;
		drawScore(highScore.data(), highScore.size(), 0, 0);
	}
	fin.close();

	if (ball.gameOver == true) {
		glColor3f(1.0, 0.0, 0.0);
		string gameOver;
		gameOver = "GAME OVER";
		drawScore(gameOver.data(), gameOver.size(), 350, 350);
		string gameOverPress;
		gameOverPress = "Press Y - restart or N - exit";
		drawScore(gameOverPress.data(), gameOverPress.size(), 220, 320);
	}
	glFlush(); 
	
	//glutSwapBuffers();
}

void Timer(int) {
	if (ball.active == true && ball.gameOver == false) {
		ball.move();
		for (int i = 0; i < iCol; i++) {
			for (int j = 0; j < jCol; j++) {
				Brick &b = brick[i][j];
				if (isCollision(ball, b)) {
					if (abs(ball.x - b.col*b.w - b.w / 2) < abs(ball.y - b.row*b.h - b.h / 2)) {
						ball.dy*=-1;
						score++;
					}
					else if (abs(ball.x - b.col*b.w - b.w / 2) > abs(ball.y - b.row*b.h - b.h / 2)) {
						ball.dx *= -1;
						score++;
					}
					else {
						if (ball.dx > 0) {
							if (ball.x < b.col*b.w + 1) ball.dx*=-1; //score++; 
						}
						else if (ball.x > (b.col + 1)*b.w - 1) ball.dx*=-1; //score++; 
						if (ball.dy > 0) {
							if (ball.y < b.row*b.h + 1) ball.dy *= -1; //score++; 
						}
						else if (ball.y > (b.row + 1)*b.h - 1) ball.dy *= -1;// score++; 
					}
					if (--b.hit == 0) {
						b.active = false;
					}
					goto o;
				}
			}
		}
	}
o:	Display();
	glutTimerFunc(33, Timer, 0);
}
void Mouse(int ax, int ay) {
	mouse_x = ax;
	r_x = ax - r_w/2;
	if (!ball.active) {
		ball.x = r_x + r_w / 2;
		ball.y = r_y - ball.R;
	}
}
void MousePress(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && !ball.active) {
		ball.active = true;
		ball.dx = 4; 
		ball.dy = -4;
	}
}
void Init() {
	/*glClearColor(0.8, 1.0, 0.6, 1.0);
	//glOrtho(0, 300, 300, 0, 1, 0);
	glColor3ub(0, 0, 0);
	glRectf(-0.2f, 0.4f, 0.2f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-200.0, 200.0, -200.0, 200.0, -5.0, 5.0);*/
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 300, 300, 0, 1, 0);
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == 'y' && ball.gameOver == true) {
		cout << "Must be are restart";
	}
	if (key == 'n' && ball.gameOver == true) {
		exit(0);
	}
}

int main() {
	bool restart = false;
	do {
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500);
		glutInitWindowPosition(100, 140);
		glutCreateWindow("Arcanoid");
		glutDisplayFunc(Display);
		glutTimerFunc(33, Timer, 0);
		glutPassiveMotionFunc(Mouse);
		glutMouseFunc(MousePress);
		glutKeyboardFunc(Keyboard);
		Init();
		for (int i = 0; i < iCol; i++) {
			for (int j = 0; j < jCol; j++) {
				brick[i][j].active = true;
				brick[i][j].col = j;
				brick[i][j].row = i;
			}
		}
		glutMainLoop();
		//glutInit(&argc, argv);
	} while (restart);
}