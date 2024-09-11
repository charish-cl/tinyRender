#include <iostream>

#include "tgaimage.h"
using namespace std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


//Bresenham's line algorithm
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

	bool isOverOne = false;

	//斜率判断 直接相除不好，直接移动到等式右边就行了
	if ((std::abs(y1 - y0)>std::abs(x1 - x0))) {
		isOverOne = true;
		//交换，x,y
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for(int x=x0; x<=x1; x++) {
		//点斜式方程求解
		float t =(x-x0)/(float)(x1-x0);
		int y = y0 +(y1-y0)*t;

		if(isOverOne) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}

	}
}


int main(int argc, char** argv) {
	TGAImage image(400, 400, TGAImage::RGB);


	// image.flip_horizontally(); // i want to have the y-axis pointing upwards
	line(13, 40, 80, 40, image, white); //线段A
	line(20, 13, 40, 80, image, red); //线段B
	line(80, 40, 13, 20, image, red);//线段C
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	std::cout << "Image saved to output.tga" << std::endl;
	return 0;
}


