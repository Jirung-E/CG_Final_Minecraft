#include "Util.h"

#include <iostream>
#include <cmath>
#include <random>
#include <cstdarg>
#include <thread>
#include <chrono>

using namespace std;


void sleep(float seconds) {
    this_thread::sleep_for(chrono::milliseconds(int(seconds * 1000)));
}


Range<float>::Range(float start, float end, float step): start { start }, end { end }, step { step } {

}

Range<int>::Range(int start, int end, int step) : start { start }, end { end }, step { step } {

}


static std::random_device rd;
static std::mt19937 gen(rd());

//template<typename T>
template<>
float random(const Range<float>& range) {
    int cnt = int((range.end - range.start) / range.step);
    uniform_int_distribution<> dist(0, cnt);
    float result = range.start + range.step*dist(gen);
    return result > range.end ? range.end : result;
}

template<>
int random(const Range<int>& range) {
    int cnt = int((range.end - range.start) / range.step);
    uniform_int_distribution<> dist(0, cnt);
    return range.start + range.step*dist(gen);
}


Float1::Float1(float value) {
    *this = value;
}

Float1& Float1::operator=(float value) {
    if(value > 1.0f) {
        this->value = 1.0f;
    } else if(value < 0.0f) {
        this->value = 0.0f;
    } else {
        this->value = value;
    }
    return *this;
}

Float1& Float1::operator+=(float value) {
    return operator=(this->value + value);
}

Float1& Float1::operator-=(float value) {
    return operator=(this->value - value);
}

Float1& Float1::operator*=(float value) {
    return operator=(this->value * value);
}

Float1& Float1::operator/=(float value) {
    return operator=(this->value / value);
}

Float1::operator float() const {
    return value;
}


ColorRGB::ColorRGB(float r, float g, float b, float a):
    r { Float1 { r } }, g { Float1 { g } }, b { Float1 { b } }, a { Float1 { a } } {

}

ColorRGB::ColorRGB(int r, int g, int b, int a):
    r { Float1 { r / 255.0f } }, g { Float1 { g / 255.0f } }, b { Float1 { b / 255.0f } }, a { Float1 { a / 255.0f } } {


}

ColorRGB::ColorRGB(const ColorRGB& color, float a):
    ColorRGB { color.r, color.g, color.b, a } {

}

ColorRGB::ColorRGB(const ColorRGB& color, int a):
    ColorRGB { color.r, color.g, color.b, Float1 { a / 255.0f } } {

}

ColorRGB::ColorRGB(): ColorRGB { 0, 0, 0 } {

}


ColorRGB& ColorRGB::operator+=(const ColorRGB& other) {
    r = Float1(r + other.r);
    g = Float1(g + other.g);
    b = Float1(b + other.b);
    a = Float1(a + other.a);
    return *this;
}

ColorRGB& ColorRGB::operator|=(const ColorRGB& other) {
    r = r * (1 - other.a) + other.r * other.a;
    g = g * (1 - other.a) + other.g * other.a;
    b = b * (1 - other.a) + other.b * other.a;
    return *this;
}

ColorRGB ColorRGB::operator+(const ColorRGB& other) const {
    ColorRGB new_color = *this;
    new_color += other;
    return new_color;
}

ColorRGB ColorRGB::operator|(const ColorRGB& other) const {
    ColorRGB new_color = *this;
    new_color |= other;
    return new_color;
}

bool ColorRGB::operator==(const ColorRGB& other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
}


ColorHSV::ColorHSV(float h, float s, float v, float a):
    h { Float1 { h } }, s { Float1 { s } }, v { Float1 { v } }, a { Float1 { a } } {

}

// h: 0~360
ColorHSV::ColorHSV(int h, float s, float v, float a): ColorHSV { float((h >= 0 ? h : h+360) % 360) / 360.0f, s, v, a } {

}

ColorHSV::ColorHSV() : ColorHSV { 0.0f, 1.0f, 1.0f } {

}


ColorRGB convertHSVToRGB(const ColorHSV& hsv) {
    float angle = hsv.h * 360;
    float c = hsv.v * hsv.s;
    float x = c * (1 - abs(fmodf(hsv.h * 6, 2) - 1));
    float m = hsv.v - c;
    if(0 <= angle && angle < 60) {
        return ColorRGB(c + m, x + m, m, hsv.a);
    } else if(60 <= angle && angle < 120) {
        return ColorRGB(x+m, c+m, m, hsv.a);
    } else if(120 <= angle && angle < 180) {
        return ColorRGB(m, c+m, x+m, hsv.a);
    } else if(180 <= angle && angle < 240) {
        return ColorRGB(m, x+m, c+m, hsv.a);
    } else if(240 <= angle && angle < 300) {
        return ColorRGB(x+m, m, c+m, hsv.a);
    } else if(300 <= angle && angle <= 360) {
        return ColorRGB(c+m, m, x+m, hsv.a);
    }

    return ColorRGB(0.0f, 0.0f, 0.0f, hsv.a);
}

//ColorHSV convertRGBToHSV(const ColorRGB& rgb) {
//    return ColorHSV();
//}


ColorRGB randomColorRGB() {
    uniform_real_distribution<float> hue(0.0f, 1.0f);
    uniform_real_distribution<float> sat(0.0f, 1.0f);
    ColorHSV c { hue(gen), sat(gen), 1.0f };
    return convertHSVToRGB(c);
}

ColorHSV randomColorHSV() {
    uniform_real_distribution<float> hue(0.0f, 1.0f);
    uniform_real_distribution<float> sat(0.5f, 1.0f);
    return { hue(gen), sat(gen), 1.0f };
}


bool Log::print_log = true;
bool Log::print_error_log = true;

void Log::log(const char* format, ...) {
    if(!print_log) {
        return;
    }
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    putchar('\n');
    va_end(args);
}

void Log::errorLog(const string& error) {
    if(!print_error_log) {
        return;
    }
    cerr << error << endl;
}

void Log::print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void Log::print(const std::string& text) {
	printf(text.c_str());
}

void Log::println(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    putchar('\n');
    va_end(args);
}

void Log::println(const std::string& text) {
    println(text.c_str());
}
