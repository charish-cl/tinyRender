#include <iostream>

#include "model.h"
#include "tgaimage.h"
using namespace std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

Vec3f light_dir(0, 0, -1); //光照方向
//Bresenham's line algorithm
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool isOverOne = false;

    //斜率判断 直接相除不好，直接移动到等式右边就行了
    if ((std::abs(y1 - y0) > std::abs(x1 - x0))) {
        isOverOne = true;
        //交换，x,y
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x = x0; x <= x1; x++) {
        //点斜式方程求解
        float t = (x - x0) / (float) (x1 - x0);
        int y = y0 + (y1 - y0) * t;

        if (isOverOne) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

//相似三角形，传入两个点，一个y值，返回x值
int similarTriangle(Vec2i a, Vec2i b, int y) {
    if (a.y == b.y) // 防止除以零
        return a.x;
    return a.x + (y - a.y) * (b.x - a.x) / (b.y - a.y);
}

//计算质心坐标
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {

    //[u,v,1]和[AB,AC,PA]对应的x和y向量都垂直，所以叉乘
    //先算出[AB,AC,PA] x,y对应的向量
    Vec3f xVec = Vec3f(B.x - A.x, C.x - A.x, A.x - P.x);
    Vec3f yVec = Vec3f(B.y - A.y, C.y - A.y, A.y - P.y);
    //叉乘
    Vec3f u = xVec ^ yVec;
    //三点共线时，会导致u[2]为0，此时返回(-1,1,1)
    if (std::abs(u.z) < 1e-4)
        return Vec3f(-1, 1, 1);

    return Vec3f(1 - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);

}
int width = 400;
int height = 400;

void triangle(Vec3f *vertices, float *zbuffer, TGAImage &image, TGAColor color) {
    //填充三角形
    //计算aabb包围盒
    Vec3f v0 = vertices[0];
    Vec3f v1 = vertices[1];
    Vec3f v2 = vertices[2];
    //错误写法
    // Vec3f min_coord = Vec3f(std::min(std::min(v0.x, v1.x), v2.x), std::min(std::min(v0.y, v1.y), v2.y), 0);
    // Vec3f max_coord = Vec3f(std::max(std::max(v0.x, v1.x), v2.x), std::max(std::max(v0.y, v1.y), v2.y), 0);
    Vec3f min_coord = Vec3f(floor(std::min(std::min(v0.x, v1.x), v2.x)), floor(std::min(std::min(v0.y, v1.y), v2.y)), 0);
    Vec3f max_coord = Vec3f(ceil(std::max(std::max(v0.x, v1.x), v2.x)), ceil(std::max(std::max(v0.y, v1.y), v2.y)), 0);
    Vec3f P;
    for (P.x=min_coord.x; P.x<=max_coord.x; P.x++) {
        for (P.y=min_coord.y; P.y<=max_coord.y; P.y++) {
            {
                Vec3f bary = barycentric(vertices[0], vertices[1], vertices[2], P);
                ////判断是否在三角形内
                if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                //这里z设为0有问题啊，应该是顶点的z值，但是这里没有用到
                 // bary.z = 0;

                float z = (vertices[0].z * bary.x + vertices[1].z * bary.y + vertices[2].z * bary.z);

                //计算z值
                if (zbuffer[int(P.y * width + P.x)] < z) {
                    zbuffer[int(P.y * width + P.x)] = z;
                    //对color进行插值
                    // 对光照强度进行插值
                     color.r = (int)(color.r * bary.x + color.r * bary.y + color.r * bary.z);
                     color.g = (int)(color.g * bary.x + color.g * bary.y + color.g * bary.z);
                     color.b = (int)(color.b * bary.x + color.b * bary.y + color.b * bary.z);
                     color.a = 255;
                    image.set(P.x, P.y, color);
                }
            }
        }
    }
}
Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}
Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}
const int depth  = 255;
Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();
    Matrix res = Matrix::identity(4);
    for (int i = 0; i < 3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}

int main(int argc, char **argv) {


    float *zbuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = -1e30;
    }
    TGAImage image(width, height, TGAImage::RGB);
    Model *model = new Model("../obj/african_head.obj"); //代码方式构造model

    Vec3f camera(-3, 0, 3); // 将相机放在 (3, 3, 3) 位置，形成 45 度视角
    Vec3f center(0, 0, 0); // 模型中心
    Vec3f up(0, 1, 0);     // 世界坐标系的上方向
    // 计算视角矩阵 (LookAt矩阵)
    Matrix View = lookat(camera, center, up);
    //初始化透视矩阵
    Matrix Projection = Matrix::identity(4);
    //初始化视角矩阵
    Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
    //投影矩阵[3][2]=-1/c，c为相机z坐标
    Projection[3][2] = -1.f/camera.z;


    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i); //face是一个数组，用于存储一个面的三个顶点
        Vec3f screen_coords[3];
        Vec3f world_coords[3]; //新加入一个数组用于存放三个顶点的世界坐标
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
        screen_coords[j] = m2v(ViewPort *View * Projection * v2m(v));
            world_coords[j] = v; //世界坐标
        }
        //计算法向量
        Vec3f normal = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        normal.normalize();

        float light_intensity = normal * light_dir; //光照强度=法向量*光照方向   即法向量和光照方向重合时，亮度最高
        //强度小于0，说明平面朝向为内  即背面裁剪
        //计算光照
        //这里传入screen

        if (light_intensity > 0) {
            triangle(screen_coords, zbuffer,image,
                     TGAColor(light_intensity * 255, light_intensity * 255, light_intensity * 255, 255));
        }
    }
    // line(100, 100, 300, 300, image, red);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    std::cout << "Image saved to output.tga" << std::endl;
    return 0;
}
