#include <iostream>

#include "model.h"
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

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
	//填充三角形
	//把三角形分成两个三角形，分别填充
	//看是不是空心三角形，如果是，就画线，如果不是，就画填充
	if(t0.x == t1.x && t0.y == t2.y) {return;}
	//根据y坐标排序
	if(t0.y > t1.y) {std::swap(t0, t1);}
	if(t0.y > t2.y) {std::swap(t0, t2);}
	if(t1.y > t2.y) {std::swap(t1, t2);}
	//画线
	for(int y=t0.y; y<=t2.y; y++) {
		bool isTop = y>t1.y;

	}
}
int main(int argc, char** argv) {
	int width = 400;
	int height = 400;
	TGAImage image(width, height, TGAImage::RGB);

	Model* model = new Model("../obj/african_head.obj"); //代码方式构造model

	//nfaces()返回模型的面数
	for (int i = 0; i < model->nfaces(); i++) {

		vector<int> face = model->face(i);//获得第i个面的顶点索引
		for (int j = 0; j < 3; j++) {

			Vec3f vert1 = model->vert(face[j]);//获得第j个顶点的xyz坐标
			Vec3f vert2 = model->vert(face[(j+1)%3]);//获得第j+1个顶点的xyz坐标

			//对vert1和vert2进行屏幕坐标转换
			float x1 = (vert1.x + 1) * width / 2;
			float y1 = (vert1.y + 1) * height / 2;
			float x2 = (vert2.x + 1) * width / 2;
			float y2 = (vert2.y + 1) * height / 2;
			//调用bresenham算法绘制直线
			line(x1, y1, x2, y2, image, red);

		}

	}

	// line(100, 100, 300, 300, image, red);
	image.flip_vertically();
	image.write_tga_file("output.tga");
	std::cout << "Image saved to output.tga" << std::endl;
	return 0;
}


