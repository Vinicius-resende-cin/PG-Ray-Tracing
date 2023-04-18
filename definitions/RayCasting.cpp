#ifndef _CASTRAY_
#define _CASTRAY_

#include "Phong.cpp"

Intersecao castRay(Cena cena, const Ray &raioAtual)
{
    Intersecao intersec = Intersecao(raioAtual);
    Intersecao intersecAux = Intersecao(raioAtual);
    Forma *obj;

    for (int i = 0; i < cena.formas.size(); i++)
    {
        obj = cena.formas[i];
        bool intersecao = obj->intersecta(intersecAux);

        if (intersecao && intersecAux.t < intersec.t && intersecAux.t > 1)
        {
            intersec = intersecAux;
        }
    }

    return intersec;
}

#endif