#pragma once

class Brick {
public:
	bool active;
	short row, col, hit; 
	float w, h; 
	Brick(short R_, short c_, float w_, float h_, short hit_) :
		row(R_), col(c_), hit(hit_), h(h_), active(false) {};
	Brick() : row(), col(), w(20), h(20), hit(1), active(false) {};
};