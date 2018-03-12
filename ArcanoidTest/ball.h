#pragma once

class Ball {
public: 
	float x, y, dx, dy, R; 
	bool active = false;  
	bool gameOver = false;
	Ball(float y_, float R_) : x(), y(y_), R(R_), dx(), dy(), active(false), gameOver(false) {
	
	}
	void move();
};
