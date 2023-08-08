#pragma once

#include<bgfx/bgfx.h>

namespace Quad {


	struct PosTexCoord0Vertex
	{
		float m_x;
		float m_y;
		float m_z;
		float m_u;
		float m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		}

		static bgfx::VertexLayout ms_layout;
	};
	bgfx::VertexLayout PosTexCoord0Vertex::ms_layout;

	static PosTexCoord0Vertex s_quadVertices[] = {
			-1.0, -1.0, 0.0, -1.0, -1.0,
			1.0, -1.0, 0.0, 1.0, -1.0,
			1.0, 1.0, 0.0, 1.0, 1.0,
			-1.0, 1.0, 0.0, -1.0, 1.0,
	};

	static const uint16_t s_quadTriList[] = {
		0,2,1,
		0,3,2
	};
}