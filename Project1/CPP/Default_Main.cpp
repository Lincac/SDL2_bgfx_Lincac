#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_syswm.h>
#include<bx/timer.h>
#include<../Head/LoadShader.h>
#include<../Head/Camera.h>
#include<../Head/Cube.h>
#include<../Head/LoadTexture.h>
using namespace std;

void process_event(SDL_Event& event);

const int WIDTH = 800;
const int HEIGHT = 600;
int mouse_x = 0;
int mouse_y = 0;
bool quit = false;
int64_t m_timeOffset = bx::getHPCounter();

Camera camera({0.0,0.0,-35.0});

int main(int,char**) {
	SDL_Init(SDL_INIT_EVENTS);
	SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	SDL_SysWMinfo wmi{};
	SDL_VERSION(&wmi.version);
	SDL_GetWindowWMInfo(window, &wmi);

	// bgfx��Դ��ʼ��
	bgfx::Init init;
	init.type = bgfx::RendererType::OpenGL; // type ��ȡֵ��Χ�� bgfx::RendererType ���ͣ�����ָ����Ⱦ���ĺ��
	init.vendorId = bgfx::getCaps()->vendorId;  // ����ָ����Ⱦ���ĺ��ʹ�õ��Կ����̵ı�ʶ��
	init.platformData.nwh = wmi.info.win.window; // ��ǰ����
	init.platformData.ndt = wmi.info.win.hinstance; // ��ǰ���ھ��
	init.resolution.width = WIDTH;
	init.resolution.height = HEIGHT;
	init.resolution.reset = BGFX_RESET_VSYNC;//��ֱͬ����һ�ּ���������������Ļ˺�����󣬼�����Ⱦ֡�ʸ�����ʾ��ˢ����ʱ����Ļ�ϳ�������������ͬ֡�Ļ���ƴ����һ��
	bgfx::init(init);

	bgfx::setDebug(BGFX_DEBUG_NONE); 

	// ��ʼ����������,����������
	Cube::PosNormalTangentTexcoordVertex::init(); 
	bgfx::VertexBufferHandle vertex = bgfx::createVertexBuffer(bgfx::makeRef(Cube::s_cubeVertices, sizeof(Cube::s_cubeVertices)), Cube::PosNormalTangentTexcoordVertex::ms_layout
	); // bgfx::makeRef������ת��Ϊbgfx����ʹ�õ����ݣ�ֱ�ӽ����ڴ渴��
	bgfx::IndexBufferHandle index = bgfx::createIndexBuffer(bgfx::makeRef(Cube::s_cubeTriList, sizeof(Cube::s_cubeTriList)));

	// ������ɫ������
	bgfx::ShaderHandle vshader = Shader::loadshader("shader_compile/vs_cube.bin");
	bgfx::ShaderHandle fshader = Shader::loadshader("shader_compile/fs_cube.bin");
	bgfx::ProgramHandle program = bgfx::createProgram(vshader,fshader,true  /* destroy shaders when program is destroyed */);

	// ��������
	bgfx::TextureHandle texture = Texture::loadTexture("resource/textures_compile/aerial_rocks_04_diff_2k.dds");
	bgfx::UniformHandle uniform_texture = bgfx::createUniform("texColor", bgfx::UniformType::Sampler);

	SDL_Event sdlevent;
	while (!quit)
	{
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency());
		const float deltaTimeSec = float(double(frameTime) / freq);
		float time = (float)((now - m_timeOffset) / double(bx::getHPFrequency()));
		camera.update(deltaTimeSec);

		// ���û��������submit���ã��ͻ����submit��0����
		bgfx::touch(0);

		float view[16];
		float proj[16];
		camera.mtxLookAt(view);
		bx::mtxProj(proj, 60.0, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT)); 
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewTransform(0, view, proj);

		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				float mtx[16];
				bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
				mtx[12] = -15.0f + float(xx) * 3.0f;
				mtx[13] = -15.0f + float(yy) * 3.0f;
				mtx[14] = 0.0f;
				bgfx::setTransform(mtx);

				bgfx::setVertexBuffer(0, vertex);
				bgfx::setIndexBuffer(index);

				bgfx::setTexture(0, uniform_texture, texture);

				bgfx::setState(0
					| BGFX_STATE_WRITE_RGB 
					| BGFX_STATE_WRITE_A 
					| BGFX_STATE_WRITE_Z 
					| BGFX_STATE_DEPTH_TEST_LESS 
					| BGFX_STATE_MSAA 
				);

				bgfx::submit(0, program);
			}
		}

		bgfx::frame();

		process_event(sdlevent);
	}

	bgfx::destroy(vertex);
	bgfx::destroy(index);
	bgfx::destroy(program);
	bgfx::destroy(texture);
	bgfx::destroy(uniform_texture);
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
