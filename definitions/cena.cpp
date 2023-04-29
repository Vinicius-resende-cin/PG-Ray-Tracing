#ifndef _CENA_
#define _CENA_

#include "Camera.cpp"
#include <vector>

struct Luz
{
    Vec3 posicao;
    Cor cor;
    bool isPlane;

    Luz(){};

    Luz(Vec3 pos, Cor c, bool p = false)
    {
        posicao = pos;
        cor = c;
        isPlane = p;
    }

    virtual Vec3 getDir(Vec3 ponto) const
    {
        return posicao - ponto;
    }
};

struct PlanoLuz : public Luz
{
    Vec3 normal;
    PlanoLuz(Vec3 pos, Vec3 n, Cor c) : Luz(pos, c, true), normal(n.normalizar()) {}

    Vec3 getDir(Vec3 ponto) const
    {
        Vec3 auxDir = ponto - posicao;
        float dist = pr_esc(auxDir, normal);
        return -normal * dist;
    }
};

class Cena
{
public:
    vector<Luz *> luzes{};
    Cor background;
    Cor globalLight;
    vector<Forma *> formas{};

    Cena(Cor b = Cor(100, 100, 100)) { background = b; };
    Cena(Cor l, Cor b = Cor(100, 100, 100))
    {
        background = b;
        globalLight = l;
    };

    void addLuz(Luz *luz)
    {
        luzes.insert(luzes.end(), luz);
    };

    void addForma(Forma *obj)
    {
        formas.insert(formas.end(), obj);
    }
};

#endif