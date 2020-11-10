#include "Swimmer.h"

#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"
#include "Genome/GeneFactory.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome)
    : Swimmer(energy, transform, genome, {})
{
}

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, std::shared_ptr<Swimmer>&& parent)
    : Swimmer(energy, transform, genome, genome->GetPhenoType(*this), std::move(parent))
{
}

Swimmer::~Swimmer()
{
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantDied();
    }
}

std::shared_ptr<Entity> Swimmer::GiveBirth(const std::shared_ptr<Genome>& other)
{
    ++eggsLayed_;
    return std::make_shared<Egg>(shared_from_this(), TakeEnergy(100_mj), GetTransform(), genome_, other ? other : genome_, Random::Poisson(50u));
}

void Swimmer::AdjustVelocity(double adjustment)
{
    UseEnergy(350_uj * std::pow(adjustment, 2.0));
    SetVelocity(GetVelocity() + adjustment);
}

void Swimmer::AdjustBearing(double adjustment)
{
    UseEnergy(100_uj * std::pow(adjustment, 2.0));
    SetBearing(GetTransform().rotation + adjustment);
}

void Swimmer::TickImpl(EntityContainerInterface& container)
{
    if (closestLivingAncestor_ && !closestLivingAncestor_->Alive()) {
        closestLivingAncestor_ = FindClosestLivingAncestor();
    }

    if (brain_ && brain_->GetInputCount() > 0) {
        std::fill(std::begin(brainValues_), std::end(brainValues_), 0.0);
        for (auto& sense : senses_) {
            sense->Tick(brainValues_, container);
        }

        brain_->ForwardPropogate(brainValues_);

        for (auto& effector : effectors_) {
            effector->Tick(brainValues_, container);
        }
    }

    // Baseline energy use TODO adjust based on genome (sense + effector count e.t.c.)
    UseEnergy(20_uj);

    std::shared_ptr<Genome> otherGenes;
    container.ForEachCollidingWith(Circle{ GetTransform().x, GetTransform().y, GetRadius() }, [&](const std::shared_ptr<Entity>& other) -> void
    {
        if (other.get() != this) {
            if (FoodPellet* f = dynamic_cast<FoodPellet*>(other.get())) {
                FeedOn(*f, f->GetEnergy());
                assert(!f->Alive());
            } else if (Swimmer* s = dynamic_cast<Swimmer*>(other.get())) {
                otherGenes = s->genome_;
            }
        }
    });

    if (GetEnergy() > 300_mj) {
        container.AddEntity(GiveBirth(otherGenes));
    }
}

void Swimmer::DrawImpl(QPainter& paint)
{
    paint.save();
    paint.setBrush(GetColour());
    paint.drawEllipse({ GetTransform().x, GetTransform().y }, GetRadius(), GetRadius());
    paint.restore();

    for (auto& sense : senses_) {
        paint.save();
        sense->Draw(paint);
        paint.restore();
    }
    for (auto& effector : effectors_) {
        paint.save();
        effector->Draw(paint);
        paint.restore();
    }
}

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, const Phenotype& phenotype, std::shared_ptr<Swimmer>&& mother)
    : Entity(energy, transform, 6.0, phenotype.colour)
    , closestLivingAncestor_(std::move(mother))
    , genome_(genome)
    , brain_(phenotype.brain)
    , senses_(phenotype.senses)
    , effectors_(phenotype.effectors)
    , brainValues_(brain_->GetInputCount(), 0.0)
    , eggsLayed_(0)
    , totalDescendantCount_(0)
    , extantDescendantCount_(0)
{
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantBorn();
    }
}

std::shared_ptr<Swimmer> Swimmer::FindClosestLivingAncestor() const
{
    std::shared_ptr<Swimmer> ancestor = closestLivingAncestor_;
    while (ancestor && !ancestor->Alive()) {
        ancestor = ancestor->closestLivingAncestor_;
    }
    return ancestor;
}

void Swimmer::DescendantBorn()
{
    ++totalDescendantCount_;
    ++extantDescendantCount_;
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantBorn();
    }
}

void Swimmer::DescendantDied()
{
    --extantDescendantCount_;
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantDied();
    }
}
