#include <iostream>

#include "Camera.hpp"

Camera::Camera()
    : posicao(0.0f, 0.0f, 0.0f),
      mira(1.0f, 0.0f, 0.0f),
      Vup(0.0f, 0.0f, 1.0f),
      d(1.0f),
      altura(1.0f),
      largura(1.0f)
{
    W = (posicao - mira).normalizar();
    U = pr_vet(W, Vup).normalizar(); // vetor horizontal (esquerda) paralelo à tela
    V = pr_vet(U, W).normalizar();   // vetor vertical (cima) paralelo à tela
}

Camera::Camera(const Vec3 &C, const Vec3 &M,
               const Vec3 &Vup,
               float d, float altura, float largura)
    : posicao(C),
      mira(M),
      Vup(Vup),
      d(d),
      altura(altura),
      largura(largura)
{
    W = (posicao - mira).normalizar();
    U = pr_vet(W, Vup).normalizar(); // vetor horizontal paralelo à tela
    V = pr_vet(U, W).normalizar();   // vetor vertical paralelo à tela
}

Camera::~Camera() {}