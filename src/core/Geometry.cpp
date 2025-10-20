#include "Geometry.h"
#include <cmath>

namespace Geometry {
    double crossProduct(const Vertex& p1, const Vertex& p2, const Vertex& p3) {
        // Z = (x2 - x1)(y3 - y2) - (y2 - y1)(x3 - x2)
        return (p2.x - p1.x) * (p3.y - p2.y) - 
               (p2.y - p1.y) * (p3.x - p2.x);
    }

    /**
     * @brief Determina se um vértice é convexo, assumindo ordem CCW.
     * @return True se o vértice 'curr' for convexo.
     */
    bool isConvex(const Vertex& prev, const Vertex& curr, const Vertex& next) {
        // Para um polígono CCW, convexidade significa um giro à esquerda (crossProduct >= 0).
        // Se o resultado for 0 (colinear), consideramos convexo para fins de ear-clipping.
        return crossProduct(prev, curr, next) >= 0;
    }

    /**
     * @brief Testa se um ponto (ponto_interno) está estritamente dentro de um triângulo (v1, v2, v3).
     * Nota: Assume que o triângulo (v1, v2, v3) é orientado CCW.
     */
    bool isPointInTriangle(const Vertex& v1, const Vertex& v2, 
                           const Vertex& v3, const Vertex& internal_point) {
        if ((internal_point.x == v1.x && internal_point.y == v1.y) || 
            (internal_point.x == v2.x && internal_point.y == v2.y) || 
            (internal_point.x == v3.x && internal_point.y == v3.y)) {
            return false;
        }

        double o1 = crossProduct(v1, v2, internal_point);
        double o2 = crossProduct(v2, v3, internal_point);
        double o3 = crossProduct(v3, v1, internal_point);

        bool s1 = (o1 > 0); // P está à esquerda de v1->v2?
        bool s2 = (o2 > 0); // P está à esquerda de v2->v3?
        bool s3 = (o3 > 0); // P está à esquerda de v3->v1?

        return s1 && s2 && s3;
    }

    /**
     * @brief Testa se o vértice 'curr' forma uma orelha válida.
     */
    bool isEar(const Vertex& prev, const Vertex& curr, const Vertex& next,
               const std::vector<Vertex>& all_vertices) {
        if (!isConvex(prev, curr, next)) {
            return false;
        }

        for (const auto& v : all_vertices) {
            // Ignora vértices inativos ou os vértices da própria orelha
            if (!v.is_active || 
                &v == &prev || &v == &curr || &v == &next) {
                continue;
            }

            // Se algum vértice ativo estiver dentro do triângulo, NÃO é uma orelha.
            if (isPointInTriangle(prev, curr, next, v)) {
                return false;
            }

            return true;
        }
    }
} // namespace Geometry