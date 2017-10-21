//---------------------------------------------------------------------------

#pragma hdrstop

#include "BMPHelper.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TBMPHelper::TBMPHelper(void)
{
	InitBMPFile();
}
//---------------------------------------------------------------------------

TBMPHelper::~TBMPHelper(void)
{
	if(NULL != m_BMPFile->BMPData)
	{
		delete []m_BMPFile->BMPData;
		m_BMPFile->BMPData = NULL;
	}

	delete m_BMPFile;
}
//---------------------------------------------------------------------------

void TBMPHelper::InitBMPFile(void)
{
    m_BMPFile = new BITMAPFILE;
	memset(&(m_BMPFile->FileHeader), 0, sizeof(m_BMPFile->FileHeader));
	memset(&(m_BMPFile->InfoHeader), 0, sizeof(m_BMPFile->InfoHeader));
	m_BMPFile->BMPData = NULL;

	m_RealImageWidth    = 0;
	m_RealImageHeight   = 0;
	m_RealImageDataSize = 0;
}
//---------------------------------------------------------------------------

int TBMPHelper::OpenBMPFile(const wchar_t* wszFileName)
{
    wchar_t wszOpenMode[] = {'r','b'};
	FILE *pFile = _wfopen(wszFileName, wszOpenMode);
	if(NULL == pFile)
    {
		return 1;
	}

	// ����λͼ�ļ�ͷ
	fseek(pFile, 0, SEEK_SET);
	fread(&(m_BMPFile->FileHeader), sizeof(m_BMPFile->FileHeader), 1, pFile);

	// ������Ч��BMPλͼ�ļ�
	if(0x4d42 != m_BMPFile->FileHeader.bfType)
	{
		fclose(pFile);
		return 2;
    }

	// ����λͼ��Ϣͷ
	fseek(pFile, sizeof(m_BMPFile->FileHeader), SEEK_SET);
	fread(&(m_BMPFile->InfoHeader), sizeof(m_BMPFile->InfoHeader), 1, pFile);

	// ��֧��24λ���ɫͼ��
	if(24 != m_BMPFile->InfoHeader.biBitCount)
	{
		fclose(pFile);
		return 3;
	}

	// ����λͼ���ݴ�С���������������Ҫ���߶�ֵ��������
	m_RealImageWidth   = m_BMPFile->InfoHeader.biWidth;
	m_RealImageHeight  = m_BMPFile->InfoHeader.biHeight > 0 ? m_BMPFile->InfoHeader.biHeight : -(m_BMPFile->InfoHeader.biHeight);
	WORD BitsPerPixel = m_BMPFile->InfoHeader.biBitCount;

	DWORD LineByteCnt = (m_RealImageWidth*BitsPerPixel)/8;

	m_RealImageDataSize = LineByteCnt*m_RealImageHeight;
	m_BMPFile->BMPData = new BYTE[m_RealImageDataSize];

	fseek(pFile, m_BMPFile->FileHeader.bfOffBits, SEEK_SET);

	// ����Ҫ���ֽڶ��봦��
	if(0 == LineByteCnt%4)
	{
		// ����λͼ����
		fread(m_BMPFile->BMPData, m_RealImageDataSize, 1, pFile);
	}
	else
	{
		BYTE *WTDataBuf = m_BMPFile->BMPData;
		LONG WTSize = 3*m_RealImageWidth;
		int FillOffset = 4 - LineByteCnt%4;

		for(int iY=0; iY<m_RealImageHeight; iY++)
		{
			// ����λͼ����
			fread(WTDataBuf, WTSize, 1, pFile);
			WTDataBuf += WTSize;
			fseek(pFile, FillOffset, SEEK_CUR);
		}
	}

	fclose(pFile);

	return 0;
}
//---------------------------------------------------------------------------

int TBMPHelper::SaveToBMPFile(const wchar_t* wszFileName)
{
	if(NULL == m_BMPFile->BMPData)
	{
		return 1;
	}

	wchar_t wszOpenMode[] = {'w','b'};
	FILE *pFile = _wfopen(wszFileName, wszOpenMode);
	if(pFile == NULL)
	{
		return 2;
	}

	// д��λͼ�ļ�ͷ
	fseek(pFile, 0, SEEK_SET);
	fwrite(&(m_BMPFile->FileHeader), sizeof(m_BMPFile->FileHeader), 1, pFile);

	// д��λͼ��Ϣͷ
	fwrite(&(m_BMPFile->InfoHeader), sizeof(m_BMPFile->InfoHeader), 1, pFile);

	// д��λͼ����
	LONG BitsPerPixel = m_BMPFile->InfoHeader.biBitCount;
	LONG LineByteCnt = (m_RealImageWidth*BitsPerPixel)/8;

	// ����Ҫ���ֽڶ��봦��
	if(0 == LineByteCnt%4)
	{
		fwrite(m_BMPFile->BMPData, m_RealImageDataSize, 1, pFile);
	}
	else
	{
		BYTE *WTDataBuf = m_BMPFile->BMPData;
		LONG WTSize = 3*m_RealImageWidth;
		int FillOffset = 4 - LineByteCnt%4;
		BYTE ZeroFill = 0x00;
		for(int iY=0; iY<m_RealImageHeight; iY++)
		{
			fwrite(WTDataBuf, WTSize, 1, pFile);
			fwrite(&ZeroFill, sizeof(BYTE), FillOffset, pFile);
			WTDataBuf += WTSize;
		}
	}

	fclose(pFile);

	return 0;
}
//---------------------------------------------------------------------------

void TBMPHelper::BRSwitch(void)
{
	if(NULL == m_BMPFile->BMPData)
	{
		return;
	}

	BYTE Temp;
	DWORD MaxDataIndex = m_RealImageDataSize - 1;
	for(DWORD i=0; i<MaxDataIndex; i+=3)
	{
		Temp = m_BMPFile->BMPData[i+2];
		m_BMPFile->BMPData[i+2] = m_BMPFile->BMPData[i];
		m_BMPFile->BMPData[i] = Temp;
    }
}
//---------------------------------------------------------------------------

void TBMPHelper::BMPReverseV(void)
{
    if(NULL == m_BMPFile->BMPData)
	{
		return;
	}

	if(m_RealImageWidth <= 0 || m_RealImageHeight <= 0)
	{
        return;
	}

	int HalfV = m_RealImageHeight/2;
	int MoveSize = 3*m_RealImageWidth;
	BYTE *DataBuf = new BYTE[MoveSize];

	BYTE *UpPos = m_BMPFile->BMPData;
	BYTE *DownPos = m_BMPFile->BMPData + MoveSize*(m_RealImageHeight - 1);

	for(int iY=0; iY<HalfV; iY++)
	{
		memmove(DataBuf, UpPos, MoveSize);
		memmove(UpPos, DownPos, MoveSize);
		memmove(DownPos, DataBuf, MoveSize);

		UpPos += MoveSize;
		DownPos -= MoveSize;
	}

	delete DataBuf;
}
//---------------------------------------------------------------------------
