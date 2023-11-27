#ifndef __Util_h__
#define __Util_h__

#include <string>
#include <random>
#include <stdio.h>


void sleep(float seconds);


enum Direction {
    None = 0b0000,
    Left = 0b0001,
    Right = 0b0010,
    Up = 0b0100,
    Down = 0b1000,
};


template <typename T>
class Range {
public:
    T start;
    T end;
    T step;

public:
    Range(T start, T end, T step=T(1));
};


template <typename T>
T random(const Range<T>& range);


class Float1 {
private:
    float value;  // 0-1

public:
    Float1(float value);

public:
    Float1& operator=(float value);
    Float1& operator+=(float value);
    Float1& operator-=(float value);
    Float1& operator*=(float value);
    Float1& operator/=(float value);
    operator float() const;
};


class ColorRGB {
public:
    Float1 r;
    Float1 g;
    Float1 b;
    Float1 a;

public:
    ColorRGB(float r, float g, float b, float a=1.0f);
    ColorRGB(int r, int g, int b, int a=255);
    ColorRGB(const ColorRGB& color, float a);
    ColorRGB(const ColorRGB& color, int a);
    ColorRGB();

public:
    ColorRGB& operator+=(const ColorRGB& other);    // 합
    ColorRGB& operator|=(const ColorRGB& other);    // 알파값을 이용한 합
    ColorRGB operator+(const ColorRGB& other) const;
    ColorRGB operator|(const ColorRGB& other) const;
    bool operator==(const ColorRGB& other) const;
};

const ColorRGB RGB_Red { 255, 0, 0 };
const ColorRGB RGB_Green { 0, 255, 0 };
const ColorRGB RGB_Blue { 0, 0, 255 };
const ColorRGB RGB_White { 255, 255, 255 };
const ColorRGB RGB_Black { 0, 0, 0 };
const ColorRGB RGB_Yellow { 255, 255, 0 };
const ColorRGB RGB_Cyan { 0, 255, 255 };
const ColorRGB RGB_Magenta { 255, 0, 255 };
const ColorRGB RGB_Gray { 128, 128, 128 };
const ColorRGB RGB_DarkGray { 64, 64, 64 };
const ColorRGB RGB_LightGray { 192, 192, 192 };


class ColorHSV {
public:
    Float1 h;
    Float1 s;
    Float1 v;
    Float1 a;

public:
    ColorHSV(float h, float s, float v, float a=1.0f);
    ColorHSV(int h, float s, float v, float a=1.0f);
    ColorHSV();
};


ColorRGB convertHSVToRGB(const ColorHSV& hsva);
//ColorHSV convertRGBToHSV(const ColorRGB& rgba);

ColorRGB randomColorRGB();
ColorHSV randomColorHSV();


class Log {
public:
    static bool print_log;
    static bool print_error_log;

public:
    static void log(const char* format, ...);
    static void errorLog(const std::string& error);
    static void print(const char* format, ...);
    static void print(const std::string& text);
    static void println(const char* format, ...);
    static void println(const std::string& text);
};

#endif
