#ifndef _TRACERAY_
#define _TRACERAY_

#include "RayCasting.cpp"

Cor traceRay(Cena cena, const Camera &cam, int telaPx, int telaPy, int px, int py)
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
    const Vec3 pixelAtual = centroTela - coordY * vetorCima + coordX * vetorDireita;

    Ray raioPixelAtual = Ray(cam.posicao, pixelAtual);

    Intersecao intersec = castRay(cena, cam, raioPixelAtual, telaPx, telaPy, px, py);

    if (intersec.intersectou())
    {
        Vec3 ponto_intersec = intersec.posicao();
        intersec.cor = Phong(cena, intersec.pForma, intersec.ray.origem, ponto_intersec);
    }
    else
    {
        intersec.cor = cena.cor;
    }

    return intersec.cor;
};

#endif