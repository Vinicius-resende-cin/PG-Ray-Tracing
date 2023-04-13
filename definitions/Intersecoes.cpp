#ifndef _INTERSECOES_
#define _INTERSECOES_

#include "Intersecoes.hpp"
using namespace std;

#define EPSILON 0.000001f

Plano::Plano(float ka, float kd, float ks, float eta,
			 const Vec3 &posicao, const Vec3 &normal,
			 const Cor &cor) : Forma(ka, kd, ks, eta, cor),
							   posicao(posicao),
							   normal(normal.normalizar())
{
}

Plano::~Plano() {}

bool Plano::intersecta(Intersecao &intersecao)
{
	// verifica se intersecta
	float prodEscDirNor = pr_esc(intersecao.ray.direcao, normal);
	if (prodEscDirNor > -EPSILON && prodEscDirNor < EPSILON)
	{
		return false;
	}

	// acha ponto de intersecao
	float t = pr_esc(posicao - intersecao.ray.origem, normal) / prodEscDirNor;
	if (t <= rayTMin || t >= intersecao.t)
	{
		return false;
	}
	intersecao.t = t;
	intersecao.pForma = this;
	intersecao.cor = cor;
	return true;
}

bool Plano::INTERSECTA(const Ray &ray)
{
	// verifica se intersecta
	float prodEscDirNor = pr_esc(ray.direcao, normal);
	if (prodEscDirNor > -EPSILON && prodEscDirNor < EPSILON)
	{
		return false; // raio paralelo
	}

	// acha o ponto de intersecao
	float t = pr_esc(posicao - ray.origem, normal) / prodEscDirNor;
	if (t <= rayTMin || t >= ray.tMax)
	{
		return false;
	}
	return true;
}

Vec3 Plano::getNormal(Vec3 ponto) { return normal; };

Esfera::Esfera(float ka, float kd, float ks, float eta,
			   const Vec3 &centro, float raio,
			   const Cor &cor)
	: Forma(ka, kd, ks, eta, cor),
	  centro(centro),
	  raio(raio)
{
}

Esfera::~Esfera() {}

bool Esfera::intersecta(Intersecao &intersecao)
{
	float t0, t1; // solutions for t if the ray intersects

	// transformacao pra considerar o centro da esfera na origem
	Ray localRay = intersecao.ray;
	localRay.origem -= centro;
	// calcula coefcientes
	float a = localRay.direcao.comp2();
	float b = 2 * pr_esc(localRay.direcao, localRay.origem);
	float c = localRay.origem.comp2() - qdd(raio);

	float discr = qdd(b) - 4 * a * c;
	if (discr < 0)
		return false;
	else if (discr == 0)
		t0 = t1 = -0.5 * b / a;
	else
	{
		float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
		t0 = q / a;
		t1 = c / q;
	}
	if (t0 > t1)
		swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead
		if (t0 < 0)
			return false; // both t0 and t1 are negative
	}

	intersecao.t = t0;
	intersecao.pForma = this;
	intersecao.cor = cor;
	return true;
}

bool Esfera::INTERSECTA(const Ray &ray)
{
	// transformacao pra considerar o centro da esfera na origem
	Ray localRay = ray;
	localRay.origem -= centro;
	// calcula coefcientes
	float a = localRay.direcao.comp2();
	float b = 2 * pr_esc(localRay.direcao, localRay.origem);
	float c = localRay.origem.comp2() - qdd(raio);
	// verifica se intersecta
	float discriminante = qdd(b) - 4 * a * c;
	if (discriminante < 0.0f)
	{
		return false;
	}
	// dois pontos de intersecao t1 perto t2 longe
	float t1 = (-b - sqrt(discriminante)) / (2 * a);
	if (t1 > rayTMin && t1 < ray.tMax)
		return true;
	float t2 = (-b + sqrt(discriminante)) / (2 * a);
	if (t2 > rayTMin && t2 < ray.tMax)
		return true;
	return false;
}

Vec3 Esfera::getNormal(Vec3 ponto) { return (ponto - centro).normalizar(); };

Triangulo::Triangulo(float ka, float kd, float ks, float eta,
					 const vector<Vec3> &vertices, const Vec3 &normal,
					 const Cor &cor)
	: Forma(ka, kd, ks, eta, cor),
	  vertices(vertices),
	  normal(normal)
{
}

Triangulo::~Triangulo() {}

bool Triangulo::intersecta(Intersecao &intersecao)
{
	// triangle edge vectors

	Vec3 edge1 = vertices[1] - vertices[0];
	Vec3 edge2 = vertices[2] - vertices[0];
	Vec3 h = pr_vet(intersecao.ray.direcao, edge2);
	// h = h.normalizar();

	// check if ray is parallel

	float a = pr_esc(edge1, h);
	if (a < EPSILON)
		return false; // raio paralelo

	// find beta coordinate

	float f = 1.0 / a;
	Vec3 s = Vec3(intersecao.ray.origem) - Vec3(vertices[0]);
	float beta = f * pr_esc(s, h);
	if (beta < 0.0 || beta > 1.0)
		return false; // outside triangle

	// find gamma coordinate

	Vec3 q = pr_vet(s, edge1);
	float gamma = f * pr_esc(intersecao.ray.direcao, q);
	if (gamma < 0.0 || beta + gamma > 1.0)
		return false; // outside triangle

	// find the intersection point

	float _t = f * pr_esc(edge2, q);
	if (_t > rayTMin && _t < intersecao.t) // point intersection
	{
		intersecao.t = _t;
		intersecao.pForma = this;
		intersecao.cor = cor;
		return true;
	}
	else // line intersection
		return false;
}

bool Triangulo::INTERSECTA(const Ray &ray)
{
	// triangle edge vectors

	Vec3 edge1 = vertices[1] - vertices[0];
	Vec3 edge2 = vertices[2] - vertices[0];
	Vec3 h = pr_vet(ray.direcao, edge2);
	// h = h.normalizar();

	// check if ray is parallel

	float a = pr_esc(edge1, h);
	if (a < EPSILON)
		return false; // ray is parallel

	// find beta coordinate

	float f = 1.0 / a;
	Vec3 s = Vec3(ray.origem) - Vec3(vertices[0]);
	float beta = f * pr_esc(s, h);
	if (beta < 0.0 || beta > 1.0)
		return false; // outside triangle

	// find gamma coordinate

	Vec3 q = pr_vet(s, edge1);
	float gamma = f * pr_esc(ray.direcao, q);
	if (gamma < 0.0 || beta + gamma > 1.0)
		return false; // outside triangle

	// find the intersection point

	float _t = f * pr_esc(edge2, q);
	if (_t > rayTMin && _t < ray.tMax) // point intersection
	{
		return true;
	}
	else // line intersection
	{
		return false;
	}
}

Vec3 Triangulo::getNormal(Vec3 ponto) { return normal; };

#endif //_INTERSECOES_