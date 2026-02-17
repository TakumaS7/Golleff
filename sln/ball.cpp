#include "ball.h"
#include "light.h"
#include "texture.h"
#include "Audio.h"

void Ball::ModelLoad()
{
	//g = TextureLoad()
	m_pModel = ::ModelLoad("asset/model/ball.fbx", false); //ModelLoadがかぶっているため、::を付ける

	m_AudioId = LoadAudio("asset\\Audio\\Golouleff_Ball_SE.wav");
}

void Ball::Update()
{
	m_Acceleration += {0.0f, -0.001f, 0.0f}; //重力加速度
	m_Position += m_Acceleration;

	if (m_isOverlap)
	{
		m_Acceleration = { 0.0f, 0.0f, 0.0f };
	}

	m_isOverlap = false;
}

void Ball::Draw() const
{
	XMMATRIX world = XMMatrixTranslationFromVector(m_Position);
	SetWorldMatrix(world);
	SetLight(world);
	//SetTexture(g);
	ModelDraw(m_pModel);
}

void Ball::OnHit(const AABB& aabb)
{
	if (GetAABB().isOverlap(aabb))
	{
		//cubeのAABBの中心座標からボールのAABB中心座標が各軸から一番短いのはどれ？
		XMVECTOR cube_to_ball = XMLoadFloat3(&GetAABB().GetCenter()) - XMLoadFloat3(&aabb.GetCenter());

		float x = XMVectorGetX(cube_to_ball);
		float y = XMVectorGetY(cube_to_ball);
		float z = XMVectorGetZ(cube_to_ball);

		float ax = fabsf(x);
		float ay = fabsf(y);
		float az = fabsf(z);

		bool is_max_length_x = false;
		bool is_max_length_y = false;
		bool is_max_length_z = false;

		XMFLOAT3 hit_face = { 0.0f, 0.0f, 0.0f };

		if (ax > ay)
		{
			if (ax > az)
			{
				is_max_length_x = true;
			}
			else
			{
				is_max_length_z = true;
			}
		}
		else
		{
			if (ay > az)
			{
				is_max_length_y = true;
			}
			else
			{
				is_max_length_z = true;
			}
		}

		//めり込みを（簡易的に）直す
		float m = 0.0f;
		if (is_max_length_x)
		{
			if (x > 0)
			{
				//左から
				m = aabb.GetMax().x - (GetAABB().GetMin().x - XMVectorGetX(GetPosition()));
			}
			else
			{
				//右から
				m = aabb.GetMin().x - (GetAABB().GetMax().x - XMVectorGetX(GetPosition()));
			}

			SetPosition(XMVectorSetX(GetPosition(), m));
			m_Acceleration = XMVectorSetX(m_Acceleration, -XMVectorGetX(m_Acceleration));
		}
		else if (is_max_length_y)
		{
			if (y > 0)
			{
				//上から
				m = aabb.GetMax().y - (GetAABB().GetMin().y - XMVectorGetY(GetPosition()));
			}
			else
			{
				//下から
				m = aabb.GetMin().y - (GetAABB().GetMax().y - XMVectorGetY(GetPosition()));
			}

			SetPosition(XMVectorSetY(GetPosition(), m));
			m_Acceleration = XMVectorSetY(m_Acceleration, -XMVectorGetY(m_Acceleration));
		}
		else
		{
			if (z > 0)
			{
				//前から
				m = aabb.GetMax().z - (GetAABB().GetMin().z - XMVectorGetZ(GetPosition()));
			}
			else
			{
				//後ろから
				m = aabb.GetMin().z - (GetAABB().GetMax().z - XMVectorGetZ(GetPosition()));
			}

			SetPosition(XMVectorSetZ(GetPosition(), m));
			m_Acceleration = XMVectorSetZ(m_Acceleration, -XMVectorGetZ(m_Acceleration));
		}

		m_Acceleration *= 0.5f;

		PlayAudio(m_AudioId, false); // オーディオの再生　trueはループする
	}
}
