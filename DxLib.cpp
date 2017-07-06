#include "DxLib.h"

SDL_Joystick* joystick;

bool keysHeld[SDLK_LAST];
bool sound = true;
void deinit();
int DxLib_Init()
{
    atexit(deinit);
    setlocale(LC_CTYPE, "ja_JP.UTF-8");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
	return -1;
    }

    if (!
	(screen =
	 SDL_SetVideoMode(480 /*(int)fmax/100 */ ,
			  420 /*(int)fymax/100 */ , 32,
			  SDL_SWSURFACE | SDL_DOUBLEBUF))) {
	SDL_Quit();
	return -1;
    }

    SDL_WM_SetCaption("Syobon Action (しょぼんのアクション)",
		      NULL);
    SDL_ShowCursor(SDL_DISABLE);

    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        fprintf(stderr, "Unable to init SDL_img: %s\n", IMG_GetError());
        return -1;
    }

    //Initialize font
    if (TTF_Init() == -1) {
	fprintf(stderr, "Unable to init SDL_ttf: %s\n", TTF_GetError());
	return -1;
    }

    //Audio Rate, Audio Format, Audio Channels, Audio Buffers
#define AUDIO_CHANNELS 2
    if (sound && Mix_OpenAudio(22050, AUDIO_S16SYS, AUDIO_CHANNELS, 1024)) {
        fprintf(stderr, "Unable to init SDL_mixer: %s\n", Mix_GetError());
        sound = false;
        }
    //Try to get a joystick
    joystick = SDL_JoystickOpen(0);

    for (int i = 0; i < SDLK_LAST; i++)
	keysHeld[i] = false;
    for (int i = 0; i < FONT_MAX; i++)
	font[i] = NULL;
    srand(time(NULL));

    return 0;
}

//Main screen
SDL_Surface *screen;

//Fonts
byte fontsize = 0;
TTF_Font *font[FONT_MAX];

//Strings
void SetFontSize(byte size)
{
    fontsize = size;
    if (font[size] == NULL) {
	font[size] = TTF_OpenFont("res/sazanami-gothic.ttf", size);
	if (font[size] == NULL) {
	    printf("Unable to load font: %s\n", TTF_GetError());
	    exit(1);
	}
    }
}

byte fontType = DX_FONTTYPE_NORMAL;
void ChangeFontType(byte type)
{
    fontType = type;
}

void DrawString(int a, int b, const char *x, Uint32 c)
{
    SDL_Color color = { c >> 16, c >> 8, c };
    SDL_Surface *rendered = TTF_RenderUTF8_Solid(font[fontsize], x, color);
    if (fontType == DX_FONTTYPE_EDGE) {
	SDL_Color blk = { 0, 0, 0 };
	SDL_Surface *shadow = TTF_RenderUTF8_Solid(font[fontsize], x, blk);
	DrawGraphZ(a - 1, b - 1, shadow);
	DrawGraphZ(a, b - 1, shadow);
	DrawGraphZ(a + 1, b - 1, shadow);
	DrawGraphZ(a - 1, b, shadow);
	DrawGraphZ(a + 1, b, shadow);
	DrawGraphZ(a - 1, b + 1, shadow);
	DrawGraphZ(a, b + 1, shadow);
	DrawGraphZ(a + 1, b + 1, shadow);
	SDL_FreeSurface(shadow);
    }
    DrawGraphZ(a, b, rendered);
    SDL_FreeSurface(rendered);
}

void DrawFormatString(int a, int b, Uint32 color, const char *str, ...)
{
    va_list args;
    char *newstr = new char[strlen(str) + 16];
    va_start(args, str);
    vsprintf(newstr, str, args);
    va_end(args);
    DrawString(a, b, newstr, color);
    delete newstr;
}

//void DrawFormatString(int a, int b, int c

//Key Aliases
#define KEY_INPUT_ESCAPE SDLK_ESCAPE

bool ex = false;

