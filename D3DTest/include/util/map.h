#pragma once

namespace tomato {
	namespace util {

		template<typename K, typename D>
		class map {
		private:
			D* data;
			K* keys;

			int items;
			int size;

		public:
			map() { data = nullptr; keys = nullptr; items = 0; size = 0; }
			map(unsigned int size) {
				this->size = size;
				items = 0;

				data = new D[size];
				keys = new K[size];
			}

			~map() {
				delete[] data, keys;
			}

			inline void add(D item, K key) {
				if (items < size) {
					data[items] = item;
					keys[items] = key;

					items++;
				} else {
					//TODO: error
				}
			}

			inline D retrieve(K key) {
				for (int i = 0; i < items; i++) {
					if (keys[i] == key) return data[i];
				}

				return (D)-1;
			}
		};
	}
}
