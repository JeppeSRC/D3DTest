#pragma once
#include "texture.h"
#include "../util/string.h"

namespace tomato {
	namespace graphics {

		class Texture2D : public Texture {
		public:
			Texture2D(const util::String& filename);

			void bind() override;
			void bind(unsigned int active) override;
			void unbind() override;
			void unbind(unsigned int active) override;
		};
	}
}
