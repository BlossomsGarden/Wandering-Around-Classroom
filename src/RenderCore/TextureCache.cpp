#include "TextureCache.h"
#include <iostream>
void TextureCache::Bind()
{
	if (!m_Bind) {
		for (auto it = m_TexCache.begin(); it != m_TexCache.end(); it++) {
			it->second.Bind(it->first);
		}
	}
	m_Bind = true;
}

void TextureCache::Unbind()
{
	if (m_Bind) {
		for (auto it = m_TexCache.begin(); it != m_TexCache.end(); it++) {
			it->second.Unbind();
		}
	}
	m_Bind = false;
}

void TextureCache::AddTex(const Texture T, unsigned int slot) {
	if (slot < OPENGL_TEXTURE_SLOT_NUM) {
		m_TexCache.insert({ slot,T });
		m_TexNum++;
	}
	else {
		std::cout << "Texture Slot out of Boundary" << std::endl;
	}
}
