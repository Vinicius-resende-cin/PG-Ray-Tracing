#ifndef _PHONG_
#define _PHONG_

#include <iostream>

#include "Cena.cpp"
#include "RayCasting.cpp"
#include <cmath>

using namespace std;

float verifyRay(const Cena &cena, const Ray &raio)
{
    // verifica se há algum objeto entre a interseção e a luz e retorna o kt do objeto
    Intersecao intersecAux = Intersecao(raio);
    Forma *obj;

    for (int i = 0; i < cena.formas.size(); i++)
    {
        obj = cena.formas[i];
        bool intersecao = obj->intersecta(intersecAux);

        if (intersecao && intersecAux.t < rayTMax && intersecAux.t > EPSILON && intersecAux.t < 1)
        {
            return intersecAux.pForma->kt;
        }
    }

    return 1;
}

// Função do modelo de iluminação de Phong
Cor Phong(const Cena &cena, Forma *obj, const Vec3 &cameraposicao, const Vec3 &p_intersec)
{
    // Vetor normalizado do ponto do objeto até a posição da câmera
    Vec3 viewDir = cameraposicao - p_intersec;
    viewDir = viewDir.normalizar();

    // Intensidade da luz ambiente
    Cor ambientColor = cena.globalLight * obj->ka;
    ambientColor.clamp(0, 255);

    // Intensidade total da luz
    Cor I = ambientColor;
    I.clamp(0, 255);

    for (int i = 0; i < cena.luzes.size(); i++)
    {
        // Vetor em direção à luz
        Vec3 lightDir = cena.luzes[i]->posicao - p_intersec;

        // Vetor normal do objeto
        Vec3 normal = obj->getNormal(p_intersec);

        // projeta sombras
        Ray lightRay = Ray(p_intersec + normal * 0.01f, lightDir);
        float shadowK = verifyRay(cena, lightRay);

        lightDir = lightDir.normalizar(); // normaliza a luz

        // Reflexão da luz no objeto
        Vec3 reflectDir = -lightDir.refletir(normal);
        reflectDir = reflectDir.normalizar();

        // Intensidade da luz difusa
        float diffuseFactor = pr_esc(lightDir, normal);
        Cor diffTempColor = pr_hd(cena.luzes[i]->cor, obj->cor);
        diffTempColor.clamp(0, 255);
        Cor diffuseColor = diffTempColor * obj->kd * max(diffuseFactor, 0.0f);
        diffuseColor.clamp(0, 255);

        // Intensidade da luz especular
        float specularFactor = pow(pr_esc(reflectDir, viewDir), obj->eta);
        Cor specularColor = cena.luzes[i]->cor * obj->ks * specularFactor;
        specularColor.clamp(0, 255);

        I += (diffuseColor + specularColor) * shadowK; // atenua a cor pela sombra projetada
        I.clamp(0, 255);
    }

    return I;
}

#endif