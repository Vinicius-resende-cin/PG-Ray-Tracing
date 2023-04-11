#include <iostream>
#include <vector>
#include "lodepng.cpp"

#include "definitions/RayCasting.cpp"

using namespace std;

Cena cena = Cena(Cor(100, 100, 100));

float camToS = 500.0f;
Vec3 camPos = Vec3(0, 0, 0);
Vec3 camDir = Vec3(1, 0, 0);

Vec3 vUp = Vec3(0, 0, 1);

int pixelsX = 2 * camToS;
int pixelsY = 2 * camToS;
int Hy = camToS;
int Hx = camToS;

Esfera esf = Esfera(0.5f, 0.5f, 0.1f, 3, Vec3(1000 + camToS, 0, 0), 400, Cor(0, 255, 255));
Esfera esf2 = Esfera(0.6f, 0.6f, 0.3f, 3, Vec3(500 + camToS, 700, 500), 200, Cor(100, 0, 255));

int main()
{
    // definição da cena
    cena.addLuz(Luz(Vec3(40, 400, 100), Cor(255, 255, 255)));
    cena.addLuz(Luz(Vec3(40, -400, 100), Cor(255, 255, 255)));
    cena.addEsfera(esf);
    cena.addEsfera(esf2);

    // Create a vector of RGBA pixels
    vector<unsigned char> image(pixelsX * pixelsY * 4);

    for (int y = 0; y < pixelsY; y++)
    {
        for (int x = 0; x < pixelsX; x++)
        {
            int index = (y * pixelsX + x) * 4;
            // cout << y << ',' << x;
            Cor corPixel = rayCasting(cena, camPos, camDir, vUp, camToS, Hx, Hy, pixelsX, pixelsY, x, y);
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
