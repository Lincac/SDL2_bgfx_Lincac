#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_syswm.h>
#include<bx/timer.h>
#include<../Head/LoadShader.h>
#include<../Head/Camera.h>
using namespace std;

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;
	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};
	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

void process_event(SDL_Event& event);

const int WIDTH = 800;
const int HEIGHT = 600;
int mouse_x = 0;
int mouse_y = 0;
bool quit = false;

Camera camera;

int main(int,char**) {
	SDL_Init(SDL_INIT_EVENTS);
	SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	SDL_SysWMinfo wmi{};
	SDL_VERSION(&wmi.version);
	SDL_GetWindowWMInfo(window, &wmi);

	// bgfx资源初始化
	bgfx::Init init;
	init.type = bgfx::RendererType::OpenGL; // type 的取值范围是 bgfx::RendererType 类型，用来指定渲染器的后端
	init.vendorId = bgfx::getCaps()->vendorId;  // 用来指定渲染器的后端使用的显卡厂商的标识符
	init.platformData.nwh = wmi.info.win.window; // 当前窗口
	init.platformData.ndt = wmi.info.win.hinstance; // 当前窗口句柄
	init.resolution.width = WIDTH;
	init.resolution.height = HEIGHT;
	init.resolution.reset = BGFX_RESET_VSYNC;//垂直同步是一种技术，用来避免屏幕撕裂现象，即当渲染帧率高于显示器刷新率时，屏幕上出现两个或多个不同帧的画面拼接在一起
	bgfx::init(init);

	bgfx::setDebug(BGFX_DEBUG_NONE); 

	// 初始化顶点数据,创建缓冲区
	PosColorVertex::init(); 
	bgfx::VertexBufferHandle vertex = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout
	); // bgfx::makeRef将数据转化为bgfx可以使用的数据，直接进行内存复制
	bgfx::IndexBufferHandle index = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

	// 创建着色器程序
	bgfx::ShaderHandle vshader = Shader::loadshader("shader_compile/vs_cube.bin");
	bgfx::ShaderHandle fshader = Shader::loadshader("shader_compile/fs_cube.bin");
	bgfx::ProgramHandle program = bgfx::createProgram(vshader,fshader,true  /* destroy shaders when program is destroyed */);

	SDL_Event sdlevent;
	while (!quit)
	{
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency());
		const float deltaTimeSec = float(double(frameTime) / freq);
		camera.update(deltaTimeSec);

		bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		// 上面OpenGL差不多，glclearcolor和glclear

		bgfx::touch(0); 

		float view[16];
		float proj[16];
		camera.mtxLookAt(view);
		bx::mtxProj(proj, 45.0f, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth /*线性深度*/);
		bgfx::setViewTransform(0, view, proj);

		bgfx::setVertexBuffer(0, vertex);
		bgfx::setIndexBuffer(index);

		bgfx::setState(0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
		);

		bgfx::submit(0, program);

		bgfx::frame();

		process_event(sdlevent);
	}

	bgfx::destroy(vertex);
	bgfx::destroy(index);
	bgfx::destroy(program);
	bgfx::shutdown();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void process_event(SDL_Event& event) {
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT){
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
