#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

struct PathInfo
{
	TCHAR	Path[MAX_PATH];				// 경로 문자열(유니코드), MAX_PATH는 260
	char	PathMultibyte[MAX_PATH];	// 경로 문자열(멀티바이트)

	PathInfo() :
		Path{},
		PathMultibyte{}
	{
	}
};

class CPathManager
{
private:
	// 여러가지 경로들을 저장해두는 트리
	// Key는 문자열, Value는 실제 경로
	std::unordered_map<std::string, PathInfo*> m_mapPath;
		
public:
	bool Init();
	bool AddPath(const std::string Name, const TCHAR* Path,
		const std::string& BasePathName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& Name);

	DECLARE_SINGLE(CPathManager)
};

