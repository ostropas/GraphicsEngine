#pragma once
#include "ResourceManager.h"

class TextureManager: public ResourceManager
{
public:
	TextureManager();
	~TextureManager();
	TexturePtr createTextureFromFile(const wchar_t* file_path);
protected:
	// ������������ ����� ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};

