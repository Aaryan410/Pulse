#pragma once

struct EffectConfig {

    float trailMinVelocity = -1.5f;
    float trailMaxVelocity = 1.5f;

    float minLifetime = 0.4f;
    float maxLifetime = 0.8f;

    int minSize = 5;
    int maxSize = 10;

    int burstParticleCount = 30;
    float burstMinSpeed = 3.0f;
    float burstMaxSpeed = 7.0f;

    float gravity = 0.35f;
    float drag = 0.98f;

    int minColor = 150;
    int maxColor = 255;
};
