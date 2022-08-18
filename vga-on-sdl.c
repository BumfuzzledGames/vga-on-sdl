#include <SDL.h>
#ifdef WIN32
#include <windows.h>
#endif

#define WIDTH 320
#define HEIGHT 200
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SCREEN_PRESCALE 4

int main(int argc, char *argv[]) {
#	ifdef WIN32
	SetProcessDPIAware();
#	endif

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
			"VGA-ish",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_Surface *screen = SDL_LoadBMP("gods.bmp");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_Texture *screenBuffer = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			WIDTH, HEIGHT);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_Texture *screenBufferPrescale = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET,
			WIDTH * SCREEN_PRESCALE, HEIGHT * SCREEN_PRESCALE);

	for(;;) {
		for(SDL_Event event; SDL_PollEvent(&event); )
			if(event.type == SDL_QUIT)
				goto done;

		// Uncomment for very ugly palette rotation for testing
		//for(int i = 0; i < 10; i++) {
		//	screen->format->palette->colors[i].r++;
		//	screen->format->palette->colors[i].g++;
		//	screen->format->palette->colors[i].b++;
		//}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		{
			SDL_Surface *screenSurface;
			SDL_LockTextureToSurface(
					screenBuffer,
					NULL,
					&screenSurface);
			SDL_BlitSurface(screen, NULL, screenSurface, NULL);
			SDL_UnlockTexture(screenBuffer);
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_SetRenderTarget(renderer, screenBufferPrescale);
		SDL_RenderCopy(renderer, screenBuffer, NULL, NULL);
		SDL_SetRenderTarget(renderer, NULL);

		SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
		SDL_RenderCopy(renderer, screenBufferPrescale, NULL, NULL);

		SDL_RenderPresent(renderer);
	}

done:
	SDL_Quit();
	return 0;
}

