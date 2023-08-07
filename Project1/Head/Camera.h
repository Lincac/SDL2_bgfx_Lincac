#pragma once

#include<bx/bx.h>
#include <bx/math.h>

struct Mouse
{
	Mouse()
	{
		m_dx = 0.0f;
		m_dy = 0.0f;
		m_prevMx = 0.0f;
		m_prevMx = 0.0f;
	}

	void update(float _mx, float _my, uint32_t _width, uint32_t _height)
	{
		const float widthf = float(int32_t(_width));
		const float heightf = float(int32_t(_height));

		// Delta movement.
		m_dx = float(_mx - m_prevMx) / widthf;
		m_dy = float(_my - m_prevMy) / heightf;

		m_prevMx = _mx;
		m_prevMy = _my;
	}

	float m_dx; // Screen space.
	float m_dy;
	float m_prevMx;
	float m_prevMy;
};

class Camera
{
public:
	Camera() { reset(); }

	void reset()
	{
		m_target.curr = bx::InitZero;
		m_target.dest = bx::InitZero;

		m_pos.curr = { 0.0f, 0.0f, -3.0f };
		m_pos.dest = { 0.0f, 0.0f, -3.0f };

		m_orbit[0] = 0.0f;
		m_orbit[1] = 0.0f;
	}

	void mtxLookAt(float* _outViewMtx)
	{
		bx::mtxLookAt(_outViewMtx, m_pos.curr, m_target.curr);
	}

	void orbit(float _dx, float _dy)
	{
		m_orbit[0] += _dx;
		m_orbit[1] += _dy;
	}

	void dolly(float _dz)
	{
		const float cnear = 1.0f;
		const float cfar = 100.0f;

		const bx::Vec3 toTarget = bx::sub(m_target.dest, m_pos.dest);
		const float toTargetLen = bx::length(toTarget);
		const float invToTargetLen = 1.0f / (toTargetLen + bx::kFloatSmallest);
		const bx::Vec3 toTargetNorm = bx::mul(toTarget, invToTargetLen);

		float delta = toTargetLen * _dz;
		float newLen = toTargetLen + delta;
		if ((cnear < newLen || _dz < 0.0f)
			&& (newLen < cfar || _dz > 0.0f))
		{
			m_pos.dest = bx::mad(toTargetNorm, delta, m_pos.dest);
		}
	}

	void consumeOrbit(float _amount)
	{
		float consume[2] = {0.0};
		consume[0] = m_orbit[0] * _amount;
		consume[1] = m_orbit[1] * _amount;
		m_orbit[0] -= consume[0];
		m_orbit[1] -= consume[1];

		const bx::Vec3 toPos = bx::sub(m_pos.curr, m_target.curr);
		const float toPosLen = bx::length(toPos);
		const float invToPosLen = 1.0f / (toPosLen + bx::kFloatSmallest);
		const bx::Vec3 toPosNorm = bx::mul(toPos, invToPosLen);

		float ll[2] = {0.0};
		bx::toLatLong(&ll[0], &ll[1], toPosNorm);
		ll[0] += consume[0];
		ll[1] -= consume[1];
		ll[1] = bx::clamp(ll[1], 0.02f, 0.98f);

		const bx::Vec3 tmp = bx::fromLatLong(ll[0], ll[1]);
		const bx::Vec3 diff = bx::mul(bx::sub(tmp, toPosNorm), toPosLen);

		m_pos.curr = bx::add(m_pos.curr, diff);
		m_pos.dest = bx::add(m_pos.dest, diff);
	}

	void update(float _dt)
	{
		const float amount = bx::min(_dt / 0.12f, 1.0f);

		consumeOrbit(amount);

		m_target.curr = bx::lerp(m_target.curr, m_target.dest, amount);
		m_pos.curr = bx::lerp(m_pos.curr, m_pos.dest, amount);
	}

	void envViewMtx(float* _mtx)
	{
		const bx::Vec3 toTarget = bx::sub(m_target.curr, m_pos.curr);
		const float toTargetLen = bx::length(toTarget);
		const float invToTargetLen = 1.0f / (toTargetLen + bx::kFloatSmallest);
		const bx::Vec3 toTargetNorm = bx::mul(toTarget, invToTargetLen);

		const bx::Vec3 right = bx::normalize(bx::cross({ 0.0f, 1.0f, 0.0f }, toTargetNorm));
		const bx::Vec3 up = bx::normalize(bx::cross(toTargetNorm, right));

		_mtx[0] = right.x;
		_mtx[1] = right.y;
		_mtx[2] = right.z;
		_mtx[3] = 0.0f;
		_mtx[4] = up.x;
		_mtx[5] = up.y;
		_mtx[6] = up.z;
		_mtx[7] = 0.0f;
		_mtx[8] = toTargetNorm.x;
		_mtx[9] = toTargetNorm.y;
		_mtx[10] = toTargetNorm.z;
		_mtx[11] = 0.0f;
		_mtx[12] = 0.0f;
		_mtx[13] = 0.0f;
		_mtx[14] = 0.0f;
		_mtx[15] = 1.0f;
	}

	struct Interp3f
	{
		bx::Vec3 curr = bx::InitNone;
		bx::Vec3 dest = bx::InitNone;
	};

	Interp3f m_target;
	Interp3f m_pos;
	float m_orbit[2];
	Mouse mouse;
};
