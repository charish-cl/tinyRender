#include <iostream>

#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


//Bresenham's line algorithm
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

	for(int x=x0; x<=x1; x++) {
		//点斜式方程求解
		float t =(x-x0)/(float)(x1-x0);
		int y = y0 +(y1-y0)*t;
		image.set(x, y, color);
	}
}


int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);

	// image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	// image.flip_horizontally(); // i want to have the y-axis pointing upwards
	line(0, 0, 100, 100, image, red); // draw a diagonal line from top left to bottom right corner

	image.write_tga_file("output.tga");
	std::cout << "Image saved to output.tga" << std::endl;
	return 0;
}


