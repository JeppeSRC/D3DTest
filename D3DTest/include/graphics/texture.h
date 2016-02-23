#pragma once

namespace tomato {
	namespace graphics {

		class Texture {
		protected:
			unsigned int width;
			unsigned int height;

			unsigned int texture_id;
		public:
			Texture();
			Texture(unsigned int width, unsigned int height);
			virtual ~Texture();

			virtual void bind() = 0;
			virtual void bind(unsigned int active) { }
			virtual void unbind() = 0;
			virtual void unbind(unsigned int active) { }

			inline unsigned int getWidth() { return width; }
			inline unsigned int getHeight() { return height; }

		};


	}
}
