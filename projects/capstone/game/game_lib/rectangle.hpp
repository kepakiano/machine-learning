#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

class Rectangle{
public:
    float x,y,w,h;

    Rectangle(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h)
    {}

    bool overlapsX(const Rectangle& rhs, float overlap = 0.0) const {
      return x + w - overlap >= rhs.x && x + overlap <= rhs.x + rhs.w;
    }
    bool overlapsY(const Rectangle& rhs, float overlap = 0.0) const {
      return y + h - overlap >= rhs.y && y + overlap <= rhs.y + rhs.h;
    }
    bool overlaps(const Rectangle& rhs, float overlap = 0.0) const{
      return overlapsX(rhs, overlap) && overlapsY(rhs, overlap);
    }
};

#endif // RECTANGLE_HPP
