#ifndef MEATCHUNK_H
#define MEATCHUNK_H

#include "Entity.h"

class MeatChunk : public Entity {
public:
    MeatChunk(const Energy& energy, const Transform& transform, const double& speed);
    ~MeatChunk() override;

    virtual std::string_view GetName() const override { return "MeatChunk"; }
    virtual std::string_view GetDescription() const override;

protected:
    virtual void TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters) override final;
    virtual void DrawExtras(QPainter& paint) override final;

private:
    static double GetMeatChunkRadius(const Energy& energy);

    virtual std::vector<Property> CollectProperties() const override { return {}; }
};

#endif // MEATCHUNK_H
