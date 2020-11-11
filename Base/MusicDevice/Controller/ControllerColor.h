#ifndef CONTROLLER_COLOR_H
#define CONTROLLER_COLOR_H

#include <cstdint>

namespace base::musicDevice::controller
{
struct ColorARGB
{
   uint8_t a;
   uint8_t r;
   uint8_t g;
   uint8_t b;
};
} // namespace base::musicDevice::controller

#endif