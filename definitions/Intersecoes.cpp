#ifndef _INTERSECOES_
#define _INTERSECOES_

#include "Intersecoes.hpp"
#include "Transformacoes.cpp"

using namespace std;

#define EPSILON 0.0001f

vector<vector<float>> I = {{1, 0, 0, 0},
						   {0, 1, 0, 0},
						   {0, 0, 1, 0},
						   {0, 0, 0, 1}};

Plano::Plano(float ka, float kd, float ks, float kr, float eta,
			 const Vec3 &posicao, const Vec3 &normal,
			 const Cor &cor) : Forma(ka, kd, ks, kr, eta, cor),
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

Vec3 Plano::getNormal(Vec3 ponto) { return normal.normalizar(); };

void Plano::transform(vector<vector<float>> t, bool applyOnNormal /*=false*/)
{
	if (applyOnNormal)
		normal = afimTransform(normal, t).normalizar();
	else
		posicao = afimTransform(posicao, t);
}

void Plano::translate(float x, float y, float z)
{
	posicao += Vec3(x, y, z);
};

void Plano::rotate(Vec3 axis, float radAngle)
{
	Vec3 normalAxis = axis.normalizar();

	// Rodrigues rotation
	normal = normalAxis * pr_esc(normalAxis, normal) +
			 (float)cos(radAngle) * pr_vet(pr_vet(normalAxis, normal), normalAxis) +
			 (float)sin(radAngle) * pr_vet(normalAxis, normal);
};

void Plano::applyColor(Cor c)
{
	cor = c;
};

Esfera::Esfera(float ka, float kd, float ks, float kr, float eta,
			   const Vec3 &centro, float raio,
			   const Cor &cor)
	: Forma(ka, kd, ks, kr, eta, cor),
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
	return true;
}

bool Esfera::INTERSECTA(const Ray &ray)
{
	float t0, t1; // solutions for t if the ray intersects

	// transformacao pra considerar o centro da esfera na origem
	Ray localRay = ray;
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
	return true;
}

Vec3 Esfera::getNormal(Vec3 ponto) { return (ponto - centro).normalizar(); };

void Esfera::transform(vector<vector<float>> t)
{
	centro = afimTransform(centro, t);
};

void Esfera::translate(float x, float y, float z)
{
	centro += Vec3(x, y, z);
};

void Esfera::scale(float ratio)
{
	raio *= ratio;
};

void Esfera::applyColor(Cor c)
{
	cor = c;
};

Triangulo::Triangulo(float ka, float kd, float ks, float kr, float eta,
					 const vector<Vec3> &vertices,
					 const Cor &cor)
	: Forma(ka, kd, ks, kr, eta, cor),
	  vertices(vertices)
{
	calculatePosition();
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
	if (a < EPSILON && a > -EPSILON)
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
	if (a < EPSILON && a > -EPSILON)
		return false; // raio paralelo

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
	if (_t > rayTMin && _t < rayTMax) // point intersection
	{
		return true;
	}
	else // line intersection
		return false;
}

Vec3 Triangulo::calculateBaricentro()
{
	// calcula o baricentro atual
	return vertices[0] * 1 / 3 + vertices[1] * 1 / 3 + vertices[2] * 1 / 3;
};

void Triangulo::calculatePosition()
{
	baricentro = calculateBaricentro();

	// calcula a normal atual
	Vec3 edge1 = vertices[1] - vertices[0];
	Vec3 edge2 = vertices[2] - vertices[0];
	normal = pr_vet(edge1, edge2).normalizar();
};

Vec3 Triangulo::getNormal(Vec3 ponto)
{
	calculatePosition();
	return normal.normalizar();
};

void Triangulo::transform(vector<vector<float>> t)
{
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = afimTransform(vertices[i], t);
	}
	calculatePosition();
};

void Triangulo::translate(float x, float y, float z)
{
	for (int i = 0; i < 3; i++)
	{
		vertices[i] += Vec3(x, y, z);
	}
	calculatePosition();
};

void Triangulo::rotate(Vec3 axis, float radAngle)
{
	Vec3 normalAxis = axis.normalizar();

	for (int i = 0; i < 3; i++)
	{
		vertices[i] -= baricentro; // move o triangulo para a origem
		// Rodrigues rotation
		vertices[i] = normalAxis * pr_esc(normalAxis, vertices[i]) +
					  (float)cos(radAngle) * pr_vet(pr_vet(normalAxis, vertices[i]), normalAxis) +
					  (float)sin(radAngle) * pr_vet(normalAxis, vertices[i]);
		vertices[i] += baricentro; // move o triangulo de volta para a posicao original
	}

	calculatePosition();
};

void Triangulo::rotate(Vec3 axis, float radAngle, Vec3 centro)
{
	Vec3 normalAxis = axis.normalizar();

	for (int i = 0; i < 3; i++)
	{
		vertices[i] -= centro; // move o triangulo para a origem
		// Rodrigues rotation
		vertices[i] = normalAxis * pr_esc(normalAxis, vertices[i]) +
					  (float)cos(radAngle) * pr_vet(pr_vet(normalAxis, vertices[i]), normalAxis) +
					  (float)sin(radAngle) * pr_vet(normalAxis, vertices[i]);
		vertices[i] += centro; // move o triangulo de volta para a posicao original
	}

	calculatePosition();
};

void Triangulo::scale(float x, float y, float z){};

void Triangulo::applyColor(Cor c)
{
	cor = c;
};

#endif //_INTERSECOES_