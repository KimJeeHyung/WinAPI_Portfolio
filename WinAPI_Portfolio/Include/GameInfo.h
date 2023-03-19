#pragma once

// �� ��������� ���ӿ��� �������� ���Ǵ� ������� ��Ƶΰ�
// Include�ؼ� ����� �������� ���۵Ǿ���.
#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>	// �迭 ������ ���� std::sort() �� ���� ���� �ʿ�
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


#define GRAVITY 9.8f	// �߷� ���ӵ�

// �̹����� ����ϱ� ���� ���̺귯��(�⺻ ����)
#pragma comment(lib, "msimg32.lib")
// ���带 ����ϱ� ���� ���̺귯��(FMOD ����Ʈ ���� �ٿ�޾ƾߵ�)
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

// �ػ� ����ü
struct Resolution
{
	int Width;
	int Height;
};

// �ִϸ��̼� ��������Ʈ���� ������ �κ��� ���۰� �� ��ǥ
struct AnimationFrameData
{
	Vector2 Start;
	Vector2 End;
};

// �浹ü ����
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

// �ڽ� �ݶ��̴� ����
struct BoxInfo
{
	Vector2 LT;		// �»�� ��ǥ
	Vector2 RB;		// ���ϴ� ��ǥ
};

// ��Ŭ �ݶ��̴� ����
struct CircleInfo
{
	Vector2 Center;		// ����
	float	Radius;		// ������
};