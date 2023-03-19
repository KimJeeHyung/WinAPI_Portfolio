#pragma once
#include "Scene.h"

/*타일맵을 직접 편집할 수 있는 에디터 씬*/
class CEditScene :
    public CScene
{
	friend class CSceneManager;

protected:
	CEditScene();
	virtual ~CEditScene();

private:
	class CEditDlg* m_TileMapDlg;			// 타일맵 에디터 다이얼로그

public:
	bool Init();

public:
	void CreateTileMap(int CountX, int CountY, int SizeX, int SizeY);
	void SetTileTexture(class CTexture* Texture);
	void LoadTileMap(const char* FullPath);

public:
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void OpenTileMapEditor();
	void MouseLButtonDrag();

private:
	void ChangeFrame();
	void ChangeOption();
};

