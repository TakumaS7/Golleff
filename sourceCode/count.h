#pragma once
/*****************************************************************/
//
// 
// count.hi‘Å”•\Ž¦j
//
//
//
//                    ‚Q‚O‚Q‚S”N‚P‚QŒŽ‚Q‚S“ú@»ìŽÒ–¼F²“¡‘ô–
//
/*****************************************************************/
#include "texture.h"
#include "main.h"

class Count
{
private:
	int m_Count = 0;
	int m_OldCount = 1000;
	int m_TextureId[3]{};
	int m_AudioId = -1;
	int m_RankPattern = 0;
	bool m_RankBig = false;
	float m_RankSizeSpeed = 0.0f;
	XMFLOAT2 m_Size = {90.0f, 90.0f};
	XMFLOAT2 m_RankSize = m_Size;
	XMFLOAT4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	Count() = default;

	~Count() {}

	void InitCount();
	void UninitCount();
	void DrawCount(float x, float y);
	void DrawOldCount(float x, float y);

	void AddCount();

	void SetSize(XMFLOAT2 size) {
		m_Size = size; 
		m_RankSize = size;
	}

	int GetCount() const { return m_Count; }
	int GetOldCount() const { return m_OldCount; }

	void SetCount(int count) { m_Count = count; }
	void SetOldCount(int count) { m_OldCount = count; }

	bool CheckCount(int OldCount, int count) const { return OldCount > count; }

	void ChangeRank();
	void ChangeRankResult();
	void ChangeOldRank();
};