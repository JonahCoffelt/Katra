#ifndef PYVK_CONSTANTS_H
#define PYVK_CONSTANTS_H

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

// #define NDEBUG  // Comment to enable debug
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

#endif