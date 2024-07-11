// a simple representation of a vector type with n elements



#include <cstddef>
#include <initializer_list>
#include <stdexcept>

template <typename T, std::size_t N>

class VectorN
{
private:
		T data[N];

public:
		// constructor
		VectorN() {
			for (std::size_t i = 0; i < N; i++) {
				data[i] = T();
			}
		}

		VectorN(const T* _data) {
			for (std::size_t i = 0; i < N; i++) {
				data[i] = _data[i];
			}
		}

		// constructor from single value
		VectorN(const T& value) {
			for (std::size_t i = 0; i < N; i++) {
				data[i] = value;
			}
		}

		// make constructor from initializer list with N elements
		VectorN(std::initializer_list<T> list) {
			if (list.size() != N) {
				throw std::runtime_error("initializer list has wrong size");
			}
			std::size_t i = 0;
			for (const T& value : list) {
				data[i] = value;
				i++;
			}
		}

		// copy constructor
		VectorN(const VectorN& other) {
			for (std::size_t i = 0; i < N; i++) {
				data[i] = other.data[i];
			}
		}

		// assignment operator
		VectorN& operator=(const VectorN& other) {
			for (std::size_t i = 0; i < N; i++) {
				data[i] = other.data[i];
			}
			return *this;
		}

		// destructor
		~VectorN() = default;

		// access operator
		T& operator[](const std::size_t i) {
			return data[i];
		}

		const T& operator[](const std::size_t i) const {
			return data[i];
		}

		// get pointer to data
		T* get_data() {
			return data;
		}

		const T* get_data() const {
			return data;
		}

		// get size of vector
		[[nodiscard]] std::size_t size() const {
			return N;
		}

		// arithmetic operators
		VectorN operator+(const VectorN& other) const {
			VectorN result;
			for (std::size_t i = 0; i < N; i++) {
				result.data[i] = data[i] + other.data[i];
			}
			return result;
		}

		VectorN operator-(const VectorN& other) const {
			VectorN result;
			for (std::size_t i = 0; i < N; i++) {
				result.data[i] = data[i] - other.data[i];
			}
			return result;
		}

		VectorN operator*(const T& scalar) const {
			VectorN result;
			for (std::size_t i = 0; i < N; i++) {
				result.data[i] = data[i] * scalar;
			}
			return result;
		}

		VectorN operator/(const T& scalar) const {
			VectorN result;
			for (std::size_t i = 0; i < N; i++) {
				result.data[i] = data[i] / scalar;
			}
			return result;
		}

		// comparison operators
		bool operator==(const VectorN& other) const {
			for (std::size_t i = 0; i < N; i++) {
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}

		bool operator!=(const VectorN& other) const {
			return !(*this == other);
		}
};

typedef VectorN<float, 4> Float4;