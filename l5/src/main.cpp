#include <iostream>
#include <random>

#include <glm/glm.hpp>



const int SAMPLES_PER_PIXEL = 32;

const int width = 800;
const int height = 600;

const glm::vec3 BACKGROUND_COLOR = glm::vec3(0.2, 0.2, 0.3);
const glm::vec3 FLOOR_COLOR = glm::vec3(0.3, 0.3, 0.3);

const glm::vec3 sphereColor1 = glm::vec3(1.0, 0.0, 0.0);
const float sphereRadius1 = 1.0;
const glm::vec3 spherePosition1 = glm::vec3(1, 1, -5);

const glm::vec3 sphereColor2 = glm::vec3(0.0, 0.5, 1);
const float sphereRadius2 = 0.75;
const glm::vec3 spherePosition2 = glm::vec3(-3.5, 1.5, -4);

const float floorY = 0.0;

const glm::vec3 cameraPos(0, 1, 0);

glm::vec3 lightPosition = glm::vec3(-2, 5, -3);
glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
float lightRadius;



bool intersectSphere(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &lightDistance, const glm::vec3 &spherePosition, const float &sphereRadius) {
    const glm::vec3 oc = rayOrigin - spherePosition;

    const float a = dot(rayDirection, rayDirection);
    const float b = 2.0f * dot(oc, rayDirection);
    const float c = dot(oc, oc) - sphereRadius * sphereRadius;

    const float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        lightDistance = (-b - sqrt(discriminant)) / (2.0f * a);
        return lightDistance > 0;
    }

    return false;
}

bool intersectFloor(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &lightDistance) {
    if (std::abs(rayDirection.y) > 1e-6) {
        lightDistance = (floorY - rayOrigin.y) / rayDirection.y;
        return lightDistance > 0;
    }

    return false;
}


float calculateShadow(const glm::vec3 &point) {
    // random setup
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> randomDistance(-lightRadius, lightRadius);

    int shadowRaysHit = 0;
    for (int i = 0; i < SAMPLES_PER_PIXEL; i++) {
        const glm::vec3 randomOffset = glm::vec3(randomDistance(rng), 0, randomDistance(rng));
        const glm::vec3 jitteredLightPosition = lightPosition + randomOffset;
        const glm::vec3 jitteredLightDirection = glm::normalize(jitteredLightPosition - point);

        float _;
        if (
            intersectSphere(point + jitteredLightDirection * 1e-3f, jitteredLightDirection, _, spherePosition1, sphereRadius1) || 
            intersectSphere(point + jitteredLightDirection * 1e-3f, jitteredLightDirection, _, spherePosition2, sphereRadius2) || 
            intersectFloor(point + jitteredLightDirection * 1e-3f, jitteredLightDirection, _)
        ) {
            shadowRaysHit++;
        }
    }

    return float(shadowRaysHit) / SAMPLES_PER_PIXEL;
}

glm::vec3 traceRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) {
    float lightDistanceSphere1 = FLT_MAX, lightDistanceSphere2 = FLT_MAX, lightDistanceFloor = FLT_MAX;

    glm::vec3 outputColor = BACKGROUND_COLOR;

    bool hitFloor = intersectFloor(rayOrigin, rayDirection, lightDistanceFloor);
    if (hitFloor) {
        const glm::vec3 point = rayOrigin + lightDistanceFloor * rayDirection;
        const glm::vec3 normal = glm::vec3(0, 1, 0);
        const glm::vec3 lightDirection = normalize(lightPosition - point);

        float antiShadowFactor = 1.0f - calculateShadow(point);
        float intensity = std::max(dot(normal, lightDirection), 0.0f);
        glm::vec3 floorColor = FLOOR_COLOR * lightColor * intensity * antiShadowFactor;

        outputColor = floorColor;
    }

    bool hitSphere1 = intersectSphere(rayOrigin, rayDirection, lightDistanceSphere1, spherePosition1, sphereRadius1);
    if (hitSphere1) {
        const glm::vec3 point = rayOrigin + lightDistanceSphere1 * rayDirection;
        const glm::vec3 normal = glm::normalize(point - spherePosition1);
        const glm::vec3 lightDirection = glm::normalize(lightPosition - point);

        float antiShadowFactor = 1.0f - calculateShadow(point);
        float intensity = std::max(dot(normal, lightDirection), 0.0f);

        outputColor = sphereColor1 * lightColor * intensity * antiShadowFactor;
    }

    bool hitSphere2 = intersectSphere(rayOrigin, rayDirection, lightDistanceSphere2, spherePosition2, sphereRadius2);
    if (hitSphere2) {
        const glm::vec3 point = rayOrigin + lightDistanceSphere2 * rayDirection;
        const glm::vec3 normal = glm::normalize(point - spherePosition2);
        const glm::vec3 lightDirection = glm::normalize(lightPosition - point);

        float antiShadowFactor = 1.0f - calculateShadow(point);
        float intensity = std::max(dot(normal, lightDirection), 0.0f);

        outputColor = sphereColor2 * lightColor * intensity * antiShadowFactor;
    }

    return outputColor;
}



int main() {
    std::cin >> lightRadius;

    std::cout << "P3\n" << width << " " << height << "\n255\n";

    float fov = glm::radians(90.0f);
    float aspectRatio = float(width) / height;
    float scale = tan(fov / 2);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float ndcX = (2 * (x + 0.5f) / width - 1) * aspectRatio * scale;
            float ndcY = (1 - 2 * (y + 0.5f) / height) * scale;
            glm::vec3 rayDirection = normalize(glm::vec3(ndcX, ndcY, -1));

            glm::vec3 color = traceRay(cameraPos, rayDirection);

            std::cout << static_cast<int>(255 * std::clamp(color.r, 0.0f, 1.0f)) << " "
                      << static_cast<int>(255 * std::clamp(color.g, 0.0f, 1.0f)) << " "
                      << static_cast<int>(255 * std::clamp(color.b, 0.0f, 1.0f)) << "\n";
        }
    }

    return 0;
}
