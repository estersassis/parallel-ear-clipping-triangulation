#ifndef VERTEX_H
#define VERTEX_H

/**
 * @struct Vertex
 * @brief Representa um vértice do polígono.
 */
struct Vertex {
    double x; // Coordenada X
    double y; // Coordenada Y
    
    // Vizinhança
    int next_idx; 
    int prev_idx; 
    
    // Marcação Lógica
    bool is_active = true; 
    
    // Índice original no vetor de entrada
    int original_index; 
};

#endif // VERTEX_H