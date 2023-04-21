#ifndef _RENDERER_
#define _RENDERER_

#include "definitions/RayTracing.cpp"
#include "lodepng.cpp"
#include <iostream>
#include <string>

using namespace std;

Ray calcPixelRay(const Camera &cam, int telaPx, int telaPy, int px, int py)
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

    return Ray(cam.posicao, pixelAtual);
}

void render(int pixelsX, int pixelsY, const Cena &cena, const Camera &camera, string imageName)
{
    // Create a vector of RGBA pixels
    vector<unsigned char> image(pixelsX * pixelsY * 4);

    for (int y = 0; y < pixelsY; y++)
    {
        for (int x = 0; x < pixelsX; x++)
        {
            int index = (y * pixelsX + x) * 4;

            Ray raioPixelAtual = calcPixelRay(camera, pixelsX, pixelsY, x, y);
            Cor corPixel = traceRay(cena, raioPixelAtual, 10);
            corPixel.clamp(0, 255);

            image[index + 0] = corPixel.r; // R channel
            image[index + 1] = corPixel.g; // G channel
            image[index + 2] = corPixel.b; // B channel
            image[index + 3] = 255;        // Alpha channel
        }
    }

    // Save the image as a PNG file
    unsigned error = lodepng::encode(imageName + ".png", image, pixelsX, pixelsY);
    if (error)
    {
        cout << "PNG encoder error: " << lodepng_error_text(error) << endl;
        return;
    }

    cout << "Image saved to " << imageName << ".png'" << endl;
}

#endif