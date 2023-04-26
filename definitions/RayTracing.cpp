#ifndef _TRACERAY_
#define _TRACERAY_

#include "RayCasting.cpp"

Ray bounceRay(const Vec3 &ponto_intersec, const Vec3 &normalIntersec, const Ray &intersecRay)
{
    // bounce ray
    Vec3 origemRaioRefletido = ponto_intersec + normalIntersec * 0.01f;       // evita colisão com si próprio
    Vec3 direcaoRaioRefletido = intersecRay.direcao.refletir(normalIntersec); // reflete a direcao do raio
    return Ray(origemRaioRefletido, direcaoRaioRefletido);
}

Ray refractRay(const Vec3 &ponto_intersec, const Vec3 &normalIntersec, const Ray &intersecRay, float refInd, bool &refTot)
{
    float r = 1 / refInd;
    Vec3 normal = normalIntersec;
    if (pr_esc(intersecRay.direcao, normalIntersec) > 0) // se estiver dentro de um objeto
    {
        normal = -normalIntersec; // ajusta a normal
        r = refInd;               // ajusta o coeficiente de refração
    }

    Vec3 origemRaioRefratado = ponto_intersec - normal * 0.01f;          // evita colisões com si mesmo
    Vec3 direcaoRaioRefratado = intersecRay.direcao.refratar(normal, r); // refrata o raio

    if (direcaoRaioRefratado != Vec3(0))
    {
        return Ray(origemRaioRefratado, direcaoRaioRefratado);
    }
    else
    {
        return bounceRay(ponto_intersec, normalIntersec, intersecRay); // executa uma reflexão total
    }
}

Cor static traceRay(const Cena &cena, Ray &raioAtual, int bounces)
{
    Intersecao intersec = Intersecao(raioAtual);
    Cor c = Cor(0);

    castRay(cena, intersec);
    if (intersec.intersectou())
    {
        Vec3 ponto_intersec = intersec.posicao();
        c += Phong(cena, intersec.pForma, intersec.ray.origem, ponto_intersec);
        c.clamp(0, 255);

        if (bounces <= 0)
            return c;

        Vec3 normalIntersec = intersec.pForma->getNormal(ponto_intersec);

        if (intersec.pForma->kr > 0)
        {
            Ray raioRefletido = bounceRay(ponto_intersec, normalIntersec, intersec.ray);
            c += traceRay(cena, raioRefletido, bounces - 1) * intersec.pForma->kr;
            c.clamp(0, 255);
        }

        if (intersec.pForma->refInd > 0 && intersec.pForma->kt > 0)
        {
            bool refTot = false;
            Ray raioRefratado = refractRay(ponto_intersec, normalIntersec, intersec.ray, intersec.pForma->refInd, refTot);

            float k = intersec.pForma->kt;
            if (refTot)
                k = intersec.pForma->kr;

            c += traceRay(cena, raioRefratado, bounces - 1) * k;
            c.clamp(0, 255);
        }

        return c;
    }

    return cena.background;
};

#endif