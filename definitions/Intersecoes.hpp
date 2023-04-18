#ifndef Intersecoes_h
#define Intersecoes_h

#include <iostream>
#include <vector>
#include "Ray.cpp"

using namespace std;

// interseções de planos e esferas apenas

class Forma
{
public:
	float ka, kd, ks, eta;
	Cor cor;

public:
	Forma(float ka, float kd, float ks, float eta, Cor cor)
		: ka(ka), kd(kd), ks(ks), eta(eta), cor(cor){};
	virtual ~Forma() {}

	virtual bool intersecta(Intersecao &intersecao) = 0;
	virtual bool INTERSECTA(const Ray &ray) = 0;

	virtual Vec3 getNormal(Vec3 ponto) = 0;

	virtual void applyColor(Cor c) = 0;
};

class Plano : public Forma
{
public:
	Vec3 posicao;
	Vec3 normal;

public:
	Plano(float ka, float kd, float ks, float eta,
		  const Vec3 &posicao, const Vec3 &normal,
		  const Cor &cor = Cor(1.0f, 1.0f, 1.0f));

	~Plano();

	bool intersecta(Intersecao &intersecao);
	bool INTERSECTA(const Ray &ray);

	Vec3 getNormal(Vec3 ponto);

	void transform(vector<vector<float>> t, bool applyOnNormal = false);
	void translate(float x, float y, float z);
	void rotate(Vec3 axis, float radAngle);

	void applyColor(Cor c);
};

class Esfera : public Forma
{
public:
	Vec3 centro;
	float raio;

public:
	Esfera(float ka, float kd, float ks, float eta,
		   const Vec3 &centro, float raio,
		   const Cor &cor = Cor(1.0f, 1.0f, 1.0f));

	~Esfera();

	bool intersecta(Intersecao &intersecao);
	bool INTERSECTA(const Ray &ray);

	Vec3 getNormal(Vec3 ponto);

	void transform(vector<vector<float>> t);
	void translate(float x, float y, float z);
	void scale(float ratio);

	void applyColor(Cor c);
};

class Triangulo : public Forma
{
public:
	vector<Vec3> vertices;
	Vec3 normal;
	Vec3 baricentro;

private:
	Vec3 calculateNormal();

public:
	Triangulo(float ka, float kd, float ks, float eta,
			  const vector<Vec3> &vertices,
			  const Cor &cor = Cor(1.0f, 1.0f, 1.0f));

	~Triangulo();

	bool intersecta(Intersecao &intersecao);
	bool INTERSECTA(const Ray &ray);

	Vec3 getNormal(Vec3 ponto);

	void transform(vector<vector<float>> t);
	void translate(float x, float y, float z);
	void rotate(Vec3 axis, float radAngle);
	void scale(float x, float y, float z);

	void applyColor(Cor c);
};

// class TriangleMesh : public Forma
// {
// public:
// 	vector<Vec3> vertices;
// 	vector<int> triangleVertices;
// 	vector<Triangulo *> triangulos;
// 	Vec3 centroide;

// public:
// 	TriangleMesh(vector<Vec3> v);

// 	~TriangleMesh();

// 	bool intersecta(Intersecao &intersecao);
// 	bool INTERSECTA(const Ray &ray);

// 	Vec3 getNormal(Vec3 ponto);
// 	void resetTriangles();

// 	void transform(vector<vector<float>> t, int type);
// 	void translate(float x, float y, float z);
// 	void rotate(Vec3 axis, float angle);
// 	void scale(float x, float y, float z);

// 	void addVertice(Vec3 v);
// 	void addTriangle(int v1, int v2, int v3);

// 	void applyColor(Cor c);
// };

#endif