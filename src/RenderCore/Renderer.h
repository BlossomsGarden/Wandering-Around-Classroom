#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include  "Shader.h"

#include "../Scene/SimpleObject/3D/EntityObject3D.h"
#include "TextureCache.h"





class Renderer {
private:

	



public:
    TextureCache m_Textures;

    void UseTextureCache( TextureCache& T);;

    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum mode = GL_TRIANGLES)const ;
    void Draw(const EntityObject3D &Object,Shader&shader,GLenum mode = GL_TRIANGLES)const;
    void Clear(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) const ;
    
};
