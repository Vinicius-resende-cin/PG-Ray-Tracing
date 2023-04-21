#ifndef _RENDERER_
#define _RENDERER_

#include "definitions/RayTracing.cpp"
#include "lodepng.cpp"
#include <iostream>
#include <string>

using namespace std;

void render(int pixelsX, int pixelsY, const Cena &cena, const Camera &camera, string imageName)
{
    // Create a vector of RGBA pixels
    vector<unsigned char> image(pixelsX * pixelsY * 4);

    for (int y = 0; y < pixelsY; y++)
    {
        for (int x = 0; x < pixelsX; x++)
        {
            int index = (y * pixelsX + x) * 4;
            Cor corPixel = traceRay(cena, camera, pixelsX, pixelsY, x, y, 4);
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