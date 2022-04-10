#pragma once

#pragma once
#include"../Basic/EspWindow.h"

enum class EspImageListFlag
{
	Default,
	Device_Dependent_Bitmap,
	Color_4Bit,
	Color_8Bit,
	Color_16Bit,
	Color_24Bit,
	Color_32Bit
};

class EspImageList
{
	HIMAGELIST imagelist;
public:
	inline const HIMAGELIST& GetHandle()const { return imagelist; }
	bool Create(int width, int height, UINT flag, int init_capacity, int growth)
	{
		return imagelist = ::ImageList_Create(width, height, flag, init_capacity, growth);
	}
	bool Create(int width, int height, EspImageListFlag flag, int init_capacity, int growth)
	{
		UINT flag_uint;
		switch (flag)
		{
		case EspImageListFlag::Default:flag_uint = ILC_COLOR; break;
		case EspImageListFlag::Device_Dependent_Bitmap:flag_uint = ILC_COLORDDB; break;
		case EspImageListFlag::Color_4Bit:flag_uint = ILC_COLOR4; break;
		case EspImageListFlag::Color_8Bit:flag_uint = ILC_COLOR8; break;
		case EspImageListFlag::Color_16Bit:flag_uint = ILC_COLOR16; break;
		case EspImageListFlag::Color_24Bit:flag_uint = ILC_COLOR24; break;
		case EspImageListFlag::Color_32Bit:flag_uint = ILC_COLOR32; break;
		}
		return imagelist = ::ImageList_Create(width, height, flag_uint, init_capacity, growth);
	}
	inline void AddIcon(HICON icon) { ::ImageList_ReplaceIcon(imagelist, -1, icon); }
	inline void ReplaceIcon(unsigned int index, HICON icon) { ::ImageList_ReplaceIcon(imagelist, index, icon); }
	inline void CopyIcon(EspImageList src, int src_index, int dest_index) { ::ImageList_Copy(imagelist, dest_index, src.GetHandle(), src_index, ILCF_MOVE); }
	inline void CopyIcon(HIMAGELIST src, int src_index, int dest_index) { ::ImageList_Copy(imagelist, dest_index, src, src_index, ILCF_MOVE); }
	inline void ExchangeIcon(EspImageList src, int src_index, int dest_index) { ::ImageList_Copy(imagelist, dest_index, src.GetHandle(), src_index, ILCF_SWAP); }
	inline void ExchangeIcon(HIMAGELIST src, int src_index, int dest_index) { ::ImageList_Copy(imagelist, dest_index, src, src_index, ILCF_SWAP); }
	inline void Destroy() { ::ImageList_Destroy(imagelist); }
	inline void CreateDuplicate(HIMAGELIST& newil) { newil = ::ImageList_Duplicate(imagelist); }
	inline COLORREF GetBackgroundColor() { return ::ImageList_GetBkColor(imagelist); }
	inline HICON GetIcon(unsigned int index) { return ::ImageList_GetIcon(imagelist, index, ILD_NORMAL); }
	inline HICON GetTransparentIcon(unsigned int index) { return ::ImageList_GetIcon(imagelist, index, ILD_TRANSPARENT); }
	inline void GetIconSize(unsigned int index, int* width_ptr, int* height_ptr) { ::ImageList_GetIconSize(imagelist, width_ptr, height_ptr); }
	inline int GetCount() { return ::ImageList_GetImageCount(imagelist); }
	inline void RemoveIcon(unsigned int index) { ::ImageList_Remove(imagelist, index); }
	inline void RemoveAllIcons() { ::ImageList_Remove(imagelist, -1); }
	inline void SetBackgroundColor(COLORREF color) { ::ImageList_SetBkColor(imagelist, color); }
	inline void SetIconSize(int width, int height) { ::ImageList_SetIconSize(imagelist, width, height); }
	inline void SetCount(UINT count) { ::ImageList_SetImageCount(imagelist, count); }

	inline ~EspImageList()
	{
		ImageList_Destroy(imagelist);
	}
};