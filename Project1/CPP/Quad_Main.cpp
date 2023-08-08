#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_syswm.h>
#include<bx/timer.h>
#include<bx/bx.h>
#include<../Head/LoadShader.h>
#include<../Head/Quad.h>
#include<../Head/Cube.h>
#include<../Head/LoadTexture.h>
#include<../Head/Camera.h>
using namespace std;

void process_event(SDL_Event& event);

const int WIDTH = 800;
const int HEIGHT = 600;
int mouse_x = 0;
int mouse_y = 0;
bool quit = false;
int64_t m_timeOffset = bx::getHPCounter();

Camera camera({ 0.0,0.0,-35.0 });

int main(int, char**) {
	SDL_Init(SDL_INIT_EVENTS);
	SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	SDL_SysWMinfo wmi{};
	SDL_VERSION(&wmi.version);
	SDL_GetWindowWMInfo(window, &wmi);

#pragma region bgfx_init
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
#pragma endregion
	
#pragma region quad_vertex_init
	Quad::PosTexCoord0Vertex::init();
	bgfx::VertexBufferHandle quad_vertex = bgfx::createVertexBuffer(bgfx::makeRef(Quad::s_quadVertices, sizeof(Quad::s_quadVertices)), Quad::PosTexCoord0Vertex::ms_layout);
	bgfx::IndexBufferHandle quad_index = bgfx::createIndexBuffer(bgfx::makeRef(Quad::s_quadTriList, sizeof(Quad::s_quadTriList)));
#pragma endregion

#pragma region quad_shader_init
	bgfx::ShaderHandle quad_vshader = Shader::loadshader("shader_compile/vs_FBO.bin");
	bgfx::ShaderHandle quad_fshader = Shader::loadshader("shader_compile/fs_FBO.bin");
	bgfx::ProgramHandle quad_program = bgfx::createProgram(quad_vshader, quad_fshader, true);
#pragma endregion

#pragma region cube_vertex_init
	Cube::PosNormalTangentTexcoordVertex::init();
	bgfx::VertexBufferHandle cube_vertex = bgfx::createVertexBuffer(bgfx::makeRef(Cube::s_cubeVertices, sizeof(Cube::s_cubeVertices)), Cube::PosNormalTangentTexcoordVertex::ms_layout); 
	bgfx::IndexBufferHandle cube_index = bgfx::createIndexBuffer(bgfx::makeRef(Cube::s_cubeTriList, sizeof(Cube::s_cubeTriList)));
#pragma endregion

#pragma region cube_shader_init
	bgfx::ShaderHandle cube_vshader = Shader::loadshader("shader_compile/vs_cube.bin");
	bgfx::ShaderHandle cube_fshader = Shader::loadshader("shader_compile/fs_cube.bin");
	bgfx::ProgramHandle cube_program = bgfx::createProgram(cube_vshader, cube_fshader, true  );
#pragma endregion

#pragma region  texture_init
	bgfx::TextureHandle cube_texture = Texture::loadTexture("resource/textures_compile/aerial_rocks_04_diff_2k.dds");
	bgfx::UniformHandle cube_uniform = bgfx::createUniform("texColor", bgfx::UniformType::Sampler);

	bgfx::UniformHandle main_uniform = bgfx::createUniform("_MainTex", bgfx::UniformType::Sampler);
#pragma endregion

#pragma region fbo_init
	bgfx::ViewId id = 1;

	const uint64_t Flags = 0
		| BGFX_SAMPLER_MIN_POINT
		| BGFX_SAMPLER_MAG_POINT
		| BGFX_SAMPLER_U_CLAMP
		| BGFX_SAMPLER_V_CLAMP
		;

	bgfx::TextureHandle color_texture = bgfx::createTexture2D(uint16_t(WIDTH), uint16_t(HEIGHT), false, 1, bgfx::TextureFormat::RGBA16F , BGFX_TEXTURE_RT | Flags,NULL);
	bgfx::TextureHandle depth_texture = bgfx::createTexture2D(uint16_t(WIDTH), uint16_t(HEIGHT), false, 1, bgfx::TextureFormat::D24F, BGFX_TEXTURE_RT | Flags,NULL);

	bgfx::Attachment attachments[2];
	attachments[0].init(color_texture);
	attachments[1].init(depth_texture);

	bgfx::FrameBufferHandle fbo = bgfx::createFrameBuffer(BX_COUNTOF(attachments), attachments, true);
#pragma endregion

	SDL_Event sdlevent;
	while (!quit)
	{
#pragma region camera_update
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency());
		const float deltaTimeSec = float(double(frameTime) / freq);
		float time = (float)((now - m_timeOffset) / double(bx::getHPFrequency()));
		camera.update(deltaTimeSec);
#pragma endregion
		
		bgfx::touch(0);

#pragma region draw_cube
		float view[16];
		float proj[16];
		camera.mtxLookAt(view);
		bx::mtxProj(proj, 60.0, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewFrameBuffer(id, fbo);
		bgfx::setViewRect(id, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));
		bgfx::setViewClear(id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewTransform(id, view, proj);

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

				bgfx::setVertexBuffer(0, cube_vertex);
				bgfx::setIndexBuffer(cube_index);

				bgfx::setTexture(0, cube_uniform, cube_texture);

				bgfx::setState(0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LESS
					| BGFX_STATE_MSAA
				);

				bgfx::submit(id, cube_program);
			}
		}
#pragma endregion

#pragma region draw_quad
		float ortho[16];
		bx::mtxOrtho(ortho, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 100.0f, 0.0, bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
		bgfx::setViewTransform(0, NULL, ortho);

		bgfx::setVertexBuffer(0, quad_vertex);
		bgfx::setIndexBuffer(quad_index);

		bgfx::setTexture(0, main_uniform, bgfx::getTexture(fbo,0));

		bgfx::setState(0
			| BGFX_STATE_WRITE_RGB 
			| BGFX_STATE_WRITE_A 
			| BGFX_STATE_DEPTH_TEST_ALWAYS
		);

		bgfx::submit(0, quad_program);
#pragma endregion

		bgfx::frame();

		process_event(sdlevent);
	}

	bgfx::shutdown();

	SDL_DestroyWindow(window);
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
