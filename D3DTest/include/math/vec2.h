#pragma once

namespace tomato { 
	namespace math {


		class vec2 {
		public:
			float x;
			float y;

		public:
			vec2() : x(0.0f), y(0.0f) { }
			vec2(float x, float y);

			float length();
			vec2 normalize();
			vec2& rotate(float angle);
			vec2& translate(const vec2& v);

			float dot(const vec2& v);

			bool operator==(const vec2& v);
			bool operator!=(const vec2& v);

			vec2 operator+(const vec2& v);
			vec2 operator-(const vec2& v);
			vec2 operator*(const vec2& v);
			vec2 operator/(const vec2& v);

			vec2 operator+(float f);
			vec2 operator-(float f);
			vec2 operator*(float f);
			vec2 operator/(float f);

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