#pragma once

#include "main.h"

class Camera
{
private:
	XMMATRIX m_MtxView;
	XMMATRIX m_MtxPerspective;
	XMVECTOR m_Position;

protected:
	void SetViewMatrix(const XMMATRIX& mtx);
	void SetPerspectiveMatrix(const XMMATRIX& mtx);
	void SetPosition(const XMVECTOR& position) { m_Position = position; }

public:
	Camera() : m_Position({ 5.0f, 5.0f, -5.0f }) {}

	virtual void Update();

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetPerspectiveMatrix() const;
	const XMMATRIX GetCameraMatrix() const;
	const XMVECTOR& GetPosition() const { return m_Position; }
};