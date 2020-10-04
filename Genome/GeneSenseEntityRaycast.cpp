#include "GeneSenseEntityRaycast.h"

#include "Random.h"
#include "Swimmer.h"
#include "Sensors/SenseEntityRaycast.h"

GeneSenseEntityRaycast::GeneSenseEntityRaycast(std::vector<std::pair<double, Trait> >&& traitWeights, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double genericWeight)
    : GeneSenseEntityRaycast(std::move(traitWeights), std::make_shared<NeuralNetwork>(hiddenLayers, traitWeights.size() + 1, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(traitWeights.size() + 1, outputCount), transform, genericWeight, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseEntityRaycast::GeneSenseEntityRaycast(std::vector<std::pair<double, Trait> >&& traitWeights, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double genericWeight, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , transform_(transform)
    , genericWeight_(genericWeight)
    , traitWeights_(std::move(traitWeights))
{
}

std::shared_ptr<Gene> GeneSenseEntityRaycast::Mutate() const
{
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections = outputConnections_;
    Transform transform = transform_;
    double genericWeight = genericWeight_;
    std::vector<std::pair<double, Trait>> traitWeights = traitWeights_;

    switch(Random::Number(0, 4)) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputConnections = outputConnections->Mutated();
        break;
    case 2 :
        transform = {
            transform.x + Random::Gaussian(0.0, 0.05 + transform.x / 10),
            transform.y + Random::Gaussian(0.0, 0.05 + transform.y / 10),
            transform.rotation,
        };
        break;
    case 3 :
        genericWeight += Random::Gaussian(0.0, 0.05 + genericWeight / 10);
        break;
    case 4 :
        switch(Random::Number(0, 3)) {
        case 0 :
            // Adjust trait strength
            Random::Item(traitWeights).first += Random::Gaussian(0.0, 0.05 + Random::Item(traitWeights).first / 10);
            break;
        case 1 :
            // Switch trait
            Random::Item(traitWeights).second = Random::Item(ALL_TRAITS);
            break;
        case 2 :
            // Add trait
            // TODO requires ability to add/remove columns in network & connectors
            break;
        case 3 :
            // Remove trait
            // TODO requires ability to add/remove columns in network & connectors
            break;
        }
        break;
    }

    return std::make_shared<GeneSenseEntityRaycast>(std::move(traitWeights),
                                                    network,
                                                    outputConnections,
                                                    transform,
                                                    genericWeight,
                                                    GetMutatedDominance(),
                                                    GetMutatedMutationProbability());
}

void GeneSenseEntityRaycast::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    double distance = std::sqrt(GetDistanceSquare({ 0, 0 }, { transform_.x, transform_.y }));
    double angle = GetBearing({ 0, 0 }, { transform_.x, transform_.y });
    auto traitWeights = traitWeights_;
    target.senses.push_back(std::make_shared<SenseEntityRaycast>(network_, outputConnections_, owner, distance, angle, genericWeight_, std::move(traitWeights)));
}
