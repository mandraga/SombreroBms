
int init_SDL(SDL_Window **psdlwindow, SDL_Renderer **sdlRenderer, int x, int y, int width, int height);
int close_SDL(SDL_Window *psdlwindow, SDL_Renderer *sdlRenderer);
void init_openGL(int width, int height);

void handle_mouse_down(int button);
void handle_mouse_up(int button);
void handle_input_events(Cappdata *app);

