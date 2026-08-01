/** @file "GameplayUtil.cpp" */

#include "Util/GameplayUtil.hpp"

#include <random>

namespace {
    std::mt19937& randGen() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    /// @brief Deterministic coordinate hash
    uint32_t Hash(const int x, const int y, const uint32_t seed) {
        uint32_t h = static_cast<uint32_t>(x) * 0x27d4eb2du
            ^ static_cast<uint32_t>(y) * 0x165667b1u
            ^ seed * 0x9e3779b9u;

        h = (h ^ (h >> 15)) * 0x85ebca6bu;
        h = (h ^ (h >> 13)) * 0xc2b2ae35u;
        h = h ^ (h >> 16);

        return h;
    }
}

int GameplayUtil::RandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(randGen());
}

int GameplayUtil::RandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(randGen());
}

int GameplayUtil::RandomBool() {
    return std::bernoulli_distribution(0.5)(randGen());
}

float GameplayUtil::LerpF(float start, float end, float alpha) {
    return start + ((end - start) * alpha);
}

namespace
{
    namespace Perlin
    {
        constexpr float kInvSqrt2 = 0.70710678118654752f;

        constexpr const Vector2 kGradients[8] = {
            Vector2{ 1.0f,  0.0f}, Vector2{-1.0f,  0.0f},
            Vector2{ 0.0f,  1.0f}, Vector2{ 0.0f, -1.0f},
            Vector2{ kInvSqrt2,  kInvSqrt2}, Vector2{-kInvSqrt2,  kInvSqrt2},
            Vector2{ kInvSqrt2, -kInvSqrt2}, Vector2{-kInvSqrt2, -kInvSqrt2}
        };

        // fade curve: 6t^5 - 15t^4 + 10t^3
        float fade(const float t) {
            return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
        }

        float Remap01(const float value) {
            const float clamped = std::clamp(value, -1.0f, 1.0f);
            return (clamped + 1.0f) * 0.5f;
        }

        float GradDot(const int xi, const int yi, const uint32_t seed, const Vector2& distance) {
            const uint32_t hash = Hash(xi, yi, seed);
            const Vector2& gradient = kGradients[hash & 7];
            return gradient.Dot(distance);
        }

        float Noise(const Vector2& point, const uint32_t seed) {
            const int xi = static_cast<int>(std::floor(point.x));
            const int yi = static_cast<int>(std::floor(point.y));

            const float xf = point.x - std::floor(point.x);
            const float yf = point.y - std::floor(point.y);

            const float u = fade(xf);
            const float v = fade(yf);

            const float dotAA = GradDot(xi,     yi,     seed, Vector2{xf,        yf});
            const float dotBA = GradDot(xi + 1, yi,     seed, Vector2{xf - 1.0f, yf});
            const float dotAB = GradDot(xi,     yi + 1, seed, Vector2{xf,        yf - 1.0f});
            const float dotBB = GradDot(xi + 1, yi + 1, seed, Vector2{xf - 1.0f, yf - 1.0f});

            const float lerpX1 = GameplayUtil::LerpF(dotAA, dotBA, u);
            const float lerpX2 = GameplayUtil::LerpF(dotAB, dotBB, u);

            return GameplayUtil::LerpF(lerpX1, lerpX2, v);
        }
    }
}

float GameplayUtil::PerlinNoise(const Vector2& input, float frequency, uint32_t seed) {
    const Vector2 scaledInput = input * frequency;

    const float raw = Perlin::Noise(scaledInput, seed);

    return Perlin::Remap01(raw);
}