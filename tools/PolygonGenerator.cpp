#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <random>


struct Point {
    double x;
    double y;
    double angle; // Usado para ordenação CCW
};

// Funções Helpers
double angle(double x, double y) { return std::atan2(y, x); }

// Compara pontos por ângulo para garantir a ordem CCW
bool comparePoints(const Point& a, const Point& b) {
    return a.angle < b.angle;
}

// Salva o polígono no formato .poly
void savePolygon(const std::string& filename, const std::vector<Point>& vertices) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << filename << std::endl;
        return;
    }
    
    file << vertices.size() << "\n";
    for (const auto& p : vertices) {
        file << p.x << " " << p.y << "\n";
    }
    file.close();
    std::cout << "Polígono de " << vertices.size() << " vértices salvo em: " << filename << std::endl;
}

// --------------------------------------------------------------------------
// Funções de Geração
// --------------------------------------------------------------------------

// A. Polígono Convexo Simples
void generateConvex(int N, double radius = 100.0) {
    std::vector<Point> vertices;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 2.0 * M_PI); // Ângulos de 0 a 2*PI

    for (int i = 0; i < N; ++i) {
        double theta = distrib(gen);
        // Gera o vértice no círculo, já ordenado (ou quase) se usarmos ângulos
        vertices.push_back({radius * std::cos(theta), radius * std::sin(theta), theta});
    }

    // Ordenação final para garantir CCW
    std::sort(vertices.begin(), vertices.end(), comparePoints);

    savePolygon("data/convex_n" + std::to_string(N) + ".poly", vertices);
}

// B. Polígono Côncavo Tipo Estrela (Simples)
void generateStar(int N, double innerRadius = 50.0, double outerRadius = 100.0) {
    std::vector<Point> vertices;
    double delta_angle = 2.0 * M_PI / N;

    for (int i = 0; i < N; ++i) {
        double theta = i * delta_angle;
        double radius = (i % 2 == 0) ? outerRadius : innerRadius;

        vertices.push_back({radius * std::cos(theta), radius * std::sin(theta), theta});
    }

    // A geração já é ordenada CCW
    savePolygon("data/star_n" + std::to_string(N) + ".poly", vertices);
}

// C. Polígono Espiral (Difícil para Ear-Clipping)
void generateSpiral(int N, double initialRadius = 50.0, double rate = 0.005) {
    std::vector<Point> vertices;
    double max_angle = N * rate;

    for (int i = 0; i < N; ++i) {
        double theta = i * rate;
        double radius = initialRadius + theta * (100.0 / max_angle); // Raio aumenta com o ângulo

        vertices.push_back({radius * std::cos(theta), radius * std::sin(theta), theta});
    }
    // A geração já é ordenada CCW (pois theta é crescente)
    savePolygon("data/spiral_n" + std::to_string(N) + ".poly", vertices);
}

// --------------------------------------------------------------------------
// Main do Gerador
// --------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <tipo> <N_vertices>" << std::endl;
        std::cerr << "Tipos: convex, star, spiral" << std::endl;
        return 1;
    }

    std::string type = argv[1];
    int N = std::stoi(argv[2]);

    if (N < 4) {
        std::cerr << "N de vertices deve ser maior ou igual a 4." << std::endl;
        return 1;
    }

    if (type == "convex") {
        generateConvex(N);
    } else if (type == "star") {
        // Para garantir que a estrela seja concava, N deve ser par
        generateStar((N % 2 != 0) ? N + 1 : N); 
    } else if (type == "spiral") {
        generateSpiral(N);
    } else {
        std::cerr << "Tipo de polígono inválido." << std::endl;
        return 1;
    }

    return 0;
}