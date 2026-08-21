#include "EffectPresets.h"

namespace EffectPresets {

    EffectConfig createDefault() {
        EffectConfig config;
        return config;
    }

    EffectConfig createFire() {
        EffectConfig config;

        config.trailMinVelocity = -0.8f;
        config.trailMaxVelocity = 0.8f;

        config.gravity = -0.8;
        config.drag = 0.96;

        config.minLifetime = 0.6f;
        config.maxLifetime = 1.4f;

        config.minSize = 3;
        config.maxSize = 8;

        config.burstParticleCount = 35;
        config.burstMinSpeed = 1.5f;
        config.burstMaxSpeed = 4.0f;

        config.startColor = {255, 180, 40};
        config.endColor = {255, 30, 0};

        return config;
    }

    EffectConfig createExplosion() {
        EffectConfig config;

        config.gravity = 0.15f;
        config.drag = 0.96f;

        config.minLifetime = 0.3f;
        config.maxLifetime = 0.9f;

        config.minSize = 5;
        config.maxSize = 12;

        config.burstParticleCount = 60;
        config.burstMinSpeed = 4.0f;
        config.burstMaxSpeed = 10.0f;

        config.startColor = {255, 240, 80};
        config.endColor = {255, 50, 0};

        return config;
    }

}
