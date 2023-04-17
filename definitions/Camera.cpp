#ifndef _CAMERA_
#define _CAMERA_

#include <iostream>

#include "Camera.hpp"

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
  U = pr_vet(W, Vup).normalizar(); // vetor horizontal (esquerda) paralelo à tela
  V = pr_vet(U, W).normalizar();   // vetor vertical (cima) paralelo à tela
}

Camera::~Camera() {}

#endif