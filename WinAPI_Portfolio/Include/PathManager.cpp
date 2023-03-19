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

	// 경로 삭제
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR Root[MAX_PATH] = {};

	// 실행파일이 있는 폴더까지의 전체경로/실행파일이름.exe 로 문자열이
	// 나오게 된다. 전체경로를 구하는 이유는 어떤 컴퓨터나 폴더든 간에 
	// 실행파일이 있는 위치를 기준으로 잡을 수 있기 때문이다.
	GetModuleFileName(0, Root, MAX_PATH);

	// 전체경로 문자열의 길이 계산
	int Length = lstrlen(Root);

	for (int i = Length - 1; i >= 0; i--)
	{
		// 경로 문자열의 끝에서부터 시작해 폴더를 구분하는 문자가 나오면
		// 해당 위치보다 뒤에 있는 글자를 모두 지운다.(즉 실행파일이름.exe만 지워짐)
		if (Root[i] == '/' || Root[i] == '\\')
		{
			memset(&Root[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Root);

	// #ifdef : 뒤에 있는 내용이 #define으로 정의되어 있는지를 판단하는 if문이다.
	// 컴파일 단계에서 뒤에 있는 내용이 #define으로 정의되어 있는지 판단.
#ifdef UNICODE

	// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
	int PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 0, 0, 0, 0);

	// 실제로 변경하는 부분
	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE

	// 실행파일이 있는 폴더까지의 전체경로를 RootPath라는 이름의 키값으로 등록한다.
	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	AddPath(TEXTURE_PATH, TEXT("Texture/"));
	AddPath(SOUND_PATH, TEXT("Sound/"));
	AddPath(FONT_PATH, TEXT("Font/"));
	AddPath(MAP_PATH, TEXT("Map/"));

	return true;
}

bool CPathManager::AddPath(const std::string Name, const TCHAR* Path, const std::string& BasePathName)
{
	// 같은 이름의 경로가 있다면 빠져나간다.
	if (FindPath(Name))
		return false;

	// 베이스 경로를 얻어온다.
	const PathInfo* BasePath = FindPath(BasePathName);

	PathInfo* Info = new PathInfo;

	// 베이스 경로가 있다면 새 경로에 베이스 경로를 복사한다.
	if (BasePath)
		lstrcpy(Info->Path, BasePath->Path);

	// 복사한 베이스 경로 뒤에 인자로 넣어준 경로를 붙여넣는다.
	lstrcat(Info->Path, Path);

#ifdef UNICODE

	// 유니코드로 되어있는 문자열을 멀티바이트로 바꾸기 위한 수를 얻어온다.
	int PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, 0, 0, 0, 0);

	// 실제로 변경하는 부분
	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE

	// 키값이 Name인 새 경로를 트리에 등록한다.
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
