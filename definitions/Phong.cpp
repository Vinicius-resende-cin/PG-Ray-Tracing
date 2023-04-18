#ifndef _PHONG_
#define _PHONG_

#include <iostream>

#include "Cena.cpp"
#include <cmath>

using namespace std;

// Função do modelo de iluminação de Phong

Cor Phong(Cena cena, Forma *obj, Vec3 cameraposicao, Vec3 p_intersec)
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

        // Vetor normal do objeto
        Vec3 normal = obj->getNormal(p_intersec);

        // Reflexão da luz no objeto
        Vec3 reflectDir = -(lightDir - normal * pr_esc(-lightDir, normal) * 2);
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