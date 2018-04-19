
#ifndef POINT_GUARD
#define POINT_GUARD

struct Point {
    int x, y;
    Point(int xx, int yy) : x(xx), y(yy) { }
    Point() :x(0), y(0) { }
	const int GetX() const { return x; }
	const int GetY() const { return y; }
};

/*���صȺ������*/
inline bool operator==(Point a, Point b) { return a.x==b.x && a.y==b.y; } 

/*���ز��Ⱥ������*/
inline bool operator!=(Point a, Point b) { return !(a==b); }

#endif // POINT_GUARD

