#pragma once

#include <GL/glew.h>

namespace sparky
{
	namespace graphics
	{
		class Buffer
		{
		private:
			GLuint m_BufferID;
			GLuint m_ComponentCount;  // to interpret the data e.g. whether it is 2 vec2's or 1 vec4
		public:
			Buffer(GLfloat* data, GLsizei count, GLuint componentCount);

			void bind() const;
			void unbind() const;

			inline GLuint getComponentCount() const { return m_ComponentCount; }
		};
	}

}

