#include "SenseTraitsBase.h"

#include "Swimmer.h"

std::string SenseTraitsBase::ToString(SenseTraitsBase::Trait trait)
{
    switch (trait) {
    case Trait::Red : return "Red";
    case Trait::Green : return "Green";
    case Trait::Blue : return "Blue";
    case Trait::Energy : return "Energy";
    case Trait::Age : return "Age";
    case Trait::Size : return "Size";
    case Trait::Distance : return "Distance";
    case Trait::Health : return "Health";
    case Trait::Presence : return "Presence";
    }
    assert(false && "SenseTraitsBase::ToString(Trait), invalid trait.");
    return "";
}

SenseTraitsBase::TraitNormaliser SenseTraitsBase::DefaultNormalisation(const SenseTraitsBase::Trait& trait)
{
    switch (trait) {
    case Trait::Red :
        return { trait, { EoBE::Range(0.0, 0.9), EoBE::Range(-1.0, 1.0) } };
    case Trait::Green :
        return { trait, { EoBE::Range(0.0, 0.9), EoBE::Range(-1.0, 1.0) } };
    case Trait::Blue :
        return { trait, { EoBE::Range(0.0, 0.9), EoBE::Range(-1.0, 1.0) } };
    case Trait::Energy :
        return { trait, { EoBE::Range(0_j, 500_j), EoBE::Range(-1.0, 1.0) } };
    case Trait::Age :
        return { trait, { EoBE::Range(0.0, 50'000.9), EoBE::Range(-1.0, 1.0) } };
    case Trait::Size :
        return { trait, { EoBE::Range(0.0, 30.0), EoBE::Range(-1.0, 1.0) } };
    case Trait::Distance :
        return { trait, { EoBE::Range(0.0, 100.0), EoBE::Range(-1.0, 1.0) } };
    case Trait::Health :
        return { trait, { EoBE::Range(0.0, 100.0), EoBE::Range(-1.0, 1.0) } };
    case Trait::Presence :
        return { trait, { EoBE::Range(0.0, 10.0), EoBE::Range(0.0, 1.0) } };
    }
    assert(false && "Unimplemented Trait");
    return { static_cast<Trait>(-1), { EoBE::Range(0.0, 0.0), EoBE::Range(-1.0, 1.0) }};
}

SenseTraitsBase::SenseTraitsBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Transform& transform, std::vector<TraitNormaliser>&& toDetect)
    : Sense(network, outputConnections, owner)
    , transform_(transform)
    , toDetect_(std::move(toDetect))
{
}

void SenseTraitsBase::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseParameters& /*universeParameters*/) const
{
    // First adjust inputs for each entity detected
    FilterEntities(entities, [&](const Entity& entity)
    {
        EoBE::IterateBoth<TraitNormaliser, double>(toDetect_, inputs, [&](const TraitNormaliser& norm, double& input)
        {
            input += GetTraitFrom(entity, norm.trait);
        });
    });
    // Then normalise each input according to its normalisation range
    EoBE::IterateBoth<TraitNormaliser, double>(toDetect_, inputs, [&](const TraitNormaliser& norm, double& input)
    {
        input = norm.range.ConvertAndClamp(input);
    });
}

double SenseTraitsBase::GetTraitFrom(const Entity& target, Trait trait) const
{
    switch (trait) {
    case Trait::Red :
        return target.GetColour().redF();
    case Trait::Green :
        return target.GetColour().greenF();
    case Trait::Blue :
        return target.GetColour().blueF();
    case Trait::Energy :
        return target.GetEnergy();
    case Trait::Age :
        return target.GetAge();
    case Trait::Size :
        return target.GetRadius();
    case Trait::Distance : {
        Transform senseLocation = transform_ + owner_.GetTransform();
        return GetDistance({ senseLocation.x, senseLocation.y }, target.GetLocation());
    }
    case Trait::Health :
        if (const Swimmer* targetSwimmer = dynamic_cast<const Swimmer*>(&target)) {
            return targetSwimmer->GetHealth();
        } else {
            return 0.0;
        }
    case Trait::Presence :
        return 1.0;
    }
    assert(false && "Entity::GetTrait, unimplemented trait.");
    return 0.0;
}
