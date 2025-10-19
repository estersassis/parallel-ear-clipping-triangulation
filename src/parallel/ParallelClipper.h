#ifndef PARALLELCLIPPER_H
#define PARALLELCLIPPER_H

#include "../core/EarClipper.h"
#include "../core/Polygon.h"
#include <vector>

/**
 * @class ParallelClipper
 * @brief Implementação paralela do algoritmo Ear-Clipping.
 */
class ParallelClipper : public EarClipper {
public:
    ParallelClipper(Polygon& poly) : EarClipper(poly) {}
    ~ParallelClipper() override = default;

    std::vector<Triangle> triangulate() override;

private:
    int current_vertex_index_ = 0; 
};

#endif // PARALLELCLIPPER_H