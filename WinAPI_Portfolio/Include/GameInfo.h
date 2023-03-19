#pragma once

// 이 헤더파일은 게임에서 공통으로 사용되는 내용들을 담아두고
// Include해서 사용할 목적으로 제작되었다.
#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>	// 배열 정렬을 위한 std::sort() 를 쓰기 위해 필요
#include <stack>

#include "Vector2.h"
#include "SharedPtr.h"
#include "Flag.h"
#include "fmod.hpp"

#ifdef _DEBUG

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else

#define DBG_NEW new

#endif // _DEBUG


#define GRAVITY 9.8f	// 중력 가속도

// 이미지를 사용하기 위한 라이브러리(기본 제공)
#pragma comment(lib, "msimg32.lib")
// 사운드를 재생하기 위한 라이브러리(FMOD 사이트 가서 다운받아야됨)
#pragma comment(lib, "../Bin/fmod64_vc.lib")

#define ROOT_PATH		"RootPath"
#define TEXTURE_PATH	"TexturePath"
#define SOUND_PATH		"SoundPath"
#define FONT_PATH		"FontPath"
#define MAP_PATH		"MapPath"

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) { p->Release(); p =nullptr; }

float RadianToDegree(float Radian);
float DegreeToRadian(float Degree);

#define DELTA_TIME CGameManager::GetInst()->GetDeltaTime()

// 해상도 구조체
struct Resolution
{
	int Width;
	int Height;
};

// 애니메이션 스프라이트에서 가져올 부분의 시작과 끝 좌표
struct AnimationFrameData
{
	Vector2 Start;
	Vector2 End;
};

// 충돌체 정보
struct CollisionProfile
{
	std::string							Name;
	ECollision_Channel					Channel;
	bool								Enable;
	std::vector<ECollision_Interaction> vecCollisionInteraction;

	CollisionProfile() :
		Enable(true)
	{
	}
};

// 박스 콜라이더 정보
struct BoxInfo
{
	Vector2 LT;		// 좌상단 좌표
	Vector2 RB;		// 우하단 좌표
};

// 서클 콜라이더 정보
struct CircleInfo
{
	Vector2 Center;		// 중점
	float	Radius;		// 반지름
};