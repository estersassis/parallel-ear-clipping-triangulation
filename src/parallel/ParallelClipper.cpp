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
        std::vector<int> independent_ears;
        // Vetor auxiliar para marcar vértices envolvidos em cortes neste round.
        // Se vertices[i] for true, ele é parte de um triângulo (orelha, prev ou next) já selecionado.
        std::vector<bool> locked_vertices(N, false);

        for (int ear_idx : candidate_ears) {
            int prev_idx = vertices[ear_idx].prev_idx;
            int next_idx = vertices[ear_idx].next_idx;

            // Verificação de Conflito:
            // O triângulo candidato é formado por {prev_idx, ear_idx, next_idx}.
            // Para garantir independência total (disjunção de vértices), nenhum desses 3
            // pode ter sido usado por outra orelha selecionada anteriormente.
            if (!locked_vertices[ear_idx] && !locked_vertices[prev_idx] && !locked_vertices[next_idx]) {
                
                // Seleciona esta orelha
                independent_ears.push_back(ear_idx);

                // Trava a vizinhança para evitar que vizinhos sejam selecionados neste round
                locked_vertices[ear_idx] = true;
                locked_vertices[prev_idx] = true;
                locked_vertices[next_idx] = true;
            }
        }

        std::cout << "DEBUG: " << independent_ears.size() << " orelhas independentes selecionadas para corte." << std::endl;
        
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