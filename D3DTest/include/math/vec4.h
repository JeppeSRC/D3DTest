#pragma once


namespace tomato {
	namespace math {

		class vec2;
		class vec3;
		class vec4 {
		public:
			float x;
			float y;
			float z;
			float w;

		public:
			vec4(const vec2& other, float z, float w);
			vec4(const vec3& other, float w);
			vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
			vec4(float x, float y, float z, float w);

			bool operator==(const vec4& v);
			bool operator!=(const vec4& v);

			vec4 operator+(const vec4& v);
			vec4 operator-(const vec4& v);
			vec4 operator*(const vec4& v);
			vec4 operator/(const vec4& v);

			vec4 operator+(const vec3& v);
			vec4 operator-(const vec3& v);
			vec4 operator*(const vec3& v);
			vec4 operator/(const vec3& v);

			vec4 operator+(const vec2& v);
			vec4 operator-(const vec2& v);
			vec4 operator*(const vec2& v);
			vec4 operator/(const vec2& v);

			vec4 operator+(float f);
			vec4 operator-(float f);
			vec4 operator*(float f);
			vec4 operator/(float f);

			void operator+=(const vec4& v);
			void operator-=(const vec4& v);
			void operator*=(const vec4& v);
			void operator/=(const vec4& v);

			void operator+=(const vec3& v);
			void operator-=(const vec3& v);
			void operator*=(const vec3& v);
			void operator/=(const vec3& v);

			void operator+=(const vec2& v);
			void operator-=(const vec2& v);
			void operator*=(const vec2& v);
			void operator/=(const vec2& v);

			void operator+=(float f);
			void operator-=(float f);
			void operator*=(float f);
			void operator/=(float f);
		};

	}
}