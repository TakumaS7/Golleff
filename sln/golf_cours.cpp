/*****************************************************************/
//
// 
// golf_cours.cpp（ゴルフコース表示）
//
//
//
//                    ２０２４年１０月２９日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "golf_cours.h"
#include "cube.h"
#include "renderer.h"
#include "light.h"

void GolfCours::DrawMap()
{
	for (int i = 0; i < sizeof(g_Map) / sizeof(g_Map[0]); i++)
	{
		XMMATRIX translation = XMMatrixTranslation(g_Map[i].x, g_Map[i].y, g_Map[i].z); //場所を動かす
		XMVECTOR worldLightVector = XMVector3Normalize({ -1.0f, 0.0f, 0.0f, 0.0f }); //単位ベクトルであること
		
		XMVECTOR localLightVector = XMVector3TransformNormal(worldLightVector, XMMatrixInverse(nullptr, translation));
		SetLight(translation);

		SetWorldMatrix(translation);
		DrawCube();
	}
}
