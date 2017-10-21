//---------------------------------------------------------------------------

#ifndef BMPHelperH
#define BMPHelperH
//---------------------------------------------------------------------------

// 类名：TBMPHelper
// 功能：解析、创建、保存BMP位图文件
// 版本：1.2
// 作者：PanXL
// 时间：2017-10-21

// 说明：
// 本解决方案仅支持主流的24位真彩色位图格式

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

// BMP文件头定义
struct DEF_BITMAPFILEHEADER
{
	// 2字节，指示文件的类型，Windows下取0x4d42H
	UINT16 bfType;

	// 4字节，指示位图文件大小
	DWORD  bfSize;

	// 2字节，保留，设置为0
	UINT16 bfReserved1;

	// 2字节，保留，设置为0
	UINT16 bfReserved2;

	// 4字节，指示从文件头开始到实际图像数据之间的字节偏移量
	DWORD  bfOffBits;
};

// BMP信息头定义
struct DEF_BMPINFOHEADER
{
	// 4字节，指示DEF_BMPINFOHEADER结构大小（单位：字节）
	DWORD biSize;

	// 4字节，指示图像宽度（单位：像素）
	LONG biWidth;

	// 4字节，指示图像高度（单位：像素）
	// 该值可正可负，正值说明图像是倒向的，负值说明图像是正向的
	LONG biHeight;

	// 2字节，指示颜色平面数，总设为1
	WORD biPlanes;

	// 2字节，指示每像素的比特数，取1，4，8，16，24，32
	WORD biBitCount;

	// 4字节，指示图像数据压缩的类型
	// 取值：0（BI_RGB，不压缩，最常用）
	DWORD biCompression;

	// 4字节，指示图像的大小（单位：字节）
	// 当biCompression=0时，可置为0
	DWORD biSizeImage;

	// 4字节，指示水平分辨率（像素/米）
	LONG biXPelsPerMeter;

	// 4字节，指示垂直 分辨率（像素/米）
	LONG biYPelsPerMeter;

	// 4字节，指示位图实际使用的彩色表中的颜色索引数
	// 设为0说明使用所有的调色板项
	DWORD biClrUsed;

	// 4字节，指示对图象显示有重要影响的颜色索引的数目
	// 如果是0，表示都重要
	DWORD biClrImportant;
};
//---------------------------------------------------------------------------

// BMP文件格式定义
struct BITMAPFILE
{
	// BMP文件头
	DEF_BITMAPFILEHEADER FileHeader;

	// BMP信息头
	DEF_BMPINFOHEADER  InfoHeader;

	// 调色板支持暂未实现

	// 位图数据
	BYTE* BMPData;
};
//---------------------------------------------------------------------------

#pragma pack(pop)

class TBMPHelper
{
public:
	TBMPHelper(void);
	~TBMPHelper(void);

	// 打开BMP位图文件
	// 成功返回0
	int OpenBMPFile(const wchar_t* wszFileName);

	// 将BMP位图保存为文件
	// 成功返回0
	int SaveToBMPFile(const wchar_t* wszFileName);

	// 像素颜色分量表达序列互换：BGR<-->RGB
	void BRSwitch(void);

	// 图像垂直反转
	void BMPReverseV(void);

	BITMAPFILE *m_BMPFile;
	LONG  m_RealImageWidth;
	LONG  m_RealImageHeight;
	// 位图数据（单位：字节）
	DWORD m_RealImageDataSize;
private:

    // 初始化位图文件信息
    void InitBMPFile(void);
};
//---------------------------------------------------------------------------
#endif
