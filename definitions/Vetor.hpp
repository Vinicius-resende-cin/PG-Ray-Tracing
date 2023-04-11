#ifndef Vetor_h
#define Vetor_h

#include "math.h"
#include <iostream>
#include <algorithm>

using namespace std;

inline float qdd(float n)
{
    return n * n;
}

class Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3();

    Vec3(float x, float y, float z);

    Vec3(float x);

    Vec3(Vec3 const &v);

    ~Vec3();

    float comp() const; // comprimento

    float comp2() const; // comprimento

    Vec3 normalizar() const; // normalização de vetores

    Vec3 neg() const; // negativo

    Vec3 &operator+=(const Vec3 &v);
    Vec3 &operator-=(const Vec3 &v);
    Vec3 &operator*=(float f);
    Vec3 &operator/=(float f);
    Vec3 operator-() const;

    void print()
    {
        cout << x << '/' << y << '/' << z << endl;
    }
};
// produto escalar
float pr_esc(Vec3 v1, Vec3 v2);

// produto vetorial
Vec3 pr_vet(Vec3 v1, Vec3 v2);

// soma de vetores
inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
// subtraçao de vetores
inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
// multiplicaçao errada caso precise
inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
// divisao por constante
inline Vec3 operator/(const Vec3 &v1, float const &f)
{
    return Vec3(v1.x / f, v1.y / f, v1.z / f);
}
// multiplicaçao por constante
inline Vec3 operator*(const Vec3 &v1, float const &f)
{
    return Vec3(v1.x * f, v1.y * f, v1.z * f);
}

#endif