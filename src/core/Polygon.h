#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include "Vertex.h"
#include "Triangle.h"

/**
 * @class Polygon
 * @brief Gerencia a representação do polígono e as rotinas de I/O.
 */
class Polygon {
public:
    Polygon() = default;

    // Métodos de I/O
    bool readFromFile(const std::string& filename);
    bool writeTrianglesToFile(const std::string& filename, 
                              const std::vector<Triangle>& triangles) const;
    
    // Método para validação de corretude
    double computeArea() const; 
    double computeTrianglesArea(const std::vector<Triangle>& triangles) const;

    // Métodos de acesso (para serem usados pelos Clippers)
    std::vector<Vertex>& getVertices() { return vertices_; }
    int getActiveVertexCount() const { return active_vertex_count_; }

    // Método de atualização de vizinhança (Sequencial - será estendido para Lock-Free no paralelo)
    void updateNeighbors(int prev_idx, int curr_idx, int next_idx);
    
private:
    std::vector<Vertex> vertices_;
    int active_vertex_count_ = 0; 

    // Helper functions (e.g., para cálculo de área)
    double signedArea(int i, int j, int k) const; 
};

#endif // POLYGON_H