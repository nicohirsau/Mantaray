#pragma once

#include <cmath>

namespace MR {
class Vector2f {
    public:
        Vector2f() {}
        Vector2f(float x, float y) {
            this->x = x;
            this->y = y;
        }

        bool operator==(Vector2f b) {
            return (this->x == b.x && this->y == b.y);
        }
        Vector2f operator+(Vector2f b) {
            return Vector2f(this->x + b.x, this->y + b.y);
        }
        Vector2f operator-(Vector2f b) {
            return Vector2f(this->x - b.x, this->y - b.y);
        }
        Vector2f operator*(float b) {
            return Vector2f(this->x * b, this->y * b);
        }

        void rotate(float angle) {
            Vector2f intermediate = Vector2f(this->x, this->y);
		    float const Cos(cos(angle));
		    float const Sin(sin(angle));

		    this->x = intermediate.x * Cos - intermediate.y * Sin;
		    this->y = intermediate.x * Sin + intermediate.y * Cos;
        }

        void rotate(float angle, Vector2f center) {
            Vector2f intermediate = Vector2f(
                this->x - center.x, 
                this->y - center.y
            );
		    float const Cos = cos(angle);
		    float const Sin = sin(angle);

		    this->x = intermediate.x * Cos - intermediate.y * Sin;
		    this->y = intermediate.x * Sin + intermediate.y * Cos;
        }

    public:
        float x = 0.0f;
        float y = 0.0f;
};

class Vector2d {
    public:
        Vector2d() {}
        Vector2d(double x, double y) {
            this->x = x;
            this->y = y;
        }

        bool operator==(Vector2d b) {
            return (this->x == b.x && this->y == b.y);
        }
        Vector2d operator+(Vector2d b) {
            return Vector2d(this->x + b.x, this->y + b.y);
        }
        Vector2d operator-(Vector2d b) {
            return Vector2d(this->x - b.x, this->y - b.y);
        }

    public:
        double x = 0.0;
        double y = 0.0;
};

class Vector2i {
    public:
        Vector2i() {}
        Vector2i(int x, int y) {
            this->x = x;
            this->y = y;
        }

    public:
        int x = 0;
        int y = 0;
};

class Vector2u {
    public:
        Vector2u() {}
        Vector2u(unsigned int x, unsigned int y) {
            this->x = x;
            this->y = y;
        }

    public:
        unsigned int x = 0u;
        unsigned int y = 0u;
};

class Vector3f {
    public:
        Vector3f() {}
        Vector3f(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        bool operator==(Vector3f b) {
            return (this->x == b.x && this->y == b.y && this->z == b.z);
        }
        Vector3f operator+(Vector3f b) {
            return Vector3f(this->x + b.x, this->y + b.y, this->z + b.z);
        }
        Vector3f operator-(Vector3f b) {
            return Vector3f(this->x - b.x, this->y - b.y, this->z - b.z);
        }
        Vector3f operator*(float b) {
            return Vector3f(this->x * b, this->y * b, this->z * b);
        }
        Vector3f operator*(Vector3f b) {
            return Vector3f(this->x * b.x, this->y * b.y, this->z * b.z);
        }

        void rotateAroundX(float angle) {
            Vector2f intermediate = Vector2f(this->y, this->z);
		    float const Cos = cos(angle);
		    float const Sin = sin(angle);

		    this->y = intermediate.x * Cos - intermediate.y * Sin;
		    this->z = intermediate.x * Sin + intermediate.y * Cos;
        }

        void rotateAroundY(float angle) {
            Vector2f intermediate = Vector2f(this->x, this->z);
		    float const Cos = cos(angle);
		    float const Sin = sin(angle);

		    this->x =  intermediate.x * Cos + intermediate.y * Sin;
		    this->z = -intermediate.x * Sin + intermediate.y * Cos;
        }

        void rotateAroundZ(float angle) {
            Vector2f intermediate = Vector2f(this->x, this->y);
		    float const Cos = cos(angle);
		    float const Sin = sin(angle);

		    this->x = intermediate.x * Cos - intermediate.y * Sin;
		    this->y = intermediate.x * Sin + intermediate.y * Cos;
        }
    
    public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
};

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
        Vector2<T> operator+(Vector2<T> b) {
            return Vector2<T>(this->x + b.x, this->y + b.y);
        }
        Vector2<T> operator-(Vector2<T> b) {
            return Vector2<T>(this->x - b.x, this->y - b.y);
        }
        Vector2<T> operator*(float b) {
            return Vector2<T>(this->x * b, this->y * b);
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
}