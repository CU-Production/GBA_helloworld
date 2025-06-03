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

int main()
{
    REG_DISPCNT= 0x0003 | 0x0400; // DCNT_MODE3 | DCNT_BG2

    int startX = HALF_WIDTH;
    int startY = HALF_HEIGHT - 32;

    for (int currY = 0; currY < 62; currY++) {
      int startXL = startX - currY;
      int startXR = startX + currY;

      for (int currX = startXL; currX < startXR; currX++) {
        m3_plot(currX, startY+currY, RGB15(31, 0, 0) );
      }
    }

    // m3_plot( 120, 80, RGB15(31, 0, 0) );
    // m3_plot( 136, 80, RGB15( 0,31, 0) );
    // m3_plot( 120, 96, RGB15( 0, 0,31) );

    while(1);

    return 0;
}