#pragma once

#include "../extern/stddef.hpp"

// GsLinkObject4 replaces the length bytes at each run head with the run count.
// The polygon mode and gradation flag survive linking.
static constexpr int32_t modelPolygonWords(uint32_t header)
{
    static constexpr uint8_t texturedWords[2][2][2] = {
        {{6, 7}, {8, 9}},
        {{7, 9}, {8, 11}},
    };
    static constexpr uint8_t untexturedWords[2][2][2] = {
        {{4, 5}, {5, 6}},
        {{4, 6}, {4, 7}},
    };

    const auto mode    = header >> 24;
    const bool raw     = (mode & 1) != 0;
    const bool quad    = (mode & 8) != 0;
    const bool gouraud = (mode & 0x10) != 0;
    if ((mode & 4) != 0) return texturedWords[raw][quad][gouraud];

    int words = untexturedWords[raw][quad][gouraud];
    // Gradated lit polygons store a color per vertex instead of one for the face.
    if (!raw && (header & (4 << 16)) != 0) words += 2 + quad;
    return words;
}
