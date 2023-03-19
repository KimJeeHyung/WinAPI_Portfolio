#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

struct PathInfo
{
	TCHAR	Path[MAX_PATH];				// ��� ���ڿ�(�����ڵ�), MAX_PATH�� 260
	char	PathMultibyte[MAX_PATH];	// ��� ���ڿ�(��Ƽ����Ʈ)

	PathInfo() :
		Path{},
		PathMultibyte{}
	{
	}
};

class CPathManager
{
private:
	// �������� ��ε��� �����صδ� Ʈ��
	// Key�� ���ڿ�, Value�� ���� ���
	std::unordered_map<std::string, PathInfo*> m_mapPath;
		
public:
	bool Init();
	bool AddPath(const std::string Name, const TCHAR* Path,
		const std::string& BasePathName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& Name);

	DECLARE_SINGLE(CPathManager)
};

