#include <iostream>
#include <complex>
#include <random>
#include <cmath>
#include <stdexcept>
#include <type_traits>

template <typename T>
class Vector {
private:
    size_t size_;
    T* data_;
public:
    static constexpr double EPSILON = 1e-9;

    Vector(size_t size, T fill_value = T()) : size_(size) {
        if (size_ == 0) throw std::invalid_argument("Vector size cannot be zero");
        data_ = new T[size_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = fill_value;
        }
    }

    Vector(size_t size, double min_val, double max_val) : size_(size) {
        if (size_ == 0) throw std::invalid_argument("Vector size cannot be zero");
        if (min_val > max_val) throw std::invalid_argument("Min cannot be greater than max");
        data_ = new T[size_];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(min_val, max_val);
        for (size_t i = 0; i < size_; ++i) {
            if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
                using RealType = typename T::value_type;
                data_[i] = T(static_cast<RealType>(dist(gen)), static_cast<RealType>(dist(gen)));
            }
            else {
                data_[i] = static_cast<T>(dist(gen));
            }
        }
    }

    Vector(const Vector& other) : size_(other.size_) {
        data_ = new T[size_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new T[size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    ~Vector() {
        delete[] data_;
    }
    size_t size() const { return size_; }
    T& operator[](size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < size_; ++i) {
            if (std::abs(data_[i] - other.data_[i]) > EPSILON) return false;
        }
        return true;
    }
    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    Vector operator+(const Vector& other) const {
        if (size_ != other.size_) throw std::invalid_argument("Sizes must match for addition");
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    Vector operator-(const Vector& other) const {
        if (size_ != other.size_) throw std::invalid_argument("Sizes must match for subtraction");
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    T operator*(const Vector& other) const {
        if (size_ != other.size_) throw std::invalid_argument("Sizes must match for dot product");
        T sum = T();
        for (size_t i = 0; i < size_; ++i) {
            if constexpr (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
                sum += std::conj(data_[i]) * other.data_[i];
            }
            else {
                sum += data_[i] * other.data_[i];
            }
        }
        return sum;
    }

    Vector operator*(double scalar) const {
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] * scalar;
        }
        return result;
    }

    friend Vector operator*(double scalar, const Vector& vec) {
        return vec * scalar;
    }

    Vector operator/(double scalar) const {
        if (std::abs(scalar) < EPSILON) throw std::invalid_argument("Division by zero");
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] / scalar;
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        os << "[";
        for (size_t i = 0; i < vec.size_; ++i) {
            os << vec.data_[i];
            if (i < vec.size_ - 1) os << ", ";
        }
        os << "]";
        return os;
    }
};

template <typename T>
Vector<T> find_perpendicular_unit_vector(const Vector<T>& a) {
    if (a.size() < 2) {
        throw std::invalid_argument("Vector must have dimension at least 2 to find a perpendicular vector.");
    }
    size_t n = a.size();
    Vector<T> perp(n, T()); 
    T x = a[0];
    T y = a[1];
    perp[0] = -y;
    perp[1] = x;
    double norm_sq = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double val = std::abs(perp[i]);
        norm_sq += val * val;
    }
    double norm = std::sqrt(norm_sq);
    if (norm < Vector<T>::EPSILON) {
        throw std::runtime_error("Cannot normalize zero vector (input vector had zero length in first two components).");
    }
    return perp / norm;
}

int main() {
    try {
        std::cout << "--- Demo of Vector Class & Task 4 ---" << std::endl;
        Vector<double> a(3, -10.0, 10.0);
        std::cout << "Original Vector a: " << a << std::endl;
        Vector<double> b = find_perpendicular_unit_vector(a);
        std::cout << "Perpendicular Unit Vector b: " << b << std::endl;
        double dot = a * b;
        std::cout << "Dot product (a * b): " << dot << std::endl;
        double len_b = 0;
        for (size_t i = 0; i < b.size(); ++i) {
            double val = std::abs(b[i]);
            len_b += val * val;
        }
        if (std::abs(dot) < Vector<double>::EPSILON) {
            std::cout << "SUCCESS: Vectors are orthogonal." << std::endl;
        }
        else {
            std::cout << "FAIL: Vectors are not orthogonal." << std::endl;
        }
        std::cout << "\n--- Complex Type Test ---" << std::endl;
        Vector<std::complex<double>> c(2, -5.0, 5.0);
        std::cout << "Complex Vector c: " << c << std::endl;
        Vector<std::complex<double>> d = find_perpendicular_unit_vector(c);
        std::cout << "Perp to c: " << d << std::endl;
        std::cout << "Dot (c * d): " << c * d << std::endl; 
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
