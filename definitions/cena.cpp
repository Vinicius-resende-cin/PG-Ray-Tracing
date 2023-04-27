#ifndef _CENA_
#define _CENA_

#include "Camera.cpp"
#include <vector>

struct Luz
{
    Vec3 posicao;
    Cor cor;

    Luz(Vec3 pos, Cor c)
    {
        posicao = pos;
        cor = c;
    }
};

class Cena
{
public:
    vector<Luz> luzes{};
    Cor background;
    Cor globalLight;
    vector<Forma *> formas{};

    Cena(Cor b = Cor(100, 100, 100)) { background = b; };
    Cena(Cor l, Cor b = Cor(100, 100, 100))
    {
        background = b;
        globalLight = l;
    };

    void addLuz(Luz luz)
    {
        luzes.insert(luzes.end(), luz);
    };

    void addForma(Forma *obj)
    {
        formas.insert(formas.end(), obj);
    }
};

#endif