typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

typedef u16 COLOR;

#define INLINE static inline

#define MEM_IO      0x04000000
#define MEM_VRAM    0x06000000

#define REG_DISPCNT     *((volatile u32*)(MEM_IO+0x0000))

#define SCREEN_WIDTH   240
#define SCREEN_HEIGHT  160
#define HALF_WIDTH     120
#define HALF_HEIGHT    80

#define vid_mem     ((u16*)MEM_VRAM)

INLINE void m3_plot(int x, int y, COLOR clr) {
  vid_mem[y*SCREEN_WIDTH+x]= clr;
}

INLINE COLOR RGB15(u32 red, u32 green, u32 blue) {
  return red | (green<<5) | (blue<<10);
}

INLINE u32 c_mix(u32 r, u32 l, u32 t, u32 len) {
  return (((l * t) + (r * (len - t))) / len);
}

int main()
{
    REG_DISPCNT= 0x0003 | 0x0400; // DCNT_MODE3 | DCNT_BG2

    const int startX = HALF_WIDTH;
    const int startY = HALF_HEIGHT - 32;

    for (int currY = 0; currY < 62; currY++) {
      const int startXL = startX - currY;
      const int startXR = startX + currY;

      u32 cLR = c_mix(31,  0, currY, 62);
      u32 cLG = c_mix( 0, 31, currY, 62);
      u32 cLB = c_mix( 0,  0, currY, 62);
      u32 cRR = c_mix(31,  0, currY, 62);
      u32 cRG = c_mix( 0,  0, currY, 62);
      u32 cRB = c_mix( 0, 31, currY, 62);

      for (int currX = 0; currX < (startXR - startXL); currX++) {
        u32 currR = c_mix(cLR, cRR, currX, (startXR - startXL));
        u32 currG = c_mix(cLG, cRG, currX, (startXR - startXL));
        u32 currB = c_mix(cLB, cRB, currX, (startXR - startXL));
        m3_plot(startXL+currX, startY+currY, RGB15(currR, currG, currB));
      }
    }

    while(1);

    return 0;
}