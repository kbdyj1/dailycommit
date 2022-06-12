#include <iostream>
#include <vector>

namespace { //=================================================================

struct Point {
    float x;
    float y;
};
struct Line {
    float sx;
    float sy;
    float ex;
    float ey;
};
void drawShape(const std::vector<Point>& points) {
    for (auto p : points) {
        std::cout << "line. point(" << p.x << ", " << p.y << ")" << "\n";
    }
}
class LineToPointAdapter {
    std::vector<Point> v;
public:
    LineToPointAdapter(const Line& line) {
        v.push_back(Point{line.sx, line.sy});
        v.push_back(Point{line.ex, line.ey});
    }
    std::vector<Point> points() const {
        return v;
    }
};

} //namespace =================================================================

void test_adapter()
{
    auto line = Line{0.0, 0.0, 1.0, 1.0};
    auto lineAdapter = LineToPointAdapter(line);
    drawShape(lineAdapter.points());
}
