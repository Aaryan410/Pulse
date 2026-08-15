#include "ParticleSystem.h"

#include <algorithm>
#include <cmath>
#include <random>
using namespace std;

namespace {
    random_device randomDevice;
    mt19937 randomGenerator(randomDevice());

    uniform_real_distribution<float> velocityDistribution (
        -1.5f,
        1.5f
    );

    uniform_real_distribution<float> lifetimeDistribution (
        0.4f,
        0.8f
    );

    uniform_int_distribution<int> sizeDistribution (
        5,
        10
    );

    uniform_int_distribution<int> colorDistribution (
        150,
        255
    );
}

void ParticleSystem::spawnTrailParticle (
    float x,
    float y
) {
    Particle particle{};

    particle.x = x;
    particle.y = y;

    particle.velocityX = velocityDistribution(randomGenerator);

    particle.velocityY = velocityDistribution(randomGenerator);

    particle.lifetime = lifetimeDistribution(randomGenerator);

    particle.maxLifetime = particle.lifetime;

    particle.size = sizeDistribution(randomGenerator);

    particle.red = colorDistribution(randomGenerator);

    particle.green = colorDistribution(randomGenerator);

    particle.blue = 255;

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
        3.0f,
        7.0f
    );

    for (int i = 0; i < 30; ++i)
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

        particle.red = colorDistribution(randomGenerator);

        particle.green = colorDistribution(randomGenerator);

        particle.blue = 255;

        particles.push_back(particle);
    }
}

void ParticleSystem::update (
    float deltaTime
) {
    for (Particle& particle : particles) {
        
        particle.velocityY += 0.35f * deltaTime * 60.0f;

        particle.velocityX *= 0.98f;
        particle.velocityY *= 0.98f;

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