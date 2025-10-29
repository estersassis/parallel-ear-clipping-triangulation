#include "ParallelClipper.h"
#include "../core/Geometry.h"
#include <iostream>
#include <vector>
#include <omp.h>

std::vector<Triangle> ParallelClipper::triangulate() {

    std::vector<Vertex>& vertices = polygon_.getVertices();
    int N = vertices.size();

    if (N < 3) return {};

    // 0 = Não é orelha, 1 = É uma orelha candidata
    std::vector<int> is_candidate(N);

    while (polygon_.getActiveVertexCount() > 3) {
        std::vector<int> candidate_ears;

        #pragma omp parallel for
        for (int i = 0; i < N; ++i) {
            if (vertices[i].is_active) {
                int prev_idx = vertices[i].prev_idx;
                int next_idx = vertices[i].next_idx;

                const Vertex& prev = vertices[prev_idx];
                const Vertex& curr = vertices[i];
                const Vertex& next = vertices[next_idx];

                if (Geometry::isEar(prev, curr, next, vertices)) {
                    is_candidate[i] = 1;
                } else {
                    is_candidate[i] = 0;
                }
            } else {
                is_candidate[i] = 0;
            }
        }

        // Não é thread-safe, push back em paralelo pode causar problemas
        for (int i = 0; i < N; ++i) {
            if (is_candidate[i] == 1) {
                candidate_ears.push_back(i);
            }
        }

        std::cout << "DEBUG: Round encontrou " << candidate_ears.size() << " orelhas candidatas." << std::endl;

        if (candidate_ears.empty()) {
            std::cerr << "ERRO: Nenhuma orelha candidata encontrada." << std::endl;
            break;
        }

        // --- PASSO 3: FILTRO DE INDEPENDÊNCIA ---
        // TODO: Implementar a lógica para filtrar 'candidate_ears'
        // e gerar 'independent_ears'.
        // ...
        
        // --- PASSO 4: CORTE PARALELO---
        // TODO: Iterar sobre 'independent_ears' em paralelo
        // e atualizar o polígono (polygon_.updateNeighbors)
        // ...

        std::cerr << "DEBUG: Fim do round (placeholder break)." << std::endl;
        addRound();
        break; 
    }
    return result_triangles_;
}