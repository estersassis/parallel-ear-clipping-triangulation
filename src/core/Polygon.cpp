#include "Polygon.h"
#include "Geometry.h"
#include <fstream>
#include <iostream>

bool Polygon::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    int N;
    if (!(file >> N)) {
        std::cerr << "Erro ao ler o número de vértices." << std::endl;
        return false;
    }

    vertices_.reserve(N);
    active_vertex_count_ = N; // Todos estão ativos inicialmente

    for (int i = 0; i < N; ++i) {
        Vertex v;
        if (!(file >> v.x >> v.y)) {
            std::cerr << "Erro ao ler as coordenadas do vértice " << i << "." << std::endl;
            return false;
        }
        
        v.original_index = i;
        v.is_active = true;
        
        // Inicialização da Vizinhança por Índices
        v.prev_idx = (i == 0) ? N - 1 : i - 1;
        v.next_idx = (i == N - 1) ? 0 : i + 1;
        
        vertices_.push_back(v);
    }
    
    std::cout << "Leitura concluída. Vértices carregados: " << vertices_.size() << std::endl;
    return true;
}

double Polygon::signedArea(int i, int j, int k) const {
    const Vertex& v_i = vertices_[i];
    const Vertex& v_j = vertices_[j];
    const Vertex& v_k = vertices_[k];
    
    // Area = 0.5 * (Produto Vetorial 2D)
    return 0.5 * Geometry::crossProduct(v_i, v_j, v_k);
}

double Polygon::computeArea() const {
    double area = 0.0;
    int N = vertices_.size();
    
    for (int i = 0; i < N; ++i) {
        const Vertex& p_i = vertices_[i];
        const Vertex& p_i_next = vertices_[(i + 1) % N]; 
        
        // Fórmula do Laço (Shoelace Formula)
        area += (p_i.x * p_i_next.y - p_i_next.x * p_i.y);
    }
    
    return std::abs(area / 2.0);
}


void Polygon::updateNeighbors(int prev_idx, int curr_idx, int next_idx) {
    active_vertex_count_--; 
    
    vertices_[curr_idx].is_active = false; 

    vertices_[prev_idx].next_idx = next_idx;
    vertices_[next_idx].prev_idx = prev_idx;
}

bool Polygon::writeTrianglesToFile(const std::string& filename, 
                                 const std::vector<Triangle>& triangles) const {
    return true;
}

double Polygon::computeTrianglesArea(const std::vector<Triangle>& triangles) const {
    return -1.0;
}