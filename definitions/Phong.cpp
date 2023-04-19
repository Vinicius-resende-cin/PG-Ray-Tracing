#ifndef _PHONG_
#define _PHONG_

#include <iostream>

#include "Cena.cpp"
#include "RayCasting.cpp"
#include <cmath>

using namespace std;

float verifyRay(const Luz &l, const Ray &raio)
{
    Vec3 toPoint = l.posicao - raio.origem;

    if (pr_esc(toPoint.normalizar(), raio.direcao.normalizar()) != 1)
        return rayTMax;

    float t = toPoint.comp();

    if (t > EPSILON)
        return t;

    return rayTMax;
}

float verifyRay(const Cena &cena, const Ray &raio)
{
    Intersecao intersecAux = Intersecao(raio);
    Forma *obj;

    for (int i = 0; i < cena.formas.size(); i++)
    {
        obj = cena.formas[i];
        bool intersecao = obj->intersecta(intersecAux);

        if (intersecao && intersecAux.t < rayTMax && intersecAux.t > EPSILON)
        {
            return intersecAux.t;
        }
    }

    return rayTMax;
}

// Função do modelo de iluminação de Phong
Cor Phong(const Cena &cena, Forma *obj, const Vec3 &cameraposicao, const Vec3 &p_intersec)
{
    // Vetor normalizado do ponto do objeto até a posição da câmera
    Vec3 viewDir = cameraposicao - p_intersec;
    viewDir = viewDir.normalizar();

    // Intensidade da luz ambiente
    Cor ambientColor = cena.cor * obj->ka;
    ambientColor.clamp(0, 255);

    // Intensidade total da luz
    Cor I = ambientColor;
    I.clamp(0, 255);

    for (int i = 0; i < cena.luzes.size(); i++)
    {
        // Vetor normalizado que chega da luz
        Vec3 lightDir = cena.luzes[i].posicao - p_intersec;
        lightDir = lightDir.normalizar();

        // projeta sombras
        // Ray lightRay = Ray(p_intersec, lightDir);
        // float lightDist = verifyRay(cena.luzes[i], lightRay);
        // float closestObj = verifyRay(cena, lightRay);

        // if (closestObj < lightDist) // luz obstruída
        //     continue;

        // Vetor normal do objeto
        Vec3 normal = obj->getNormal(p_intersec);

        // Reflexão da luz no objeto
        Vec3 reflectDir = -lightDir.refletir(normal);
        reflectDir = reflectDir.normalizar();

        // Intensidade da luz difusa
        float diffuseFactor = pr_esc(lightDir, normal);
        Cor diffTempColor = pr_hd(cena.luzes[i].cor, obj->cor);
        diffTempColor.clamp(0, 255);
        Cor diffuseColor = diffTempColor * obj->kd * max(diffuseFactor, 0.0f);
        diffuseColor.clamp(0, 255);

        // Intensidade da luz especular
        float specularFactor = pow(pr_esc(reflectDir, viewDir), obj->eta);
        Cor specularColor = cena.luzes[i].cor * obj->ks * specularFactor;
        specularColor.clamp(0, 255);

        I += diffuseColor + specularColor;
        I.clamp(0, 255);
    }

    return I;
}

#endif