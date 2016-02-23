#pragma once
#include "string.h"

namespace tomato {
	namespace util {

		void load_text_file(const String& filename, String& string);
		void load_text_file(const String& filename, char** string);
	}
}

