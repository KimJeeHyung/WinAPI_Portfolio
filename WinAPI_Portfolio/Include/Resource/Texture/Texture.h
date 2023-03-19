#pragma once

#include "../../Ref.h"

struct ImageInfo
{
    HDC             hMemDC;         // �޸� DC
    HBITMAP         hBmp;           // �޸� DC�� �׷��� ��Ʈ�� �̹���
    HBITMAP         hPrevBmp;
    BITMAP          BmpInfo;        // ��Ʈ�� �̹����� ���� ������ ��Ƶδ� ����ü
    COLORREF        ColorKey;       // �÷�Ű(�ش� ���� ������� �ʰ� ��)
    bool            EnableColorKey; // �÷�Ű Ȱ��ȭ ����
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
        // ������ ������� �����ش�.(������� �� ��)
        SelectObject(hMemDC, hPrevBmp);
        DeleteObject(hBmp);
        DeleteDC(hMemDC);
    }
};

// ���ҼҴ� ���ӿ�����Ʈ�� ���������� �����ؼ� ���� ��찡 ���� ������
// ���۷��� ī��Ʈ�� �ϱ� ���� CRef�� ��ӹ޵��� �����.
class CTexture :
    public CRef
{
    friend class CTextureManager;

private:
    CTexture();
    ~CTexture();

private:
    ETexture_Type   m_Type;
    // �ִϸ��̼��� �����ϱ� ���� �̹��� ������ ������ ���� �� �ִ� �迭�� ������ش�.
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

    // ������ 0~255 ���̸� ���Ƿ� unsigned char Ÿ������ �Ѵ�.
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

// Ÿ�ϸ��� �ҷ��� �� ����� �ؽ�ó �ε� �Լ���
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

