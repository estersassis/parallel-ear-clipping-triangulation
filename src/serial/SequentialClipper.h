#ifndef SEQUENTIALCLIPPER_H
#define SEQUENTIALCLIPPER_H

#include "../core/EarClipper.h"
#include "../core/Polygon.h"
#include <vector>

/**
 * @class SequentialClipper
 * @brief Implementação sequencial do algoritmo Ear-Clipping (Baseline).
 */
class SequentialClipper : public EarClipper {
public:
    SequentialClipper(Polygon& poly) : EarClipper(poly) {}
    ~SequentialClipper() override = default;

    std::vector<Triangle> triangulate() override;

private:
    int current_vertex_index_ = 0; 
};

#endif // SEQUENTIALCLIPPER_H