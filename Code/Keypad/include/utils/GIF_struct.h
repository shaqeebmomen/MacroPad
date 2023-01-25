#include <stdint.h> 
#ifndef GIF_H
#define GIF_H
struct GIF 
{
   uint8_t frame_count;
   uint8_t *frames;
};
#endif
