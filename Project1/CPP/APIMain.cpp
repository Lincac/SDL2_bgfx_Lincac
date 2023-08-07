#include<iostream>
#include<common.h>
#include<bgfx_utils.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_syswm.h>
#include<../Head/Camera.h>
#include<../Head/LoadShader.h>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

int mouse_x = 0;
int mouse_y = 0;
bool quit = false;

Camera camera;

void process_event(SDL_Event& event);

class Model : public entry::AppI {
public:
	Model() : entry::AppI("", "", "") {}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override{}
	int shutdown() override { return 0; }
	bool update() override { return true; }

	void init() {
		SDL_Init(SDL_INIT_EVENTS);
		 window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

		SDL_SysWMinfo wmi{};
		SDL_VERSION(&wmi.version);
		SDL_GetWindowWMInfo(window, &wmi);

		bgfx::Init init;
		init.type = bgfx::RendererType::OpenGL;
		init.vendorId = bgfx::getCaps()->vendorId;
		init.platformData.nwh = wmi.info.win.window;
		init.platformData.ndt = wmi.info.win.hinstance;
		init.resolution.width = WIDTH;
		init.resolution.height = HEIGHT;
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);

		bgfx::setDebug(BGFX_DEBUG_NONE);

		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		bgfx::ShaderHandle vshader = Shader::loadshader("shader_compile/vs_cube.bin");
		bgfx::ShaderHandle fshader = Shader::loadshader("shader_compile/fs_cube.bin");
		program = bgfx::createProgram(vshader, fshader, true  /* destroy shaders when program is destroyed */);
		mesh = meshLoad("resource/meshes/bunny.bin");

		timeOffset = bx::getHPCounter();
	}

	~Model() {
		meshUnload(mesh);
		bgfx::destroy(program);
		bgfx::shutdown();
		SDL_DestroyWindow(window);
	}

	void run(Camera& camera) {
		bgfx::setViewRect(0, 0, 0,uint16_t(WIDTH), uint16_t(HEIGHT));
		bgfx::touch(0);

		bgfx::setState(0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
		);

		float view[16];
		camera.mtxLookAt(view);
		float proj[16];
		bx::mtxProj(proj, 60.0f, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		float time = (float)((bx::getHPCounter() - timeOffset) / double(bx::getHPFrequency()));
		float mtx[16];
		bx::mtxRotateXY(mtx
			, 0.0f
			, time * 0.37f
		);
		meshSubmit(mesh, 0, program, mtx);

		bgfx::frame();
	}

public:
	int64_t timeOffset;

	SDL_Window* window;

	bgfx::ProgramHandle program;
	Mesh* mesh;
};

int _main_(int , char**) {
	Model model;
	model.init();

	SDL_Event sdlevent{};
	while (!quit)
	{
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency());
		const float deltaTimeSec = float(double(frameTime) / freq);
		camera.update(deltaTimeSec);

		model.run(camera);
		process_event(sdlevent);
	}

	SDL_Quit();

	return 0;
}

void process_event(SDL_Event& event) {
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) {
			quit = true;
		}
		else if (event.type == SDL_MOUSEWHEEL) {
			camera.dolly(event.wheel.y * 0.05f);
		}
	}

	camera.mouse.update(float(mouse_x), float(mouse_y), WIDTH, HEIGHT);
	if (SDL_GetMouseState(&mouse_x, &mouse_y)) {
		camera.orbit(camera.mouse.m_dx, camera.mouse.m_dy);
	}
}