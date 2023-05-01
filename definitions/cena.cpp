#ifndef _CENA_
#define _CENA_

#include "Camera.cpp"
#include <vector>

struct Luz
{
    Vec3 posicao;
    Cor cor;
    bool isRet;
    Malha *retangulo;

    Luz(){};

    Luz(Vec3 pos, Cor c, bool p = false, Malha *r = nullptr)
    {
        posicao = pos;
        cor = c;
        isRet = p;
        retangulo = r;
    }

    Vec3 getDir(Vec3 ponto) const
    {
        if (isRet)
        {
            Vec3 normal = retangulo->getGlobalNormal();
            Vec3 auxDir = ponto - posicao;
            float dist = pr_esc(auxDir, normal);
            return -normal * dist;
        }
        return posicao - ponto;
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