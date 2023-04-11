#include "Phong.cpp"

Cor rayCasting(Cena cena, const Camera &cam, int telaPx, int telaPy, int px, int py)
{
    // mapeia as coordenadas do pixel entre -1 e 1
    float coordX = ((float)px / (float)telaPx) * 2.0f - 1.0f;
    float coordY = ((float)py / (float)telaPy) * 2.0f - 1.0f;

    // Calcula dimensões do pixel
    float alturapixel = cam.altura * 2 / telaPy;
    float largurapixel = cam.largura * 2 / telaPx;

    // Encontra o vetor que aponta para o centro da tela
    Vec3 centroTela = -(cam.W * cam.d);

    // Calcula os vetores de localização pela tela
    Vec3 vetorDireita = -cam.U * cam.largura;
    Vec3 vetorCima = cam.V * cam.altura;

    // Encontra o vetor que passa pelo pixel (px, py) na tela
    const Vec3 pixelAtual = centroTela - coordY * vetorCima + coordX * vetorDireita;

    Ray raioPixelAtual = Ray(cam.posicao, pixelAtual);
    Intersecao intersec = Intersecao(raioPixelAtual);

    for (int i = 0; i < cena.esferas.size(); i++)
    {
        Esfera e = cena.esferas[i];
        Intersecao intersecAux = Intersecao(raioPixelAtual);
        bool intersecao = e.intersecta(intersecAux);

        if (intersecao && intersecAux.t < intersec.t && intersecAux.t > 1)
        {
            intersec = intersecAux;
            Vec3 ponto_intersec = intersec.posicao();
            return Phong(cena, e, intersec.ray.origem, ponto_intersec, e.ka, e.kd, e.ks, e.eta);
        }
    }
    return cena.cor;
}