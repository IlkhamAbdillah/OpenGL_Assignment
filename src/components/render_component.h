#pragma once
#include "../config.h"

struct RenderComponent {
    unsigned int material, VAO, vertexCount;
    std::vector<unsigned int> materials;
    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> vertexCounts;
};