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
    Cor cor;
    vector<Forma *> formas{};

    Cena(){};
    Cena(Cor c)
    {
        cor = c;
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