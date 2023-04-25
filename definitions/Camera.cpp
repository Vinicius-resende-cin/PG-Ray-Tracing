#ifndef _CAMERA_
#define _CAMERA_

#include <iostream>

#include "Camera.hpp"

Camera::Camera(){};

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
  calculateBase();
}

Camera::~Camera() {}

void Camera::calculateBase()
{
  W = (posicao - mira).normalizar();
  U = pr_vet(W, Vup).normalizar(); // vetor horizontal (esquerda) paralelo à tela
  V = pr_vet(U, W).normalizar();   // vetor vertical (cima) paralelo à tela
};

void Camera::transform(vector<vector<float>> t)
{
  W = afimTransform(W, t);
  U = afimTransform(U, t);
  V = afimTransform(V, t);
};

void Camera::translate(float x, float y, float z)
{
  posicao += Vec3(x, y, z);
};

void Camera::rotate(Vec3 axis, float radAngle)
{
  Vec3 normalAxis = axis.normalizar();

  // Rodrigues rotation
  W = normalAxis * pr_esc(normalAxis, W) +
      (float)cos(radAngle) * pr_vet(pr_vet(normalAxis, W), normalAxis) +
      (float)sin(radAngle) * pr_vet(normalAxis, W);

  U = normalAxis * pr_esc(normalAxis, U) +
      (float)cos(radAngle) * pr_vet(pr_vet(normalAxis, U), normalAxis) +
      (float)sin(radAngle) * pr_vet(normalAxis, U);

  V = normalAxis * pr_esc(normalAxis, V) +
      (float)cos(radAngle) * pr_vet(pr_vet(normalAxis, V), normalAxis) +
      (float)sin(radAngle) * pr_vet(normalAxis, V);
};

void Camera::scale(float x, float y, float z)
{
  largura *= x;
  altura *= y;
  d *= z;
};

#endif