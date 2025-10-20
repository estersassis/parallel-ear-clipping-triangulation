#include "Geometry.h"
#include <cmath>

namespace Geometry {
    double crossProduct(const Vertex& p1, const Vertex& p2, const Vertex& p3) {
        // Z = (x2 - x1)(y3 - y2) - (y2 - y1)(x3 - x2)
        return (p2.x - p1.x) * (p3.y - p2.y) - 
               (p2.y - p1.y) * (p3.x - p2.x);
    }

    bool isConvex(const Vertex& prev, const Vertex& curr, const Vertex& next) {
        return false;
    }

    bool isPointInTriangle(const Vertex& v1, const Vertex& v2, 
                           const Vertex& v3, const Vertex& internal_point) {
        return false;
    }

    bool isEar(const Vertex& prev, const Vertex& curr, const Vertex& next,
               const std::vector<Vertex>& all_vertices) {
        return false;
    }
} // namespace Geometry