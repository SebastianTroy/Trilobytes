#ifndef PHENOTYPE_H
#define PHENOTYPE_H

#include "Sensors/Sense.h"
#include "Effectors/Effector.h"

#include <Energy.h>
#include <NeuralNetwork.h>

#include <QColor>

#include <memory>
#include <vector>

struct Phenotype {
public:
    QColor colour;
    Energy baseMetabolism = 20_uj;
    std::vector<std::shared_ptr<Sense>> senses;
    std::shared_ptr<NeuralNetwork> brain;
    std::vector<std::shared_ptr<Effector>> effectors;
};

#endif // PHENOTYPE_H
