
#ifndef POINT_GUARD
#define POINT_GUARD

struct Point {
    int x, y;
    Point(int xx, int yy) : x(xx), y(yy) { }
    Point() :x(0), y(0) { }
	const int GetX() const { return x; }
	const int GetY() const { return y; }
};

/*重载等号运算符*/
inline bool operator==(Point a, Point b) { return a.x==b.x && a.y==b.y; } 

/*重载不等号运算符*/
inline bool operator!=(Point a, Point b) { return !(a==b); }

#endif // POINT_GUARD

