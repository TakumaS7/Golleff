#pragma once
/*****************************************************************/
//
// 
// model.h（モデル表示）
//
//
//
//                    ２０２４年11月05日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib") //#pragma commentはライブラリのリンクにアクセスしてくれる（プロパティ設定しなくてよい）

#include "collision.h"

struct MODEL
{
	const aiScene* AiScene = nullptr;

	ID3D11Buffer** VertexBuffer;
	ID3D11Buffer** IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture;

	AABB aabb;
};


MODEL* ModelLoad(const char* FileName, bool MayaFbx);
void ModelDraw(MODEL* model, bool bforceTexture = false); //←デフォルト引数
void ModelRelease(MODEL* model);

