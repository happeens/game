#pragma once

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

struct Color {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    Color(f32 r, f32 g, f32 b, f32 a) :
        r(r), g(g), b(b), a(a) {};
};

struct Position {
    f32 x;
    f32 y;

    Position(f32 x, f32 y) : x(x), y(y) {};
};

struct Size {
    f32 width;
    f32 height;

    Size(f32 width, f32 height) :
        width(width), height(height) {};
};

typedef u16 SpriteId;

