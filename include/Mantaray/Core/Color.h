#pragma once

namespace MR {
struct Color {
    public:
        Color() {}
        Color(unsigned char val) {
            this->r = val;
            this->g = val;
            this->b = val;
            this->a = 0xFF;
        }
        Color(unsigned char r, unsigned char g, unsigned char b) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 0xFF;
        }
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

    public:
        unsigned char r = 0x00;
        unsigned char g = 0x00;
        unsigned char b = 0x00;
        unsigned char a = 0x00;
};
}