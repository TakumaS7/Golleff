#pragma once

#include "collision.h"
#include "model.h"

class Ball
{
private:
	XMVECTOR m_Position;
	XMVECTOR m_SavePos = { 3.5f, 5.0f, -4.0f };
	AABB m_Aabb;
	XMVECTOR m_Acceleration = { 0.0f, 0.0f, 0.0f };
	MODEL* m_pModel = nullptr;
	bool m_isOverlap = false;
	int m_AudioId = -1;

public:
	Ball() = default;
	Ball(const XMVECTOR& position) : m_Position(position) {};
	~Ball()
	{
		ModelRelease();
	}

	void ModelLoad();
	void ModelRelease()
	{
		if (m_pModel)
		{
			::ModelRelease(m_pModel);
			m_pModel = nullptr;
		}
	}

	void Update();
	void Draw() const;

	bool IsStopped() const
	{
		return XMVectorGetY(XMVector3Length(m_Acceleration)) < 0.001f;
	}

	void AddForce(const XMVECTOR& force)
	{
		m_Acceleration += force;
	}

	XMVECTOR GetPosition() const
	{
		return m_Position;
	}

	void SetPosition(const XMVECTOR& position)
	{
		m_Position = position;
	}

	AABB GetAABB() const 
	{ 
		return m_pModel->aabb.Translation(m_Position); 
	}

	void OnOverlap() { m_isOverlap = true; }

	void OnHit(const AABB& aabb);

	void SavePosition(XMVECTOR pos) {
		m_SavePos = pos; 
		m_SavePos = XMVectorSetY(m_SavePos, 5.0f);
	}

	XMVECTOR GetSavePosition() { return m_SavePos; }
};