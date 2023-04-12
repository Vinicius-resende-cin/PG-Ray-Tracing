#include <iostream>

#include "Ray.hpp"

using namespace std;

Ray::Ray()
	: origem(0.0f, 0.0f, 0.0f),
	  direcao(),
	  tMax(rayTMax)
{
}

Ray::Ray(const Ray &r)
	: origem(r.origem),
	  direcao(r.direcao),
	  tMax(r.tMax)
{
}

Ray::Ray(const Vec3 &origem, const Vec3 &direcao, float tMax)
	: origem(origem),
	  direcao(direcao),
	  tMax(tMax)
{
}

Ray::~Ray() {}

Ray &Ray::operator=(const Ray &r)
{
	origem = r.origem;
	direcao = r.direcao;
	tMax = r.tMax;
	return *this;
}

Vec3 Ray::calcular(float t) const
{
	return origem + direcao * t;
}

Intersecao::Intersecao()
	: ray(),
	  t(rayTMax),
	  pForma(nullptr)
{
}

Intersecao::Intersecao(const Intersecao &i)
	: ray(i.ray),
	  t(i.t),
	  pForma(i.pForma)
{
}

Intersecao::Intersecao(const Ray &ray)
	: ray(ray),
	  t(ray.tMax),
	  pForma(nullptr)
{
}

Intersecao::~Intersecao() {}

Intersecao &Intersecao::operator=(const Intersecao &i)
{
	ray = i.ray;
	t = i.t;
	pForma = i.pForma;
	return *this;
}

bool Intersecao::intersectou() const
{
	return (pForma != nullptr);
}

Vec3 Intersecao::posicao() const
{
	return ray.calcular(t);
}