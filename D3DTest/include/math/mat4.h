#pragma once


namespace tomato {
	namespace math {

		class vec3;
		class vec4;
		class mat3;

		class mat4 {
		public:
			float m[16];

		public:
			mat4();
			mat4(float diagonal);
			mat4(const mat3& m);


			mat4& translate(const vec3& v);
			mat4& scale(const vec3& v);
			mat4& rotate(const vec3& v);
			mat4& perspective(float fov, float aspect, float zNear, float zFar);
			mat4& orthographic(float left, float right, float top, float bottom, float zNear, float zFar);

			mat4 operator*(const mat4& m);
			mat4 operator*(const mat3& m);
			vec4 operator*(const vec4& v);
			vec3 operator*(const vec3& v);
		};

	}
}