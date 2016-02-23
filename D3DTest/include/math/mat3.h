#pragma once

namespace tomato {
	namespace math {

		class vec3;
		class mat4;
		class mat3 {
		public:
			float m[9];

		public:
			mat3();
			mat3(float diagonal);
			mat3(const mat4& m);

			mat3& scale(const vec3& v);
			mat3& rotate(const vec3& v);
		
			mat3 operator*(const mat3& m);
			vec3 operator*(const vec3& v);
		};

	}
}