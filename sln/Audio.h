#pragma once
/*****************************************************************/
//
// 
// Audio.h（オーディオ表示）
//
//
//
//                    ２０２４年09月26日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include <xaudio2.h>

void InitAudio();
void UninitAudio();


int LoadAudio(const char* FileName);
void UnloadAudio(int Index);
void PlayAudio(int Index, bool Loop = false);

