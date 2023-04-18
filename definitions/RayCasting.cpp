#ifndef _CASTRAY_
#define _CASTRAY_

#include "Phong.cpp"

void castRay(Cena cena, Intersecao &intersec)
{
    Intersecao intersecAux = Intersecao(intersec.ray);
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
}

#endif