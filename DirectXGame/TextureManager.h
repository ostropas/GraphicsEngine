#pragma once
#include "Rect.h"
#include "ResourceManager.h"
#include "Texture.h"

class TextureManager: public ResourceManager
{
public:
	TextureManager();
	~TextureManager();
	TexturePtr createTextureFromFile(const wchar_t* file_path);
	TexturePtr createTexture(const Rect& size, Texture::Type type);
protected:
	// ������������ ����� ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};