void UpdateKeys()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_KEYDOWN:
	    keysHeld[event.key.keysym.sym] = true;
	    break;
	case SDL_KEYUP:
	    keysHeld[event.key.keysym.sym] = false;
	    break;
	case SDL_JOYAXISMOTION:
	    if(event.jaxis.which == 0)
	    {
		if(event.jaxis.axis == JOYSTICK_XAXIS)
		{
		    if(event.jaxis.value < 0) keysHeld[SDLK_LEFT] = true;
		    else if(event.jaxis.value > 0) keysHeld[SDLK_RIGHT] = true;
		    else {
			keysHeld[SDLK_LEFT] = false;
			keysHeld[SDLK_RIGHT] = false;
		    }
		}
		else if(event.jaxis.axis == JOYSTICK_YAXIS)
		{
		    if(event.jaxis.value < 0) keysHeld[SDLK_UP] = true;
		    else if(event.jaxis.value > 0) keysHeld[SDLK_DOWN] = true;
		    else {
			keysHeld[SDLK_UP] = false;
			keysHeld[SDLK_DOWN] = false;
		    }
		}
	    }
	    break;
	case SDL_QUIT:
	    ex = true;
	    break;
	}
    }
}

byte ProcessMessage()
{
    return ex;
}

byte CheckHitKey(int key)
{
    if(key == SDLK_z && keysHeld[SDLK_SEMICOLON]) return true;
    return keysHeld[key];
}

byte WaitKey()
{
    SDL_Event event;
    while (true) {
	while (SDL_PollEvent(&event))
	    if (event.type == SDL_KEYDOWN)
		return event.key.keysym.sym;
    }
}

/*Uint32 GetColor(byte r, byte g, byte b)
{
    return r << 8 * 3 | g << 8 * 2 | b << 8 | 0xFF;
}*/

void DrawGraphZ(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;
        SDL_BlitSurface(mx, NULL, screen, &offset);
    }
}

void DrawTurnGraphZ(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;

        SDL_Surface *flipped = zoomSurface(mx, -1, 1, 0);
        SDL_SetColorKey(flipped, SDL_SRCCOLORKEY,
                SDL_MapRGB(flipped->format, 9 * 16 + 9, 255, 255));
        SDL_BlitSurface(flipped, NULL, screen, &offset);
        SDL_FreeSurface(flipped);
    }
}

void DrawVertTurnGraph(int a, int b, SDL_Surface * mx)
{
    if(mx)
    {
        SDL_Rect offset;
        offset.x = a - mx->w / 2;
        offset.y = b - mx->h / 2;

        SDL_Surface *flipped = rotozoomSurface(mx, 180, 1, 0);
        SDL_SetColorKey(flipped, SDL_SRCCOLORKEY,
                SDL_MapRGB(flipped->format, 9 * 16 + 9, 255, 255));
        SDL_BlitSurface(flipped, NULL, screen, &offset);
        SDL_FreeSurface(flipped);
    }
}

SDL_Surface *DerivationGraph(int srcx, int srcy, int width, int height,
			     SDL_Surface * src)
{
    SDL_Surface *img =
	SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
			     screen->format->BitsPerPixel,
			     src->format->Rmask, src->format->Bmask,
			     src->format->Gmask, src->format->Amask);

    SDL_Rect offset;
    offset.x = srcx;
    offset.y = srcy;
    offset.w = width;
    offset.h = height;

    SDL_BlitSurface(src, &offset, img, NULL);
    SDL_SetColorKey(img, SDL_SRCCOLORKEY,
		    SDL_MapRGB(img->format, 9 * 16 + 9, 255, 255));
    return img;
}

//Noticably different than the original
SDL_Surface *LoadGraph(const char *filename)
{
    SDL_Surface *image = IMG_Load(filename);

    if (image) return image;
	fprintf(stderr, "Error: Unable to load %s: %s\n", filename, IMG_GetError());
	exit(1);
}

void PlaySoundMem(Mix_Chunk* s, int l)
{
    if(sound) Mix_PlayChannel(-1, s, l);
}

Mix_Chunk* LoadSoundMem(const char* f)
{
    if(!sound) return NULL;

    Mix_Chunk* s = Mix_LoadWAV(f);
    if(s) return s;
    fprintf(stderr, "Error: Unable to load sound %s: %s\n", f, Mix_GetError());
    return NULL;
}

Mix_Music* LoadMusicMem(const char* f)
{
    if(!sound) return NULL;

    Mix_Music* m = Mix_LoadMUS(f);
    if(m) return m;
    fprintf(stderr, "Error: Unable to load music %s: %s\n", f, Mix_GetError());
    return NULL;
}

