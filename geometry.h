#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>

template <class t> struct Vec2 {
    t x, y;

    // 默认构造函数，不需要 Vec2<t>() 这种形式，改为 Vec2()
    Vec2() : x(t()), y(t()) {}

    // 带参数的构造函数
    Vec2(t _x, t _y) : x(_x), y(_y) {}

    // 拷贝构造函数
    Vec2(const Vec2<t> &v) : x(t()), y(t()) { *this = v; }

    // 赋值操作符
    Vec2<t>& operator =(const Vec2<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }

    // 加法操作符重载
    Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(x + V.x, y + V.y); }

    // 减法操作符重载
    Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(x - V.x, y - V.y); }

    // 乘法操作符重载
    Vec2<t> operator *(float f) const { return Vec2<t>(x * f, y * f); }

    // 下标操作符，用于访问 x 或 y，按索引来区分
    t& operator[](const int i) {
        if (i == 0) return x;  // i == 0 返回 x
        else return y;         // i != 0 返回 y
    }

    // 输出操作符重载，声明为友元函数
    template <class U>
    friend std::ostream& operator<<(std::ostream& s, const Vec2<U>& v);
};

// 输出操作符重载实现
template <class U>
std::ostream& operator<<(std::ostream& s, const Vec2<U>& v) {
    s << "(" << v.x << ", " << v.y << ")";
    return s;
}


template <class t> struct Vec3 {
    t x, y, z;
    Vec3() : x(t()), y(t()), z(t()) { }
    Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    template <class u> Vec3(const Vec3<u> &v);
    Vec3(const Vec3 &v) : x(t()), y(t()), z(t()) { *this = v; }
    Vec3 & operator =(const Vec3 &v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }
    Vec3 operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    Vec3 operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
    Vec3 operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
    Vec3 operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
    t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
    float norm () const { return std::sqrt(x*x+y*y+z*z); }
    Vec3 & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
    t& operator[](const int i) { if (i<=0) return x; else if (i==1) return y; else return z; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <> template <> Vec3<int>::Vec3(const Vec3<float> &v);
template <> template <> Vec3<float>::Vec3(const Vec3<int> &v);


template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

//////////////////////////////////////////////////////////////////////////////////////////////

const int DEFAULT_ALLOC=4;

class Matrix {
    std::vector<std::vector<float> > m;
    int rows, cols;
public:
    Matrix(int r=DEFAULT_ALLOC, int c=DEFAULT_ALLOC);
    inline int nrows();
    inline int ncols();

    static Matrix identity(int dimensions);
    std::vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();

    friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

/////////////////////////////////////////////////////////////////////////////////////////////


#endif //__GEOMETRY_H__