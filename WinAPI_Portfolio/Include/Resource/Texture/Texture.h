#pragma once

#include "../../Ref.h"

struct ImageInfo
{
    HDC             hMemDC;         // 메모리 DC
    HBITMAP         hBmp;           // 메모리 DC에 그려낼 비트맵 이미지
    HBITMAP         hPrevBmp;
    BITMAP          BmpInfo;        // 비트맵 이미지에 대한 정보를 담아두는 구조체
    COLORREF        ColorKey;       // 컬러키(해당 색깔만 출력하지 않게 함)
    bool            EnableColorKey; // 컬러키 활성화 여부
    TCHAR           FileName[MAX_PATH];
    TCHAR           FullPath[MAX_PATH];
    std::string     PathName;

    ImageInfo() :
        hMemDC(0),
        hBmp(0),
        hPrevBmp(0),
        BmpInfo{},
        ColorKey(RGB(255, 0, 255)),
        EnableColorKey(false),
        FileName{},
        FullPath{}
    {
    }

    ~ImageInfo()
    {
        // 도구를 원래대로 돌려준다.(순서대로 할 것)
        SelectObject(hMemDC, hPrevBmp);
        DeleteObject(hBmp);
        DeleteDC(hMemDC);
    }
};

// 리소소는 게임오브젝트와 마찬가지로 공유해서 쓰는 경우가 많기 때문에
// 레퍼런스 카운트를 하기 위해 CRef를 상속받도록 만든다.
class CTexture :
    public CRef
{
    friend class CTextureManager;

private:
    CTexture();
    ~CTexture();

private:
    ETexture_Type   m_Type;
    // 애니메이션을 구현하기 위해 이미지 정보를 여러개 담을 수 있는 배열을 만들어준다.
    std::vector<ImageInfo*> m_vecImageInfo;

public:
    int GetWidth(int Index = 0) const
    {
        return (int)m_vecImageInfo[Index]->BmpInfo.bmWidth;
    }

    int GetHeight(int Index = 0) const
    {
        return (int)m_vecImageInfo[Index]->BmpInfo.bmHeight;
    }

    ETexture_Type GetTextureType() const
    {
        return m_Type;
    }

    HDC GetDC(int Index = 0) const
    {
        return m_vecImageInfo[Index]->hMemDC;
    }

    // 색깔은 0~255 사이만 쓰므로 unsigned char 타입으로 한다.
    void SetColorKey(unsigned char r, unsigned char g, unsigned char b,
        int Index = 0)
    {
        m_vecImageInfo[Index]->ColorKey = RGB(r, g, b);
        m_vecImageInfo[Index]->EnableColorKey = true;
    }

    void SetColorKeyAll(unsigned char r, unsigned char g, unsigned char b)
    {
        size_t Size = m_vecImageInfo.size();

        for (size_t i = 0; i < Size; i++)
        {
            m_vecImageInfo[i]->ColorKey = RGB(r, g, b);
            m_vecImageInfo[i]->EnableColorKey = true;
        }
    }

    bool GetEnableColorKey(int Index = 0) const
    {
        return m_vecImageInfo[Index]->EnableColorKey;
    }

    COLORREF GetColorKey(int Index = 0) const
    {
        return m_vecImageInfo[Index]->ColorKey;
    }

public:
    bool LoadTexture(const TCHAR* FileName, 
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const TCHAR* FullPath);

#ifdef UNICODE

    bool LoadTexture(const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const std::vector<std::wstring>& vecFullPath);

#else

    bool LoadTexture(const std::vector<std::string>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(const std::vector<std::string>& vecFullPath);

#endif // UNICODE

// 타일맵을 불러올 때 사용할 텍스처 로드 함수들
private:
    bool LoadTexture(ImageInfo* Info, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(ImageInfo* Info, const TCHAR* FullPath);

#ifdef UNICODE

    bool LoadTexture(std::vector<ImageInfo*>* vecInfo, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(std::vector<ImageInfo*>* vecInfo, const std::vector<std::wstring>& vecFullPath);

#else

    bool LoadTexture(std::vector<ImageInfo*>* vecInfo, const std::vector<std::string>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTextureFullPath(std::vector<ImageInfo*>* vecInfo, const std::vector<std::string>& vecFullPath);

#endif // UNICODE
    
public:
    void Save(FILE* File);
    void Load(FILE* File);
};

