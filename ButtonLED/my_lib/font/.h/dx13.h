/**
  ******************************************************************************
  * 简介：适用于pal库oled驱动的字体文件
  *       该文件由bdf2c软件自动生成，工具获取方式 Bilibili关注"铁头山羊"
  * 字体名称：-FreeType-DengXian-Medium-R-Normal--18-130-100-100-P-159-ISO10646-1
  * 字体字号：13磅
  * 字符数量：6/29221
  * 备注：123
  ******************************************************************************
*/

#ifndef dx13_H_
#define dx13_H_

#include "oled_font.h"

/* 字形数据 */
static const uint8_t dx13_GlyphBitmap_4E16[] = {0x01,0x08,0x00,0x11,0x08,0x00,0x11,0x08,0x00,0x11,0x08,0x00,0x11,0x08,0x00,0xFF,0xFF,0x80,0x11,0x08,0x00,0x11,0x08,0x00,0x11,0x08,0x00,0x11,0x08,0x00,0x11,0xF8,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x1F,0xFF,0x00,}; // 世
static const uint8_t dx13_GlyphBitmap_4F60[] = {0x19,0x80,0x11,0x00,0x21,0x00,0x23,0xFE,0x62,0x06,0xE4,0x44,0xAC,0x40,0xA0,0x40,0x20,0x48,0x23,0x48,0x22,0x4C,0x26,0x46,0x24,0x42,0x2C,0x42,0x20,0x40,0x21,0xC0,}; // 你
static const uint8_t dx13_GlyphBitmap_597D[] = {0x10,0x00,0x23,0xFC,0x20,0x0C,0x20,0x08,0xFC,0x10,0x24,0x20,0x44,0x20,0x47,0xFE,0x68,0x20,0x38,0x20,0x10,0x20,0x18,0x20,0x24,0x20,0x40,0x20,0x00,0xE0,}; // 好
static const uint8_t dx13_GlyphBitmap_754C[] = {0x1F,0xFC,0x10,0x84,0x10,0x84,0x1F,0xFC,0x10,0x84,0x10,0x84,0x1F,0xFC,0x06,0x30,0x0C,0x18,0x32,0x17,0xE2,0x11,0x02,0x10,0x02,0x10,0x04,0x10,0x38,0x10,}; // 界

/* 映射表 */
static const uint32_t dx13_Map[] = {19990,20320,22909,30028,};

/* 字形 */
static const Glyph_TypeDef dx13_Glyphs[] = {
	// 世
	{
		.Name = "4E16", 
		.Encoding = 19990, 
		.Swx0 = 996, 
		.Swy0 = 0, 
		.Dwx0 = 18, 
		.Dwy0 = 0, 
		.BBw = 17, 
		.BBh = 15, 
		.BBxoff0x = 1, 
		.BByoff0y = -2, 
		.nBytes = 45, 
		.Bitmap = dx13_GlyphBitmap_4E16, 
	},
	// 你
	{
		.Name = "4F60", 
		.Encoding = 20320, 
		.Swx0 = 941, 
		.Swy0 = 0, 
		.Dwx0 = 17, 
		.Dwy0 = 0, 
		.BBw = 15, 
		.BBh = 16, 
		.BBxoff0x = 1, 
		.BByoff0y = -1, 
		.nBytes = 32, 
		.Bitmap = dx13_GlyphBitmap_4F60, 
	},
	// 好
	{
		.Name = "597D", 
		.Encoding = 22909, 
		.Swx0 = 941, 
		.Swy0 = 0, 
		.Dwx0 = 17, 
		.Dwy0 = 0, 
		.BBw = 15, 
		.BBh = 15, 
		.BBxoff0x = 1, 
		.BByoff0y = -2, 
		.nBytes = 30, 
		.Bitmap = dx13_GlyphBitmap_597D, 
	},
	// 界
	{
		.Name = "754C", 
		.Encoding = 30028, 
		.Swx0 = 941, 
		.Swy0 = 0, 
		.Dwx0 = 17, 
		.Dwy0 = 0, 
		.BBw = 16, 
		.BBh = 15, 
		.BBxoff0x = 0, 
		.BByoff0y = -2, 
		.nBytes = 30, 
		.Bitmap = dx13_GlyphBitmap_754C, 
	},
};

/* 字体文件 */
const Font_TypeDef dx13 =
{
	.SpecVersion = "2.1",
	.FontName = "-FreeType-DengXian-Medium-R-Normal--18-130-100-100-P-159-ISO10646-1",
	.MetricsSet = 0,
	.FontSize = 13,
	.Xres = 100,
	.Yres = 100,
	.FBBx = 23,
	.FBBy = 21,
	.FBBXoff = -2,
	.FBBYoff = -5,
	.nChars = 6,
	.Map = dx13_Map,
	.Glyphs = dx13_Glyphs,
};

#endif
