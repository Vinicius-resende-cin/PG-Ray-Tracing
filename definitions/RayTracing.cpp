#ifndef _TRACERAY_
#define _TRACERAY_

#include "RayCasting.cpp"

void bounceRay(Cena cena, Intersecao &intersec, int bounces)
{
    for (int i = 1; i <= bounces; i++)
    {
        Vec3 ponto_intersec = intersec.posicao();
        Vec3 normalIntersec = intersec.pForma->getNormal(ponto_intersec);

        // bounce ray
        intersec.ray.origem = ponto_intersec + normalIntersec * 0.01f;        // evita colisão com si próprio
        intersec.ray.direcao = intersec.ray.direcao.refletir(normalIntersec); // reflete a direcao do raio

        castRay(cena, intersec);

        if (intersec.intersectou())
        {
            Vec3 ponto_intersec = intersec.posicao();
            intersec.cor += Phong(cena, intersec.pForma, intersec.ray.origem, ponto_intersec) * intersec.pForma->kr * (1 / i);
            intersec.cor.clamp(0, 255);
        }
    }
};

void refractRay(Cena cena, Intersecao &intersec, int bounces)
{
    for (int i = bounces; i > 0; i--)
    {
        Vec3 ponto_intersec = intersec.posicao();
        Vec3 normalIntersec = intersec.pForma->getNormal(ponto_intersec);

        if (pr_esc(intersec.ray.direcao, normalIntersec) > 0)
            normalIntersec = -normalIntersec;

        // refract ray
        intersec.ray.origem = ponto_intersec + normalIntersec * 0.01f;                                                // evita colisão com si próprio
        bool refratou = intersec.ray.direcao.refratar(intersec.ray.direcao, normalIntersec, intersec.pForma->refInd); // refrata a direcao do raio

        if (!refratou)
        {
            bounceRay(cena, intersec, i);
            return;
        }

        castRay(cena, intersec);

        if (intersec.intersectou())
        {
            Vec3 ponto_intersec = intersec.posicao();
            intersec.cor += Phong(cena, intersec.pForma, intersec.ray.origem, ponto_intersec) * intersec.pForma->kt * (1 / i);
            intersec.cor.clamp(0, 255);
        }
    }
};

Cor traceRay(const Cena &cena, const Camera &cam, int telaPx, int telaPy, int px, int py, int bounces)
{
    // mapeia as coordenadas do pixel entre -1 e 1
    float coordX = ((float)px / (float)telaPx) * 2.0f - 1.0f;
    float coordY = ((float)py / (float)telaPy) * 2.0f - 1.0f;

    // Encontra o vetor que aponta para o centro da tela
    Vec3 centroTela = -(cam.W * cam.d);

    // Calcula os vetores de localização pela tela
    Vec3 vetorDireita = -cam.U * cam.largura;
    Vec3 vetorCima = cam.V * cam.altura;

    // Encontra o vetor que passa pelo pixel (px, py) na tela
    const Vec3 pixelAtual = (centroTela - coordY * vetorCima + coordX * vetorDireita);

    Ray raioPixelAtual = Ray(cam.posicao, pixelAtual);

    Intersecao intersec = Intersecao(raioPixelAtual);

    castRay(cena, intersec);
    if (intersec.intersectou())
    {
        Vec3 ponto_intersec = intersec.posicao();
        intersec.cor += Phong(cena, intersec.pForma, intersec.ray.origem, ponto_intersec);
        intersec.cor.clamp(0, 255);

        if (intersec.pForma->kr > 0)
            bounceRay(cena, intersec, bounces);

        // if (intersec.pForma->refInd > 0 && intersec.pForma->kt > 0)
        //     refractRay(cena, intersec, bounces);
    }

    return intersec.cor;
};

#endif