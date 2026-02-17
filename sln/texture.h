/*==============================================================================

   テクスチャ管理 [texture.h]
														 Author : Youhei Sato
														 Date   : 2024/06/04
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3d11.h>

//テクスチャモジュールの初期化
void TextureInitialize(void);

//テクスチャの読み込み
int TextureLoad(const std::wstring& texture_filename);

//番号を入れて出力
ID3D11ShaderResourceView* GetTexture(int id);

void SetTexture(int id);

//その番号の大きさ取得
int TextureGetWidth(int id);
int TextureGetHeight(int id);

//全開放（全てを開放する）
void TextureFinalize(void);

#endif // TEXTURE_H
