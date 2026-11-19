#pragma once

#include <algorithm>
#include <cmath>

namespace launchpad_fly_in {

inline float SmoothStep(float value) {
    float t = std::clamp(value, 0.f, 1.f);
    return t * t * (3.f - 2.f * t);
}

// A critically damped unit-mass step, with zero initial velocity.
// Blend out the small residual in the tail so the finite timeline settles
// at exactly one with zero velocity and can be played back in either direction.
inline float Spring(float time) {
    float t = std::clamp(time, 0.f, 1.f);
    float x = 7.5f * t;  // Natural angular frequency times local duration.
    float residual = (1.f + x) * std::exp(-x);
    return 1.f - residual * (1.f - SmoothStep((t - .72f) / .28f));
}

struct Pose {
    float dx, dy, scale, opacity;
};

inline Pose Sample(float phase, float x, float y, float width, float height,
                   float margin, float delayFraction) {
    if (phase >= 1.f) return {0.f, 0.f, 1.f, 1.f};

    // Intersect the outward ray with a rectangle padded by the tile extent.
    // Normalize each axis separately to preserve the same effect on tall and
    // wide displays. The convergence point sits near the viewport centre.
    float originX = width * .5f, originY = height * .48f;
    float rayX = x - originX, rayY = y - originY;
    float radius = std::max(std::abs(rayX) / originX,
                            std::abs(rayY) / (rayY < 0 ? originY : height - originY));
    if (radius < .001f) rayY = -1.f;
    float edgeX = rayX < 0 ? originX + margin : width - originX + margin;
    float edgeY = rayY < 0 ? originY + margin : height - originY + margin;
    float reach = 1.f / std::max(std::abs(rayX) / edgeX, std::abs(rayY) / edgeY);
    float delay = delayFraction * std::clamp(radius, 0.f, 1.f);
    float settled = Spring((phase - delay) / (1.f - delay));
    float remaining = 1.f - settled;
    return {
        (originX + rayX * reach - x) * remaining,
        (originY + rayY * reach - y) * remaining,
        1.f + std::clamp(reach - 1.f, .50f, 1.55f) * remaining,
        SmoothStep(settled / .52f),
    };
}

// Analytically integrate the playback speed. When retargeted mid-flight, keep
// the previous speed and blend toward the new one with a 12 ms time constant.
// Position, scale and opacity continue to be sampled from the same timeline.
inline float Advance(float phase, double& velocity, float target, double elapsed,
                     int durationMs) {
    if (phase == target) {
        velocity = 0;
        return target;
    }
    constexpr double response = .012;
    double speed = (target == 1.f ? 1000.0 : -1000.0) / durationMs;
    if (velocity == 0 && (phase == 0.f || phase == 1.f)) velocity = speed;
    double time = std::max(0.0, elapsed);
    double blend = -std::expm1(-time / response);
    double next = phase + speed * time + (velocity - speed) * response * blend;
    velocity += (speed - velocity) * blend;
    constexpr double endpointTolerance = 1e-7;
    if (next <= endpointTolerance || next >= 1.0 - endpointTolerance) {
        velocity = 0;
        return next < .5 ? 0.f : 1.f;
    }
    return static_cast<float>(next);
}

}  // namespace launchpad_fly_in
