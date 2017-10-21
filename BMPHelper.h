//---------------------------------------------------------------------------

#ifndef BMPHelperH
#define BMPHelperH
//---------------------------------------------------------------------------

// ������TBMPHelper
// ���ܣ�����������������BMPλͼ�ļ�
// �汾��1.2
// ���ߣ�PanXL
// ʱ�䣺2017-10-21

// ˵����
// �����������֧��������24λ���ɫλͼ��ʽ

//---------------------------------------------------------------------------
#include <stdio.h>
#include <memory.h>
//---------------------------------------------------------------------------

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned short UINT16;
typedef unsigned long  DWORD;
typedef long           LONG;
//---------------------------------------------------------------------------

#pragma pack(push)
#pragma pack(2)

// BMP�ļ�ͷ����
struct DEF_BITMAPFILEHEADER
{
	// 2�ֽڣ�ָʾ�ļ������ͣ�Windows��ȡ0x4d42H
	UINT16 bfType;

	// 4�ֽڣ�ָʾλͼ�ļ���С
	DWORD  bfSize;

	// 2�ֽڣ�����������Ϊ0
	UINT16 bfReserved1;

	// 2�ֽڣ�����������Ϊ0
	UINT16 bfReserved2;

	// 4�ֽڣ�ָʾ���ļ�ͷ��ʼ��ʵ��ͼ������֮����ֽ�ƫ����
	DWORD  bfOffBits;
};

// BMP��Ϣͷ����
struct DEF_BMPINFOHEADER
{
	// 4�ֽڣ�ָʾDEF_BMPINFOHEADER�ṹ��С����λ���ֽڣ�
	DWORD biSize;

	// 4�ֽڣ�ָʾͼ���ȣ���λ�����أ�
	LONG biWidth;

	// 4�ֽڣ�ָʾͼ��߶ȣ���λ�����أ�
	// ��ֵ�����ɸ�����ֵ˵��ͼ���ǵ���ģ���ֵ˵��ͼ���������
	LONG biHeight;

	// 2�ֽڣ�ָʾ��ɫƽ����������Ϊ1
	WORD biPlanes;

	// 2�ֽڣ�ָʾÿ���صı�������ȡ1��4��8��16��24��32
	WORD biBitCount;

	// 4�ֽڣ�ָʾͼ������ѹ��������
	// ȡֵ��0��BI_RGB����ѹ������ã�
	DWORD biCompression;

	// 4�ֽڣ�ָʾͼ��Ĵ�С����λ���ֽڣ�
	// ��biCompression=0ʱ������Ϊ0
	DWORD biSizeImage;

	// 4�ֽڣ�ָʾˮƽ�ֱ��ʣ�����/�ף�
	LONG biXPelsPerMeter;

	// 4�ֽڣ�ָʾ��ֱ �ֱ��ʣ�����/�ף�
	LONG biYPelsPerMeter;

	// 4�ֽڣ�ָʾλͼʵ��ʹ�õĲ�ɫ���е���ɫ������
	// ��Ϊ0˵��ʹ�����еĵ�ɫ����
	DWORD biClrUsed;

	// 4�ֽڣ�ָʾ��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ
	// �����0����ʾ����Ҫ
	DWORD biClrImportant;
};
//---------------------------------------------------------------------------

// BMP�ļ���ʽ����
struct BITMAPFILE
{
	// BMP�ļ�ͷ
	DEF_BITMAPFILEHEADER FileHeader;

	// BMP��Ϣͷ
	DEF_BMPINFOHEADER  InfoHeader;

	// ��ɫ��֧����δʵ��

	// λͼ����
	BYTE* BMPData;
};
//---------------------------------------------------------------------------

#pragma pack(pop)

class TBMPHelper
{
public:
	TBMPHelper(void);
	~TBMPHelper(void);

	// ��BMPλͼ�ļ�
	// �ɹ�����0
	int OpenBMPFile(const wchar_t* wszFileName);

	// ��BMPλͼ����Ϊ�ļ�
	// �ɹ�����0
	int SaveToBMPFile(const wchar_t* wszFileName);

	// ������ɫ����������л�����BGR<-->RGB
	void BRSwitch(void);

	// ͼ��ֱ��ת
	void BMPReverseV(void);

	BITMAPFILE *m_BMPFile;
	LONG  m_RealImageWidth;
	LONG  m_RealImageHeight;
	// λͼ���ݣ���λ���ֽڣ�
	DWORD m_RealImageDataSize;
private:

    // ��ʼ��λͼ�ļ���Ϣ
    void InitBMPFile(void);
};
//---------------------------------------------------------------------------
#endif
