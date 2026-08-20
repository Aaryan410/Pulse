#pragma once
#include "Particle.h"
#include <vector>
#include "EffectConfig.h"

class ParticleSystem {
    public:
        ParticleSystem(const EffectConfig& config);
        void spawnTrailParticle(float x, float y);
        void spawnBurst(float x, float y);

        void update(float deltaTime);

        const std::vector<Particle>& getParticles() const;

    private:
        std::vector<Particle> particles;
        EffectConfig config;
};