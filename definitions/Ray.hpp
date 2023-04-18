#ifndef Ray_h
#define Ray_h

#include "Vetor.cpp"
#include "Cor.cpp"

// raios refletidos nao intersectarem com eles mesmos
#define rayTMin 0.0001f

// distancia infinita valor padrao
#define rayTMax 1.0e30f

struct Ray
{
	Vec3 origem;
	Vec3 direcao;
	float tMax;

	Ray();
	Ray(const Ray &r);
	Ray(const Vec3 &origem, const Vec3 &direcao,
		float tMax = rayTMax);

	virtual ~Ray();

	Ray &operator=(const Ray &r);

	Vec3 calcular(float t) const;
	Vec3 refletir(const Vec3 &normal) const;
};

class Forma;

struct Intersecao
{
	Ray ray;
	float t;
	Forma *pForma;
	Cor cor = Cor(0, 0, 0);

	Intersecao();
	Intersecao(const Intersecao &i);
	Intersecao(const Ray &ray);

	virtual ~Intersecao();

	Intersecao &operator=(const Intersecao &i);

	bool intersectou() const;
	Vec3 posicao() const;
};

#endif