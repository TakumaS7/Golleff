#pragma once
/*****************************************************************/
//
// 
// light.h（ライト処理）
//
//
//
//                    ２０２４年10月16日　製作者名：佐藤琢磨
//
/*****************************************************************/
#include "main.h"

void InitLight(void);
void UninitLight(void);

void SetDirectionalLight(const XMVECTOR& dir, const XMFLOAT4& color);
void SetAmbient(const XMFLOAT4& color);

//ライトの設定
// 
// 引数
//		world ... ライティング対象のワールド変換行列
// 
void SetLight(const XMMATRIX& world);
