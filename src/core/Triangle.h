#ifndef TRIANGLE_H
#define TRIANGLE_H

/**
 * @struct Triangle
 * @brief Representa um triângulo gerado, armazenando os índices originais dos vértices.
 */
struct Triangle {
    int v1_idx; 
    int v2_idx; 
    int v3_idx; 
    
    Triangle(int i, int j, int k) : v1_idx(i), v2_idx(j), v3_idx(k) {}
};

#endif // TRIANGLE_H