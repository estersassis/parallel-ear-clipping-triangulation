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
        #pragma omp parallel
        {
            // Buffer local para armazenar os triângulos encontrados por esta thread
            // Isso evita contenção excessiva no vetor global 'result_triangles_'
            std::vector<Triangle> thread_triangles;

            #pragma omp for
            for (size_t i = 0; i < independent_ears.size(); ++i) {
                int ear_idx = independent_ears[i];
                
                // Acessa os vizinhos (leitura segura, pois garantimos independência no Passo 3)
                int prev_idx = vertices[ear_idx].prev_idx;
                int next_idx = vertices[ear_idx].next_idx;

                // 1. Cria o triângulo e armazena localmente
                thread_triangles.emplace_back(prev_idx, ear_idx, next_idx);

                // 2. Atualiza a vizinhança no polígono
                // IMPORTANTE: updateNeighbors decrementa 'active_vertex_count_'.
                // Como essa variável não é atómica no Polygon.cpp, precisamos de uma região crítica
                // para evitar Race Conditions no contador. A manipulação de ponteiros seria segura sem lock
                // devido à independência, mas o contador exige proteção.
                polygon_.updateNeighbors(prev_idx, ear_idx, next_idx);
            }

            // Consolida os triângulos desta thread no vetor de resultados global
            #pragma omp critical(merge_triangles)
            {
                result_triangles_.insert(result_triangles_.end(), thread_triangles.begin(), thread_triangles.end());
            }
        }

        std::cerr << "DEBUG: Fim do round (placeholder break)." << std::endl;
        addRound();
    }
    return result_triangles_;
}