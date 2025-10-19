#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Vertex.h"
#include <vector>

/**
 * @namespace Geometry
 * @brief Funções geométricas estáticas e puras.
 */
namespace Geometry {
    /**
     * @brief Calcula a orientação (esquerda/direita) de um triplet de vértices.
     * @return double Valor do produto vetorial (orientação).
     */
    double crossProduct(const Vertex& p1, const Vertex& p2, const Vertex& p3);
    
    /**
     * @brief Determina se um vértice é convexo (baseado na orientação de seus vizinhos).
     */
    bool isConvex(const Vertex& prev, const Vertex& curr, const Vertex& next);

    /**
     * @brief Testa se um ponto (ponto_interno) está dentro de um triângulo (v1, v2, v3).
     */
    bool isPointInTriangle(const Vertex& v1, const Vertex& v2, 
                           const Vertex& v3, const Vertex& internal_point);
    
    /**
     * @brief Testa se um triângulo formado pelos vizinhos é uma orelha válida.
     */
    bool isEar(const Vertex& prev, const Vertex& curr, const Vertex& next,
               const std::vector<Vertex>& all_vertices);

}

#endif // GEOMETRY_H