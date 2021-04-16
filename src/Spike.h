#ifndef SPIKE_H
#define SPIKE_H

#include "Entity.h"

class Spike : public Entity {
public:
    Spike(const Transform& transform);
    ~Spike() override;

    virtual std::string_view GetName() const override { return "Spike"; }
    virtual std::string_view GetDescription() const override;

protected:
    virtual void TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters) override;
    virtual void DrawExtras(QPainter& paint) override;

private:
    virtual std::vector<Property> CollectProperties() const override;
};

#endif // SPIKE_H
