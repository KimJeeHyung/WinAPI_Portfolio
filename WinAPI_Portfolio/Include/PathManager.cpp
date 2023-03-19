#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	std::unordered_map<std::string, PathInfo*>::iterator iter;
	std::unordered_map<std::string, PathInfo*>::iterator iterEnd;

	iter = m_mapPath.begin();
	iterEnd = m_mapPath.end();

	// ��� ����
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR Root[MAX_PATH] = {};

	// ���������� �ִ� ���������� ��ü���/���������̸�.exe �� ���ڿ���
	// ������ �ȴ�. ��ü��θ� ���ϴ� ������ � ��ǻ�ͳ� ������ ���� 
	// ���������� �ִ� ��ġ�� �������� ���� �� �ֱ� �����̴�.
	GetModuleFileName(0, Root, MAX_PATH);

	// ��ü��� ���ڿ��� ���� ���
	int Length = lstrlen(Root);

	for (int i = Length - 1; i >= 0; i--)
	{
		// ��� ���ڿ��� ���������� ������ ������ �����ϴ� ���ڰ� ������
		// �ش� ��ġ���� �ڿ� �ִ� ���ڸ� ��� �����.(�� ���������̸�.exe�� ������)
		if (Root[i] == '/' || Root[i] == '\\')
		{
			memset(&Root[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Root);

	// #ifdef : �ڿ� �ִ� ������ #define���� ���ǵǾ� �ִ����� �Ǵ��ϴ� if���̴�.
	// ������ �ܰ迡�� �ڿ� �ִ� ������ #define���� ���ǵǾ� �ִ��� �Ǵ�.
#ifdef UNICODE

	// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
	int PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 0, 0, 0, 0);

	// ������ �����ϴ� �κ�
	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE

	// ���������� �ִ� ���������� ��ü��θ� RootPath��� �̸��� Ű������ ����Ѵ�.
	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	AddPath(TEXTURE_PATH, TEXT("Texture/"));
	AddPath(SOUND_PATH, TEXT("Sound/"));
	AddPath(FONT_PATH, TEXT("Font/"));
	AddPath(MAP_PATH, TEXT("Map/"));

	return true;
}

bool CPathManager::AddPath(const std::string Name, const TCHAR* Path, const std::string& BasePathName)
{
	// ���� �̸��� ��ΰ� �ִٸ� ����������.
	if (FindPath(Name))
		return false;

	// ���̽� ��θ� ���´�.
	const PathInfo* BasePath = FindPath(BasePathName);

	PathInfo* Info = new PathInfo;

	// ���̽� ��ΰ� �ִٸ� �� ��ο� ���̽� ��θ� �����Ѵ�.
	if (BasePath)
		lstrcpy(Info->Path, BasePath->Path);

	// ������ ���̽� ��� �ڿ� ���ڷ� �־��� ��θ� �ٿ��ִ´�.
	lstrcat(Info->Path, Path);

#ifdef UNICODE

	// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ���� ���´�.
	int PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 0, 0, 0, 0);

	// ������ �����ϴ� �κ�
	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE

	// Ű���� Name�� �� ��θ� Ʈ���� ����Ѵ�.
	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	std::unordered_map<std::string, PathInfo*>::iterator iter;
	iter = m_mapPath.find(Name);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}
