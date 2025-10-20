#include <iostream>
#include <string>
#include <chrono>
#include <memory>
#include <fstream>
#include <iomanip>
#include "core/Polygon.h"
#include "core/EarClipper.h"
#include "serial/SequentialClipper.h"
#include "parallel/ParallelClipper.h"

void log_results_to_csv(const std::string& mode, int N, long long duration_ms, 
                        int num_rounds, bool success) {
    
    std::ofstream csv_file("baseline_results.csv", std::ios::app);

    if (!csv_file.is_open()) {
        std::cerr << "AVISO: Nao foi possivel abrir/criar baseline_results.csv para log." << std::endl;
        return;
    }

    csv_file.seekp(0, std::ios::end);
    if (csv_file.tellp() == 0) {
        csv_file << "N;Modo;Tempo_ms;Num_Rounds;Corretude_Sucesso\n";
    }

    csv_file << N << ";"
             << mode << ";"
             << duration_ms << ";"
             << num_rounds << ";"
             << (success ? "SIM" : "NAO") << "\n";
}

void print_usage(const std::string& appName) {
    std::cerr << "Uso: " << appName << " <modo> <arquivo_entrada> <arquivo_saida>" << std::endl;
    std::cerr << "Modos disponíveis:" << std::endl;
    std::cerr << "  serial    - Executa a versão sequencial (baseline)." << std::endl;
    std::cerr << "  parallel  - Executa a versão paralela com OpenMP." << std::endl;
    std::cerr << "Nota: O número de threads para o modo 'parallel' é definido via OMP_NUM_THREADS." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    Polygon polygon;
    if (!polygon.readFromFile(input_file)) {
        std::cerr << "Erro: Falha ao ler o arquivo de polígono: " << input_file << std::endl;
        return 1;
    }
    std::cout << "Polígono lido com " << polygon.getVertices().size() << " vértices." << std::endl;

    std::unique_ptr<EarClipper> clipper;

    if (mode == "serial") {
        clipper = std::make_unique<SequentialClipper>(polygon);
        std::cout << "Modo: Sequencial (Baseline)" << std::endl;
    } else if (mode == "parallel") {
        clipper = std::make_unique<ParallelClipper>(polygon);
        std::cout << "Modo: Paralelo (OpenMP)" << std::endl;
    } else {
        std::cerr << "Erro: Modo de execução inválido." << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<Triangle> result_triangles = clipper->triangulate();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Triangulação concluída." << std::endl;
    std::cout << "Triângulos gerados: " << result_triangles.size() << std::endl;
    std::cout << "Tempo de Execução (" << mode << "): " << duration_ms.count() << " ms" << std::endl;

    if (mode == "parallel") {
        std::cout << "Rounds de Processamento: " << clipper->getNumRounds() << " (Métrica de Escalabilidade)" << std::endl;
    }

    double original_area = polygon.computeArea();
    double triangles_area = polygon.computeTrianglesArea(result_triangles);
    bool correctness_success = (std::abs(original_area - triangles_area) < 1e-9);

    if (correctness_success) { 
        std::cout << "Corretude Geométrica: SUCESSO (Áreas Coincidentes)" << std::endl;
    } else {
        std::cerr << "Corretude Geométrica: FALHA (Áreas Divergentes)" << std::endl;
        std::cerr << "Área Original: " << original_area << ", Área dos Triângulos: " << triangles_area << std::endl;
    }

    log_results_to_csv(mode, 
                   polygon.getVertices().size(), 
                   duration_ms.count(),
                   clipper->getNumRounds(),
                   correctness_success);

    if (!polygon.writeTrianglesToFile(output_file, result_triangles)) {
        std::cerr << "Erro: Falha ao escrever o arquivo de triângulos: " << output_file << std::endl;
        return 1;
    }
    std::cout << "Resultados salvos em " << output_file << std::endl;
    
    return 0;
}