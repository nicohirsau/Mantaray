#pragma once

#include <cmath>

namespace MR {
template <class T>
class Vector2 {
    public:
        Vector2() {}
        Vector2(T x, T y) {
            this->x = x;
            this->y = y;
        }

        bool operator==(Vector2<T> b) {
            return (this->x == b.x && this->y == b.y);
        }
        bool operator!=(Vector2<T> b) {
            return (this->x != b.x && this->y != b.y);
        }
        Vector2<T> operator+(Vector2<T> b) {
            return Vector2<T>(this->x + b.x, this->y + b.y);
        }
        Vector2<T> operator+=(Vector2<T> b) {
            return Vector2<T>(this->x + b.x, this->y + b.y);
        }
        Vector2<T> operator-(Vector2<T> b) {
            return Vector2<T>(this->x - b.x, this->y - b.y);
        }
        Vector2<T> operator-=(Vector2<T> b) {
            return Vector2<T>(this->x - b.x, this->y - b.y);
        }
        Vector2<T> operator*(float b) {
            return Vector2<T>(this->x * b, this->y * b);
        }
        Vector2<T> operator*=(Vector2<T> b) {
            return Vector2<T>(this->x * b.x, this->y * b.y);
        }

        void rotate(float angle) {
            Vector2<T> intermediate = Vector2<T>(this->x, this->y);
            float const Cos(cos(angle));
            float const Sin(sin(angle));

            this->x = intermediate.x * Cos - intermediate.y * Sin;
            this->y = intermediate.x * Sin + intermediate.y * Cos;
        }

        void rotate(float angle, Vector2<T> center) {
            Vector2<T> intermediate = Vector2<T>(
                this->x - center.x, 
                this->y - center.y
            );
            float const Cos = cos(angle);
            float const Sin = sin(angle);

            this->x = intermediate.x * Cos - intermediate.y * Sin;
            this->y = intermediate.x * Sin + intermediate.y * Cos;
        }

    public:
        T x = 0.0f;
        T y = 0.0f;
};

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

template <class T>
class Vector3 {
    public:
        Vector3() {}
        Vector3(T x, T y, T z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        bool operator==(Vector3<T> b) {
            return (this->x == b.x && this->y == b.y && this->z == b.z);
        }
        bool operator!=(Vector2<T> b) {
            return (this->x != b.x && this->y != b.y && this->z != b.z);
        }
        Vector3<T> operator+(Vector3<T> b) {
            return Vector3<T>(this->x + b.x, this->y + b.y, this->z + b.z);
        }
        Vector3<T> operator-(Vector3<T> b) {
            return Vector3<T>(this->x - b.x, this->y - b.y, this->z - b.z);
        }
        Vector3<T> operator*(float b) {
            return Vector3<T>(this->x * b, this->y * b, this->z * b);
        }
        Vector3<T> operator*(Vector3<T> b) {
            return Vector3<T>(this->x * b.x, this->y * b.y, this->z * b.z);
        }

        void rotateAroundX(float angle) {
            Vector2<T> intermediate = Vector2<T>(this->y, this->z);
            float const Cos = cos(angle);
            float const Sin = sin(angle);

            this->y = intermediate.x * Cos - intermediate.y * Sin;
            this->z = intermediate.x * Sin + intermediate.y * Cos;
        }

        void rotateAroundY(float angle) {
            Vector2<T> intermediate = Vector2<T>(this->x, this->z);
            float const Cos = cos(angle);
            float const Sin = sin(angle);

            this->x =  intermediate.x * Cos + intermediate.y * Sin;
            this->z = -intermediate.x * Sin + intermediate.y * Cos;
        }

        void rotateAroundZ(float angle) {
            Vector2<T> intermediate = Vector2<T>(this->x, this->y);
            float const Cos = cos(angle);
            float const Sin = sin(angle);

            this->x = intermediate.x * Cos - intermediate.y * Sin;
            this->y = intermediate.x * Sin + intermediate.y * Cos;
        }
    
    public:
        T x = 0.0f;
        T y = 0.0f;
        T z = 0.0f;
};

typedef Vector3<int> Vector3i;
typedef Vector3<unsigned int> Vector3u;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

template <class T>
class Vector4 {
    public:
        Vector4() {}
        Vector4(T x, T y, T z, T w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        bool operator==(Vector4<T> b) {
            return (this->x == b.x && this->y == b.y && this->z == b.z && this->w == b.w);
        }
        Vector4<T> operator+(Vector4<T> b) {
            return Vector4<T>(this->x + b.x, this->y + b.y, this->z + b.z, this->w + b.w);
        }
        Vector4<T> operator-(Vector4<T> b) {
            return Vector4<T>(this->x - b.x, this->y - b.y, this->z - b.z, this->w - b.w);
        }
        Vector4<T> operator*(float b) {
            return Vector4<T>(this->x * b, this->y * b, this->z * b, this->w * b);
        }
        Vector4<T> operator*(Vector4<T> b) {
            return Vector4<T>(this->x * b.x, this->y * b.y, this->z * b.z, this->w * b.w);
        }
    
    public:
        T x = 0.0f;
        T y = 0.0f;
        T z = 0.0f;
        T w = 0.0f;
};

typedef Vector4<int> Vector4i;
typedef Vector4<unsigned int> Vector4u;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
}
