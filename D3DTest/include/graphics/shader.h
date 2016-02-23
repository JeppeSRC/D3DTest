#pragma once
#include "../util/map.h"
#include "../util/string.h"
#include "../../include/math/math.h"

namespace tomato {
	namespace graphics {

		class Shader {
		private:
			util::map<util::String, unsigned int> uniforms;

			unsigned int shader_id;

		public:
			Shader(const util::String& vertex_shader, const util::String& fragment_shader, const util::String& geometry_shader, bool src = false);
			~Shader();

			unsigned int getUniformLocation(const util::String& name);

			void setInt(const char* name, int v);
			void setUInt(const char* name, unsigned int v);
			void setFloat(const char* name, float v);
			void setDouble(const char* name, double v);
			void setVec2(const char* name, const math::vec2& v);
			void setVec3(const char* name, const math::vec3& v);
			void setVec4(const char* name, const math::vec4& v);
			void setMat3(const char* name, const math::mat3& v);
			void setMat4(const char* name, const math::mat4& v);

			void bind();
			
			static void unbind();
		};
	}
}