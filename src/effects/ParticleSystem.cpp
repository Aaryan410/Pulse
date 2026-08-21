#include "ParticleSystem.h"

#include <algorithm>
#include <cmath>
#include <random>
using namespace std;

namespace {
    random_device randomDevice;
    mt19937 randomGenerator(randomDevice());
}

ParticleSystem::ParticleSystem(
    const EffectConfig& effectConfig
)
    : config{effectConfig} {

}

void ParticleSystem::spawnTrailParticle (
    float x,
    float y
) {

    uniform_real_distribution<float> velocityDistribution (
        config.trailMinVelocity,
        config.trailMaxVelocity
    );

    uniform_real_distribution<float> lifetimeDistribution (
        config.minLifetime,
        config.maxLifetime
    );

    uniform_int_distribution<int> sizeDistribution (
        config.minSize,
        config.maxSize
    );

    Particle particle{};

    particle.x = x;
    particle.y = y;

    particle.velocityX = velocityDistribution(randomGenerator);
    particle.velocityY = velocityDistribution(randomGenerator);

    particle.lifetime = lifetimeDistribution(randomGenerator);
    particle.maxLifetime = particle.lifetime;

    particle.size = sizeDistribution(randomGenerator);

    particle.startColor = config.startColor;
    particle.endColor = config.endColor;

    particles.push_back(particle);
}


void ParticleSystem::spawnBurst (
    float x,
    float y
) {
    uniform_real_distribution<float> angleDistribution (
        0.0f,
        6.283185f
    );

    uniform_real_distribution<float> speedDistribution (
        config.burstMinSpeed,
        config.burstMaxSpeed
    );

    uniform_real_distribution<float> lifetimeDistribution (
        config.minLifetime,
        config.maxLifetime
    );

    uniform_int_distribution<int> sizeDistribution (
        config.minSize,
        config.maxSize
    );

    for (int i = 0; i < config.burstParticleCount; ++i)
    {
        Particle particle{};

        particle.x = x;
        particle.y = y;

        float angle = angleDistribution(randomGenerator);

        float speed = speedDistribution(randomGenerator);

        particle.velocityX = cos(angle) * speed;
        particle.velocityY = sin(angle) * speed;

        particle.lifetime = lifetimeDistribution(randomGenerator);
        particle.maxLifetime = particle.lifetime;

        particle.size = sizeDistribution(randomGenerator);

        particle.startColor = config.startColor;
        particle.endColor = config.endColor;

        particles.push_back(particle);
    }
}

void ParticleSystem::update (
    float deltaTime
) {
    for (Particle& particle : particles) {
        
        particle.velocityY += config.gravity * deltaTime * 60.0f;

        particle.velocityX *= config.drag;
        particle.velocityY *= config.drag;

        particle.x += particle.velocityX * deltaTime * 60.0f;
        particle.y += particle.velocityY * deltaTime * 60.0f;

        particle.lifetime -= deltaTime;
    }

    particles.erase (
        remove_if (
            particles.begin(),
            particles.end(),
            [](const Particle& particle) {
                return particle.lifetime <= 0.0f;
            }
        ),
        particles.end()
    );
}

const vector<Particle>&
ParticleSystem::getParticles() const {
    return particles;
}