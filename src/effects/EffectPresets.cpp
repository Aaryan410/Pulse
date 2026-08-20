#include "EffectPResets.h"

namespace EffectPresets {

    EffectConfig createDefault() {
        EffectConfig config;
        return config;
    }

    EffectConfig createFire() {
        EffectConfig config;

        config.gravity = -0.15f;
        config.drag = 0.97f;

        config.minLifetime = 0.5f;
        config.maxLifetime = 1.2f;

        config.minSize = 4;
        config.maxSize = 9;

        config.burstParticleCount = 20;
        config.burstMinSpeed = 2.0f;
        config.burstMaxSpeed = 5.0f;

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

        return config;
    }

}
