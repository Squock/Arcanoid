#include "ball.h"
#include "vars.h"
#include <iostream>
#include <fstream>
//#include "main.cpp"
using namespace std;
//#include "gameOver.h"
extern int score;

void Ball::move() {
	x += dx; 
	y += dy; 
	if (x - R <= 0) {
		x = R; 
		dx *= -1; 
	}
	else if (y - R <= 0){
		y = R; 
		dy *= -1; 
	}
	else if (x + R >= 300) {
		x = 300 - R;
		dx *= -1; 
	} 
	else if (y + R >= r_y && y + R <= r_y + r_h && x > r_x && x < r_x + r_w) {
		y = r_y - R;
		dy *= -1;
	}
	else if (y > 300) {
		cout << "\nthis is break"<< score;
		ofstream file;
		file.open("file.txt");
		file << score;
		file.close();
		gameOver = true;
		active = false;
	}	
}



