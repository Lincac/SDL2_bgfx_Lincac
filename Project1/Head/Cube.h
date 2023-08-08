#pragma once

#include<bgfx/bgfx.h>
#include <bx/pixelformat.h>

inline uint32_t encodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
{
	const float src[] =
	{
		_x * 0.5f + 0.5f,
		_y * 0.5f + 0.5f,
		_z * 0.5f + 0.5f,
		_w * 0.5f + 0.5f,
	};
	uint32_t dst;
	bx::packRgba8(&dst, src);
	return dst;
}

namespace Cube {
	struct PosNormalTangentTexcoordVertex
	{
		float m_x;
		float m_y;
		float m_z;
		uint32_t m_normal;
		uint32_t m_tangent;
		int16_t m_u;
		int16_t m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
				.add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
				.end();
		}
		static bgfx::VertexLayout ms_layout;
	};
	bgfx::VertexLayout PosNormalTangentTexcoordVertex::ms_layout;

	static PosNormalTangentTexcoordVertex s_cubeVertices[] =
	{
	{-1.0f,  1.0f,  1.0f, encodeNormalRgba8(0.0f,  0.0f,  1.0f), 0,      0,      0 },
	{ 1.0f,  1.0f,  1.0f, encodeNormalRgba8(0.0f,  0.0f,  1.0f), 0, 0x7fff,      0 },
	{-1.0f, -1.0f,  1.0f, encodeNormalRgba8(0.0f,  0.0f,  1.0f), 0,      0, 0x7fff },
	{ 1.0f, -1.0f,  1.0f, encodeNormalRgba8(0.0f,  0.0f,  1.0f), 0, 0x7fff, 0x7fff },
	{-1.0f,  1.0f, -1.0f, encodeNormalRgba8(0.0f,  0.0f, -1.0f), 0,      0,      0 },
	{ 1.0f,  1.0f, -1.0f, encodeNormalRgba8(0.0f,  0.0f, -1.0f), 0, 0x7fff,      0 },
	{-1.0f, -1.0f, -1.0f, encodeNormalRgba8(0.0f,  0.0f, -1.0f), 0,      0, 0x7fff },
	{ 1.0f, -1.0f, -1.0f, encodeNormalRgba8(0.0f,  0.0f, -1.0f), 0, 0x7fff, 0x7fff },
	{-1.0f,  1.0f,  1.0f, encodeNormalRgba8(0.0f,  1.0f,  0.0f), 0,      0,      0 },
	{ 1.0f,  1.0f,  1.0f, encodeNormalRgba8(0.0f,  1.0f,  0.0f), 0, 0x7fff,      0 },
	{-1.0f,  1.0f, -1.0f, encodeNormalRgba8(0.0f,  1.0f,  0.0f), 0,      0, 0x7fff },
	{ 1.0f,  1.0f, -1.0f, encodeNormalRgba8(0.0f,  1.0f,  0.0f), 0, 0x7fff, 0x7fff },
	{-1.0f, -1.0f,  1.0f, encodeNormalRgba8(0.0f, -1.0f,  0.0f), 0,      0,      0 },
	{ 1.0f, -1.0f,  1.0f, encodeNormalRgba8(0.0f, -1.0f,  0.0f), 0, 0x7fff,      0 },
	{-1.0f, -1.0f, -1.0f, encodeNormalRgba8(0.0f, -1.0f,  0.0f), 0,      0, 0x7fff },
	{ 1.0f, -1.0f, -1.0f, encodeNormalRgba8(0.0f, -1.0f,  0.0f), 0, 0x7fff, 0x7fff },
	{ 1.0f, -1.0f,  1.0f, encodeNormalRgba8(1.0f,  0.0f,  0.0f), 0,      0,      0 },
	{ 1.0f,  1.0f,  1.0f, encodeNormalRgba8(1.0f,  0.0f,  0.0f), 0, 0x7fff,      0 },
	{ 1.0f, -1.0f, -1.0f, encodeNormalRgba8(1.0f,  0.0f,  0.0f), 0,      0, 0x7fff },
	{ 1.0f,  1.0f, -1.0f, encodeNormalRgba8(1.0f,  0.0f,  0.0f), 0, 0x7fff, 0x7fff },
	{-1.0f, -1.0f,  1.0f, encodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0,      0,      0 },
	{-1.0f,  1.0f,  1.0f, encodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0, 0x7fff,      0 },
	{-1.0f, -1.0f, -1.0f, encodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0,      0, 0x7fff },
	{-1.0f,  1.0f, -1.0f, encodeNormalRgba8(-1.0f,  0.0f,  0.0f), 0, 0x7fff, 0x7fff },
	};

	static const uint16_t s_cubeTriList[] =
	{
		 0,  2,  1,
		 1,  2,  3,
		 4,  5,  6,
		 5,  7,  6,

		 8, 10,  9,
		 9, 10, 11,
		12, 13, 14,
		13, 15, 14,

		16, 18, 17,
		17, 18, 19,
		20, 21, 22,
		21, 23, 22,
	};


	void calcTangents(void* _vertices, uint16_t _numVertices, bgfx::VertexLayout _layout, const uint16_t* _indices, uint32_t _numIndices) {
		struct PosTexcoord
		{
			float m_x;
			float m_y;
			float m_z;
			float m_pad0;
			float m_u;
			float m_v;
			float m_pad1;
			float m_pad2;
		};

		float* tangents = new float[6 * _numVertices];
		bx::memSet(tangents, 0, 6 * _numVertices * sizeof(float));

		PosTexcoord v0;
		PosTexcoord v1;
		PosTexcoord v2;

		for (uint32_t ii = 0, num = _numIndices / 3; ii < num; ++ii)
		{
			const uint16_t* indices = &_indices[ii * 3];
			uint32_t i0 = indices[0];
			uint32_t i1 = indices[1];
			uint32_t i2 = indices[2];

			bgfx::vertexUnpack(&v0.m_x, bgfx::Attrib::Position, _layout, _vertices, i0);
			bgfx::vertexUnpack(&v0.m_u, bgfx::Attrib::TexCoord0, _layout, _vertices, i0);

			bgfx::vertexUnpack(&v1.m_x, bgfx::Attrib::Position, _layout, _vertices, i1);
			bgfx::vertexUnpack(&v1.m_u, bgfx::Attrib::TexCoord0, _layout, _vertices, i1);

			bgfx::vertexUnpack(&v2.m_x, bgfx::Attrib::Position, _layout, _vertices, i2);
			bgfx::vertexUnpack(&v2.m_u, bgfx::Attrib::TexCoord0, _layout, _vertices, i2);

			const float bax = v1.m_x - v0.m_x;
			const float bay = v1.m_y - v0.m_y;
			const float baz = v1.m_z - v0.m_z;
			const float bau = v1.m_u - v0.m_u;
			const float bav = v1.m_v - v0.m_v;

			const float cax = v2.m_x - v0.m_x;
			const float cay = v2.m_y - v0.m_y;
			const float caz = v2.m_z - v0.m_z;
			const float cau = v2.m_u - v0.m_u;
			const float cav = v2.m_v - v0.m_v;

			const float det = (bau * cav - bav * cau);
			const float invDet = 1.0f / det;

			const float tx = (bax * cav - cax * bav) * invDet;
			const float ty = (bay * cav - cay * bav) * invDet;
			const float tz = (baz * cav - caz * bav) * invDet;

			const float bx = (cax * bau - bax * cau) * invDet;
			const float by = (cay * bau - bay * cau) * invDet;
			const float bz = (caz * bau - baz * cau) * invDet;

			for (uint32_t jj = 0; jj < 3; ++jj)
			{
				float* tanu = &tangents[indices[jj] * 6];
				float* tanv = &tanu[3];
				tanu[0] += tx;
				tanu[1] += ty;
				tanu[2] += tz;

				tanv[0] += bx;
				tanv[1] += by;
				tanv[2] += bz;
			}
		}

		for (uint32_t ii = 0; ii < _numVertices; ++ii)
		{
			const bx::Vec3 tanu = bx::load<bx::Vec3>(&tangents[ii * 6]);
			const bx::Vec3 tanv = bx::load<bx::Vec3>(&tangents[ii * 6 + 3]);

			float nxyzw[4];
			bgfx::vertexUnpack(nxyzw, bgfx::Attrib::Normal, _layout, _vertices, ii);

			const bx::Vec3 normal = bx::load<bx::Vec3>(nxyzw);
			const float    ndt = bx::dot(normal, tanu);
			const bx::Vec3 nxt = bx::cross(normal, tanu);
			const bx::Vec3 tmp = bx::sub(tanu, bx::mul(normal, ndt));

			float tangent[4];
			bx::store(tangent, bx::normalize(tmp));
			tangent[3] = bx::dot(nxt, tanv) < 0.0f ? -1.0f : 1.0f;

			bgfx::vertexPack(tangent, true, bgfx::Attrib::Tangent, _layout, _vertices, ii);
		}

		delete[] tangents;
	}
}