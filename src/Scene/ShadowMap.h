#pragma once
class ShadowMap
{
	private:
		unsigned int FBO;
		unsigned int m_DepthMap;
		int m_ShadowWidth, m_ShadowHeight;
	
	public:
		float m_Near, m_Far;
		ShadowMap(int shadow_width,int shadow_height,float near,float far);

		unsigned int GetFBO() { return FBO; };
		unsigned int GetDepthMap(){ return m_DepthMap; };
	
			
};

