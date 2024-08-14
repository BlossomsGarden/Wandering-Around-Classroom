#pragma once
class VertexBuffer
{
	private:
		unsigned int m_RendererID;
	public:
		VertexBuffer() :m_RendererID(0){};
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();


		void Bind()const;
		void Unbind() const;
		void SetData(const void *data,unsigned int size);

};

