/*****************************************************************/
//
// 
// model.cpp（モデル表示）
//
//
//
//                    ２０２４年11月05日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "main.h"
#include "texture.h"
#include "model.h"
#include <assert.h>
#include "renderer.h"

static int g_TextureWhite = -1;



MODEL* ModelLoad(const char* FileName, bool MayaFbx )
{
	MODEL* model = new MODEL;


	const std::string modelPath( FileName );

	model->AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(model->AiScene); //assert → 中身がNULLだとストップさせる

	model->VertexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];
	model->IndexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];

	XMFLOAT3 min;
	XMFLOAT3 max;

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

		//aiVector3D boundingbox_max = mesh->mAABB.mMax;
		//aiVector3D boundingbox_min = mesh->mAABB.mMin;


		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				if (!MayaFbx)
				{
					vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, -mesh->mVertices[v].z, mesh->mVertices[v].y);
				}
				else
				{
					vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				}

				//走査
				if (m == 0 && v == 0)
				{
					min = max = vertex[v].Position;
				}
				else
				{
					min.x = std::min(vertex[v].Position.x, min.x); //std::min 小さいほうを選ぶ
					min.y = std::min(vertex[v].Position.y, min.y); //std::min 小さいほうを選ぶ
					min.z = std::min(vertex[v].Position.z, min.z); //std::min 小さいほうを選ぶ
					max.x = std::max(vertex[v].Position.x, max.x); //std::max 大きいほうを選ぶ
					max.y = std::max(vertex[v].Position.y, max.y); //std::max 大きいほうを選ぶ
					max.z = std::max(vertex[v].Position.z, max.z); //std::max 大きいほうを選ぶ
				}

				vertex[v].TexCoord = XMFLOAT2( mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, -mesh->mNormals[v].z, mesh->mNormals[v].y);
				//vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
			}

			D3D11_BUFFER_DESC bd = {};
			//ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd = {};
			//ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			GetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);

			delete[] vertex;
		}


		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd = {};
			//ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd = {};
			//ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			GetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

			delete[] index;
		}

	}

	model->aabb = { min, max };

	if (!model->AiScene->mNumTextures)
	{
		g_TextureWhite = TextureLoad(L"asset/texture/white.png");
	}
	else
	{
		//テクスチャ読み込み
		for (unsigned int i = 0; i < model->AiScene->mNumTextures; i++)
		{
			aiTexture* aitexture = model->AiScene->mTextures[i];

			ID3D11ShaderResourceView* texture;
			TexMetadata metadata;
			ScratchImage image;
			LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
			CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
			assert(texture);

			model->Texture[aitexture->mFilename.data] = texture;
		}
	}

	return model;
}




void ModelDraw(MODEL* model, bool bforceTexture)
{
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		if (!bforceTexture)
		{
			//テクスチャ設定
			if (model->AiScene->mNumTextures)
			{
				aiString texture;
				aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[m]->mMaterialIndex];
				aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);

				if (texture != aiString(""))
				{
					GetDeviceContext()->PSSetShaderResources(0, 1, &model->Texture[texture.data]);
				}
			}
			else
			{
				ID3D11ShaderResourceView* p = GetTexture(g_TextureWhite);
				GetDeviceContext()->CSSetShaderResources(0, 1, &p);
			}
		}

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer[m], &stride, &offset);

		//インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(model->IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0); //unsigned intだとR32

		//プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));

		aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[m]->mMaterialIndex];
		aiColor3D diffuse;
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

		material.Diffuse = { diffuse.r, diffuse.g, diffuse.b, 1.0f }; //モデルの色
		SetMaterial(material);

		// ポリゴン描画
		GetDeviceContext()->DrawIndexed(model->AiScene->mMeshes[m]->mNumFaces * 3, 0, 0); //左の36はポリゴンの表示に使用する頂点数
	}
}




void ModelRelease(MODEL* model)
{
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		model->VertexBuffer[m]->Release();
		model->IndexBuffer[m]->Release();
	}

	delete[] model->VertexBuffer;
	delete[] model->IndexBuffer;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
	{
		pair.second->Release();
	}


	aiReleaseImport(model->AiScene);


	delete model;
}






