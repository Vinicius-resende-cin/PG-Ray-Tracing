#ifndef _CASTRAY_
#define _CASTRAY_

#include "Phong.cpp"

Intersecao castRay(Cena cena, const Camera &cam, const Ray &raioAtual, int telaPx, int telaPy, int px, int py)
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