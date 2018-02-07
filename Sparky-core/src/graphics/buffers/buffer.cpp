#include "buffer.h"

namespace sparky
{
	namespace graphics
	{
		Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount)
			: m_ComponentCount(componentCount)
		{
			glGenBuffers(1, &m_BufferID);                       // 1 buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);          // this buffer is an array of data
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);                   // unbinding
		}

		void Buffer::bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		}

		void Buffer::unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}
	}
}