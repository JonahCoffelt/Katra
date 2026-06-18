#ifndef PYVK_CONSTANTS_H
#define PYVK_CONSTANTS_H

const int MAX_FRAMES_IN_FLIGHT = 2;

// #define NDEBUG  // Comment to enable debug
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

#endif