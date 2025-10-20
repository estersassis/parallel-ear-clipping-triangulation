#include "SequentialClipper.h"
#include "../core/Geometry.h"
#include <iostream>

std::vector<Triangle> SequentialClipper::triangulate() {
    
    std::vector<Vertex>& vertices = polygon_.getVertices();
    int N = vertices.size();

    if (N < 3) return {};

    int current_idx = current_vertex_index_;

    while (polygon_.getActiveVertexCount() > 3) {
        
        bool ear_found = false;
        int start_idx = current_idx; 

        do {
            if (vertices[current_idx].is_active) {
                
                int prev_idx = vertices[current_idx].prev_idx;
                int next_idx = vertices[current_idx].next_idx;

                const Vertex& prev = vertices[prev_idx];
                const Vertex& curr = vertices[current_idx];
                const Vertex& next = vertices[next_idx];
                
                if (Geometry::isEar(prev, curr, next, vertices)) {
                    
                    result_triangles_.emplace_back(curr.original_index, 
                                                  next.original_index, 
                                                  prev.original_index);
                                                  
                    polygon_.updateNeighbors(prev_idx, current_idx, next_idx);
                    
                    current_idx = next_idx; 
                    ear_found = true;
                    break;
                }
            }
            
            
            current_idx = vertices[current_idx].next_idx; 
            
            // Medida de segurança contra loops infinitos em polígonos não-simples
            if (current_idx == start_idx) {
                // Caso extremo: Não achamos orelhas (polígono não é simples ou tem erro)
                std::cerr << "ERRO: Não foi possível encontrar mais orelhas. Polígono não triangulável?" << std::endl;
                return result_triangles_;
            }

        } while (current_idx != start_idx); // Continua a busca até voltar ao ponto de partida
        
        if (!ear_found && polygon_.getActiveVertexCount() > 3) {
            std::cerr << "ERRO: O loop interno falhou em encontrar uma orelha válida." << std::endl;
            break; 
        }
    }
    
    if (polygon_.getActiveVertexCount() == 3) {
        int v_idx = -1;
        for (int i = 0; i < N; ++i) {
            if (vertices[i].is_active) {
                if (v_idx == -1) v_idx = i;
            }
        }

        int v1 = v_idx;
        int v2 = vertices[v1].next_idx;
        int v3 = vertices[v2].next_idx;
        
        // Adiciona o último triângulo
        result_triangles_.emplace_back(vertices[v1].original_index, 
                                      vertices[v2].original_index, 
                                      vertices[v3].original_index);
    }

    return result_triangles_;
}