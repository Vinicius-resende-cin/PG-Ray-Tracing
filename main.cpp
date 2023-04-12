#include <iostream>
#include <vector>
#include "lodepng.cpp"

#include "definitions/RayCasting.cpp"

using namespace std;

Cena cena(Cor(100, 100, 100));

float d = 100.0f;
int Hy = d;
int Hx = d;
Vec3 camPos(0, 0, 0);
Vec3 camDir(1, 0, 0);
Vec3 vUp = Vec3(1, 0, 1);
Camera camera(camPos, camDir, vUp, d, Hy, Hx);

int pixelsX = 1000;
int pixelsY = 1000;

// Forma *p1 = new Plano(0.5f, 0.5f, 0.1f, 1, Vec3(0, 0, 0), Vec3(0, 0, 1), Cor(0, 0, 0));

int main()
{
    // definição da cena
    Esfera *esf = new Esfera(0.5f, 0.5f, 0.1f, 3, Vec3(1000 + d, 0, 0), 300, Cor(0, 255, 255));
    Esfera *esf2 = new Esfera(0.6f, 0.6f, 0.3f, 3, Vec3(500 + d, 500, 500), 100, Cor(100, 0, 255));

    Forma *f1 = dynamic_cast<Forma *>(esf);
    Forma *f2 = dynamic_cast<Forma *>(esf2);

    cena.addLuz(Luz(Vec3(40, 400, 100), Cor(255, 255, 255)));
    cena.addLuz(Luz(Vec3(40, -400, 100), Cor(255, 255, 255)));
    cena.addForma(f1);
    cena.addForma(f2);
    // cena.addPlano(p1);

    // Create a vector of RGBA pixels
    vector<unsigned char> image(pixelsX * pixelsY * 4);

    for (int y = 0; y < pixelsY; y++)
    {
        for (int x = 0; x < pixelsX; x++)
        {
            int index = (y * pixelsX + x) * 4;
            Cor corPixel = rayCasting(cena, camera, pixelsX, pixelsY, x, y);
            image[index + 0] = corPixel.r; // R channel
            image[index + 1] = corPixel.g; // G channel
            image[index + 2] = corPixel.b; // B channel
            image[index + 3] = 255;        // Alpha channel
        }
    }

    // Save the image as a PNG file
    unsigned error = lodepng::encode("image.png", image, pixelsX, pixelsY);
    if (error)
    {
        cout << "PNG encoder error: " << lodepng_error_text(error) << endl;
        return 1;
    }

    cout << "Image saved to 'image.png'" << endl;
    return 0;
}
