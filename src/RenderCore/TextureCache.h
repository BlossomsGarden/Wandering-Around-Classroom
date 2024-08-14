#pragma once
#include <unordered_map>
#include "Texture.h"
#include "../Global.h"
//TODO:Fix this cache
class TextureCache {
public:


	void Bind() ;
	void Unbind();
	bool isBound() { return m_Bind; };
	void AddTex(const Texture T, unsigned int slot = 0);;


	TextureCache() :m_TexNum(0), m_Bind(false) { }

	std::unordered_map<unsigned int, Texture> m_TexCache;
private:

	int m_TexNum = 0;
	bool m_Bind = false;

};

