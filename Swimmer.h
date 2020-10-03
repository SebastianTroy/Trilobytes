#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "NeuralNetwork.h"
#include "Genome/Genome.h"
#include "Sensors/Sense.h"
#include "Effectors/Effector.h"

#include <memory>

class Swimmer : public Entity {
public:
    Swimmer(Energy energy, const Transform& transform);
    Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome);
    virtual ~Swimmer() override final;

    virtual std::string_view GetName() const override { return "Swimmer"; }

    std::shared_ptr<Entity> GiveBirth(const std::shared_ptr<Genome>& other);
    unsigned GetBrainInputCount() const { return brain_->GetInputCount(); }
    unsigned GetBrainOutputCount() const { return brain_->GetOutputCount(); }

    const NeuralNetwork& InspectBrain() { return *brain_; };
    const std::vector<std::shared_ptr<Sense>>& InspectSenses() { return senses_; };
    const std::vector<std::shared_ptr<Effector>>& InspectEffectors() { return effectors_; };

    void AdjustVelocity(double adjustment);
    void AdjustBearing(double adjustment);

protected:
    virtual void TickImpl(EntityContainerInterface& container) override final;
    virtual void DrawImpl(QPainter& paint) override final;

private:
    std::shared_ptr<Genome> genome_;
    std::shared_ptr<NeuralNetwork> brain_;
    std::vector<std::shared_ptr<Sense>> senses_;
    std::vector<std::shared_ptr<Effector>> effectors_;
    std::vector<double> brainValues_;

    static std::vector<std::shared_ptr<Gene>> CreateDefaultGenome();

};

#endif // SWIMMER_H
