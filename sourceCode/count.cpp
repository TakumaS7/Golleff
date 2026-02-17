#include "count.h"
#include "renderer.h"
#include "sprite.h"
#include "Audio.h"

void Count::InitCount()
{
	m_RankPattern = 0;

	m_TextureId[0] = TextureLoad(L"asset/texture/numbers.png");
	m_TextureId[1] = TextureLoad(L"asset/texture/dasuu.png");
	m_TextureId[2] = TextureLoad(L"asset/texture/Rank.png");

	m_AudioId = LoadAudio("asset\\Audio\\Golouleff_RankDown_SE.wav"); // オーディオの読み込み
}

void Count::UninitCount()
{
	UnloadAudio(m_AudioId);
}

void Count::DrawCount(float x, float y)
{
	ChangeRank(); // ランクを入れ替える

	SetUVMatrix(XMMatrixIdentity());

	//2D描画なので奥行処理をOFF
	SetDepthEnable(false);

	SetPixelShader2d();

	SetTexture(m_TextureId[0]);

	DrawSpriteRotateUV({ x + m_Size.x * 0.75f, y, 0.0f }, m_Size, m_Color, 0.0f, m_Count % 10, 10, 1); // 1
	DrawSpriteRotateUV({ x, y, 0.0f }, m_Size, m_Color, 0.0f, m_Count % 100 / 10, 10, 1); // 10
	DrawSpriteRotateUV({ x + -m_Size.x * 0.75f, y, 0.0f }, m_Size, m_Color, 0.0f, m_Count % 1000 / 100, 10, 1); // 100

	SetTexture(m_TextureId[1]);
	DrawSprite({ x - m_Size.x * 0.5f, y - m_Size.y * 0.75f, 0.0f }, m_Size, m_Color);

	SetTexture(m_TextureId[2]);
	DrawSpriteRotateUV({ x + m_Size.x * 0.75f, y - m_Size.y * 0.75f, 0.0f }, m_RankSize, m_Color, 0.0f, m_RankPattern, 5, 1);
}

void Count::DrawOldCount(float x, float y)
{
	ChangeOldRank(); // ランクを入れ替える

	SetUVMatrix(XMMatrixIdentity());

	//2D描画なので奥行処理をOFF
	SetDepthEnable(false);

	SetPixelShader2d();

	SetTexture(m_TextureId[0]);

	DrawSpriteRotateUV({ x + m_Size.x * 0.75f, y, 0.0f }, m_Size, m_Color, 0.0f, m_OldCount % 10, 10, 1); // 1
	DrawSpriteRotateUV({ x, y, 0.0f }, m_Size, m_Color, 0.0f, m_OldCount % 100 / 10, 10, 1); // 10
	DrawSpriteRotateUV({ x + -m_Size.x * 0.75f, y, 0.0f }, m_Size, m_Color, 0.0f, m_OldCount % 1000 / 100, 10, 1); // 100

	SetTexture(m_TextureId[1]);
	DrawSprite({ x - m_Size.x * 0.5f, y - m_Size.y * 0.75f, 0.0f }, m_Size, m_Color);

	SetTexture(m_TextureId[2]);
	DrawSpriteRotateUV({ x + m_Size.x * 0.75f, y - m_Size.y * 0.75f, 0.0f }, m_RankSize, m_Color, 0.0f, m_RankPattern, 5, 1);
}

void Count::AddCount()
{
	++m_Count;

	if (m_Count >= 999)
	{
		m_Count = 999;
	}
}

void Count::ChangeRank()
{
	if (m_Count >= 7 && m_Count < 10) {
		if (!m_RankBig && m_RankPattern == 0) 
		{
			PlayAudio(m_AudioId, false); // オーディオの再生　trueはループする

			m_RankSize.x += -m_RankSizeSpeed;
			m_RankSize.y += -m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;
		}
		else if (m_RankBig) 
		{
			m_RankSize.x += m_RankSizeSpeed;
			m_RankSize.y += m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;

			if (m_RankSize.x >= m_Size.x)
			{
				m_RankSize = m_Size;

				m_RankSizeSpeed = 0.0f;

				m_RankBig = false;
			}
		}

		if (m_RankSize.x <= 0.0f) 
		{
			m_RankPattern = 1; // ランクA

			m_RankBig = true;
		}
	}
	else if (m_Count >= 10 && m_Count < 15) {
		if (!m_RankBig && m_RankPattern == 1) 
		{
			PlayAudio(m_AudioId, false); // オーディオの再生　trueはループする

			m_RankSize.x += -m_RankSizeSpeed;
			m_RankSize.y += -m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;
		}
		else if (m_RankBig) 
		{
			m_RankSize.x += m_RankSizeSpeed;
			m_RankSize.y += m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;

			if (m_RankSize.x >= m_Size.x)
			{
				m_RankSize = m_Size;

				m_RankSizeSpeed = 0.0f;

				m_RankBig = false;
			}
		}

		if (m_RankSize.x <= 0.0f) 
		{
			m_RankPattern = 2; // ランクB

			m_RankBig = true;
		}
	}
	else if (m_Count >= 15 && m_Count < 20) {
		if (!m_RankBig && m_RankPattern == 2)
		{
			PlayAudio(m_AudioId, false); // オーディオの再生　trueはループする

			m_RankSize.x += -m_RankSizeSpeed;
			m_RankSize.y += -m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;
		}
		else if (m_RankBig)
		{
			m_RankSize.x += m_RankSizeSpeed;
			m_RankSize.y += m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;

			if (m_RankSize.x >= m_Size.x)
			{
				m_RankSize = m_Size;

				m_RankSizeSpeed = 0.0f;

				m_RankBig = false;
			}
		}

		if (m_RankSize.x <= 0.0f)
		{
			m_RankPattern = 3; // ランクC

			m_RankBig = true;
		}
	}
	else if (m_Count >= 20) {
		if (!m_RankBig && m_RankPattern == 3)
		{
			PlayAudio(m_AudioId, false); // オーディオの再生　trueはループする

			m_RankSize.x += -m_RankSizeSpeed;
			m_RankSize.y += -m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;
		}
		else if (m_RankBig)
		{
			m_RankSize.x += m_RankSizeSpeed;
			m_RankSize.y += m_RankSizeSpeed;

			m_RankSizeSpeed += 0.5f;

			if (m_RankSize.x >= m_Size.x)
			{
				m_RankSize = m_Size;

				m_RankSizeSpeed = 0.0f;

				m_RankBig = false;
			}
		}

		if (m_RankSize.x <= 0.0f)
		{
			m_RankPattern = 4; // ランクD

			m_RankBig = true;
		}
	}
}

void Count::ChangeRankResult()
{
	if (m_Count >= 7 && m_Count < 10) {
		m_RankPattern = 1; // ランクA
	}
	else if (m_Count >= 10 && m_Count < 15) {
		m_RankPattern = 2; // ランクB
	}
	else if (m_Count >= 15 && m_Count < 20) {
		m_RankPattern = 3; // ランクC
	}
	else if (m_Count >= 20) {
		m_RankPattern = 4; // ランクD
	}
}

void Count::ChangeOldRank()
{
	if (m_OldCount >= 7 && m_OldCount < 10) {
		m_RankPattern = 1; // ランクA
	}
	else if (m_OldCount >= 10 && m_OldCount < 15) {
		m_RankPattern = 2; // ランクB
	}
	else if (m_OldCount >= 15 && m_OldCount < 20) {
		m_RankPattern = 3; // ランクC
	}
	else if (m_OldCount >= 20) {
		m_RankPattern = 4; // ランクD
	}
}
