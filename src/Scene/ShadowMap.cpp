#include "ShadowMap.h"


#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "../utils.h"

ShadowMap::ShadowMap(int shadow_width, int shadow_height,float near,float far) 
	:FBO(0), m_ShadowHeight(shadow_height), m_ShadowWidth(shadow_width),m_Near(near),m_Far(far)
{
	GLCALL(glGenFramebuffers(1, &FBO));
	GLCALL(glGenTextures(1, &m_DepthMap));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_DepthMap));
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ShadowWidth, m_ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);


	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, FBO));

	GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0));

	GLCALL(glDrawBuffer(GL_NONE));
	GLCALL(glReadBuffer(GL_NONE));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

}


