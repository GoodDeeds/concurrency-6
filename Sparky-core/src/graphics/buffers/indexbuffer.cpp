#include "indexbuffer.h"

namespace sparky
{
	namespace graphics
	{
		IndexBuffer::IndexBuffer(GLushort* data, GLsizei count)
			: m_Count(count)
		{
			glGenBuffers(1, &m_BufferID);								 // 1 buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);          // this buffer is an array of data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);                   // unbinding
		}

		void IndexBuffer::bind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		}

		void IndexBuffer::unbind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}