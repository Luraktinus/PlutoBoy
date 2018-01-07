#include "../../non_core/graphics_out.h"
#include "../../non_core/logger.h"

#include <SDL.h>

#ifdef DREAMCAST
#include <SDL/SDL_endian.h>
#endif

static SDL_Surface *screen;
static uint32_t *pixels;
static uint32_t previous[GB_PIXELS_Y][GB_PIXELS_X]; //Store last frame's pixels 
static int screen_width;
static int screen_height;

#ifdef PSP
static SDL_Joystick *stick;
#endif


/*  Initialise graphics and create win_x by win_y pixel
 *  screen. Keeps track of GB_PIXELS_Y by GB_PIXELS_X screen. 
 *  return  1 if successful, 0 otherwise */
int init_screen(int win_x, int win_y, uint32_t *p) {
    
    screen_width = win_x;
    screen_height = win_y;

    pixels = p;

    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK )==-1)) {
        log_message(LOG_ERROR, "Could not initialize SDL: %s.\n", SDL_GetError());
        return 0;
     }

#ifdef PSP
	stick = SDL_JoystickOpen(0);
	SDL_JoystickEventState(SDL_ENABLE);
#endif

    SDL_WM_SetCaption("Gameboy","");
    screen = SDL_SetVideoMode(screen_width, screen_height, 0 ,0);

   // cols[0] = SDL_MapRGB(screen->format, 255, 255, 255); /* White */
   // cols[1] = SDL_MapRGB(screen->format, 170, 170, 170); /* Light Grey */
   // cols[2] = SDL_MapRGB(screen->format, 85, 85, 85); /* Dark Grey */
   // cols[3] = SDL_MapRGB(screen->format, 0, 0, 0); /* Black */

   // cols[0] = SDL_MapRGB(screen->format, 155, 187, 14);
   // cols[1] = SDL_MapRGB(screen->format, 115, 160, 103);
   // cols[2] = SDL_MapRGB(screen->format, 53, 98, 55);
   // cols[3] = SDL_MapRGB(screen->format, 15, 56, 14);

   // colors[0] = SDL_MapRGB(screen->format, 255, 255, 255);
   // colors[1] = SDL_MapRGB(screen->format, 136, 192, 112);
   // colors[2] = SDL_MapRGB(screen->format, 48,  104, 80);
   // colors[3] = SDL_MapRGB(screen->format, 8, 24, 32);

    for (int i = 0; i < GB_PIXELS_Y; i++) {
        for (int j = 0; j < GB_PIXELS_X; j++) {
            previous[i][j] = -1;           
        }
    }

    return 1;

}


/* Draw a GameBoy pixel of the given co-ordinates and of
 * the given 32bit color */
static void draw_pix(Uint32 color, int y, int x)
{
    int width_inc = screen_width/GB_PIXELS_X;
    int height_inc = screen_height/GB_PIXELS_Y;
    SDL_Rect rect = {x * width_inc, y * height_inc, width_inc, height_inc};
    SDL_FillRect(screen, &rect, color);
}


/*  Update the screen output */
void draw_screen() {
    for (int y = 0; y < GB_PIXELS_Y; y++) {
        for (int x = 0; x < GB_PIXELS_X; x++) {
            
            Uint32 rgb = pixels[(y * GB_PIXELS_X) + x];
	    Uint32 current_pix = SDL_MapRGB(screen->format, (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF); 
            //No need to redraw unchanged pixels
            if (previous[y][x] != current_pix) {
                draw_pix(current_pix, y, x);
                previous[y][x] = current_pix;
            }
        }
    } 
    
    SDL_Flip(screen);   
}
