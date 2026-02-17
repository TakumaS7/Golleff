#pragma once

#include "renderer.h"

class AABB; //前方宣言
class Point;
class Sphere;

class Collision //コリジョンインターフェース
{
public:
	virtual bool isOverlap(const AABB& aabb) const = 0;
	virtual bool isOverlap(const Point& point) const = 0;
	virtual bool isOverlap(const Sphere& sphere) const = 0;
};

class AABB : public Collision
{
private:
	XMFLOAT3 m_min = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_max = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_center = { 0.0f, 0.0f, 0.0f };

public:
	AABB() = default;
	AABB(const XMFLOAT3& min, const XMFLOAT3& max);

	const XMFLOAT3& GetMin() const
	{
		return m_min;
	}

	const XMFLOAT3& GetMax() const
	{
		return m_max;
	}

	const XMFLOAT3& GetCenter() const
	{
		return m_center;
	}

	AABB Translation(const XMVECTOR& move) const;

	bool isOverlap(const AABB& aabb) const override;
	bool isOverlap(const Point& point) const override;
	bool isOverlap(const Sphere& sphere) const override { return false; }
};

class Point : public Collision
{
private:
	XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };

public:
	Point() = default;
	Point(const XMFLOAT3& position) : m_Position(position) {};

	void SetPosition(const XMFLOAT3& position)
	{
		m_Position = position;
	}

	const XMFLOAT3& GetPosition() const
	{
		return m_Position;
	}

	bool isOverlap(const AABB& aabb) const override
	{
		return aabb.isOverlap(*this); //*this：自分自身
	}

	bool isOverlap(const Point& point) const override
	{
		return false;
	}

	bool isOverlap(const Sphere& sphere) const override
	{
		return false;
	}
};

class Sphere : public Collision
{
private:
	XMFLOAT3 m_Center = { 0.0f, 0.0f, 0.0f };
	float m_Radius = 1.0f;

public:
	Sphere() = default;
	Sphere(const XMFLOAT3& center, float radius)
		: m_Center(center), m_Radius(radius) {};

	Sphere Translation(const XMVECTOR& move) const;

	bool isOverlap(const AABB& aabb) const override { return false; }
	bool isOverlap(const Point& point) const override;
	bool isOverlap(const Sphere& sphere) const override { return false; }
};
