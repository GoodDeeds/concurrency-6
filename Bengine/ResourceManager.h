#pragma once

#include "TextureCache.h"
#include <string>

namespace Bengine {

	class ResourceManager                       // static class only one instance
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;
	};
}

