#ifndef EARCLIPPER_H
#define EARCLIPPER_H

#include "Polygon.h"
#include "Triangle.h"
#include <vector>

class Polygon;

/**
 * @class EarClipper
 * @brief Classe base abstrata para o algoritmo de Triangulação por Ear-Clipping.
 * * Define a interface fundamental para as implementações sequencial e paralela, 
 * garantindo que ambas possuam o método principal de triangulação.
 */
class EarClipper {
public:

    EarClipper(Polygon& poly) : polygon_(poly) {}
    virtual ~EarClipper() = default;

    /**
     * @brief Executa o processo de triangulação no polígono fornecido.
     * @return std::vector<Triangle> Uma lista de triângulos gerados.
     */
    virtual std::vector<Triangle> triangulate() = 0; // Método puramente virtual

    /**
     * @brief Retorna o número de rounds de processamento (relevante apenas para a versão paralela).
     * @return int O número de rounds executados.
     */
    virtual int getNumRounds() const {
        return 1; 
    }

protected:
    Polygon& polygon_; 
    std::vector<Triangle> result_triangles_;
};

#endif // EARCLIPPER_H