#include "Phong.cpp"

Cor rayCasting(Cena cena, const Vec3 &camPos, const Vec3 &camDir, const Vec3 &vUp, float camToS, int hx, int hy, int telaPx, int telaPy, int px, int py)
{
    // Calcula dimensões do pixel
    float alturapixel = hy * 2 / telaPy;
    float largurapixel = hx * 2 / telaPx;

    // Encontra o centro da tela
    Vec3 centroTela = camPos + (camDir * camToS);

    // Calcula os vetores de movimentação
    Vec3 vetorDireita = -Vec3(0, 1, 0) * largurapixel;
    Vec3 vetorBaixo = -Vec3(0, 0, 1) * alturapixel;

    // Encontra o centro do pixel superior esquerdo (0, 0)
    Vec3 cantoSupEsq = centroTela + (Vec3(0, 1, 0) * hx) - (Vec3(0, 0, -1) * hy);
    Vec3 pixelSupEsq = cantoSupEsq + 0.5 * vetorDireita + 0.5 * vetorBaixo;

    // Encontra o pixel (px, py) na tela
    const Vec3 pixelAtual = pixelSupEsq + py * vetorBaixo + px * vetorDireita;

    Ray raioPixelAtual = Ray(camPos, pixelAtual);
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