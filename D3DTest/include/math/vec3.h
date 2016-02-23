#pragma once


namespace tomato {
	namespace math {

		class vec2;
		class vec3 {
		public:
			float x;
			float y;
			float z;

		public:
			vec3(const vec2& other, float z);
			vec3() : x(0.0f), y(0.0f), z(0.0f) { }
			vec3(float x, float y, float z);

			float length();
			vec3 normalize();
			vec3& rotateX(float angle);
			vec3& rotateY(float angle);
			vec3& rotateZ(float angle);
			vec3& translate(const vec3& v);

			float dot(const vec3& v);
			vec3 cross(const vec3& v);

			bool operator==(const vec3& v);
			bool operator!=(const vec3& v);

			vec3 operator+(const vec3& v);
			vec3 operator-(const vec3& v);
			vec3 operator*(const vec3& v);
			vec3 operator/(const vec3& v);

			vec3 operator+(const vec2& v);
			vec3 operator-(const vec2& v);
			vec3 operator*(const vec2& v);
			vec3 operator/(const vec2& v);

			vec3 operator+(float f);
			vec3 operator-(float f);
			vec3 operator*(float f);
			vec3 operator/(float f);

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