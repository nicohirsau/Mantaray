#pragma once

#include "Mantaray/Core/Vector.hpp"

namespace MR {
template <class T>
struct Rectangle {
    public:
        Rectangle() { }
        Rectangle(Vector2<T> &inPosition, Vector2<T> &inSize) {
            this->position = Vector2<T>(inPosition.x, inPosition.y);
            this->size = Vector2<T>(inSize.x, inSize.y);
        }
        Rectangle(T x, T y, T width, T height) {
            this->position = Vector2<T>(x, y);
            this->size = Vector2<T>(width, height);
        }

        T x() {
            return this->position.x;
        }
        T y() {
            return this->position.y;
        }

        T width() {
            return this->size.x;
        }
        T height() {
            return this->size.y;
        }

        Vector2<T> position = Vector2<T>(0, 0);
        Vector2<T> size = Vector2<T>(1, 1);
};

typedef Rectangle<int> Rectanglei;
typedef Rectangle<unsigned int> Rectangleu;
typedef Rectangle<float> Rectanglef;
typedef Rectangle<double> Rectangled;
}
