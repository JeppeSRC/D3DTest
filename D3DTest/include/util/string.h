#pragma once

namespace tomato {
	namespace util {

		class String {
		public:
			char* str;
			size_t length;

		public:
			String() { str = nullptr; length = 0; }
			String(const char* string);
			String(const char* stirng, size_t length);
			String(const String& string);
			String(String&& string);
			~String();

			String& operator=(const String& string);
			String& operator=(String&& string);

			String& operator<<(const String& string);

			bool operator==(const String& string);
			bool operator!=(const String& string);

		};

	}
}
