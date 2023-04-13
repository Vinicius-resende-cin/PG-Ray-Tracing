#include "Renderer.cpp"
#include "definitions/Transformacoes.cpp"

#define PI 3.1415926535f

using namespace std;

// definição da câmera
float d = 100.0f;
int Hy = d;
int Hx = d;
Vec3 camPos(0, 0, 0);
Vec3 camDir(1, 0, 0);
Vec3 vUp = Vec3(1, 0, 1);
Camera camera(camPos, camDir, vUp, d, Hy, Hx);

// definição da cena
Cena cena(Cor(100, 100, 100));

// definição das luzes
Luz l1 = Luz(Vec3(800, 400, 100), Cor(0, 0, 255));
Luz l2 = Luz(Vec3(500, -400, 100), Cor(255, 255, 255));

// definição dos objetos
Esfera *e1 = new Esfera(0.5f, 0.5f, 0.1f, 3, Vec3(1100, 0, 0), 300, Cor(0, 255, 255));
Esfera *e2 = new Esfera(0.6f, 0.6f, 0.3f, 3, Vec3(600, 500, 500), 100, Cor(100, 0, 255));

Plano *p1 = new Plano(0.5f, 0.5f, 0.1f, 1, Vec3(0, 0, -400), Vec3(0, 0, 1), Cor(200, 200, 100));
Plano *p2 = new Plano(0.5f, 0.5f, 0.1f, 2, Vec3(0, -400, 0), Vec3(0, 1, 1), Cor(255, 255, 0));

Triangulo *t1 = new Triangulo(0.5f, 0.5f, 0.1f, 1,
                              vector<Vec3>{
                                  Vec3(855, 350, -200),
                                  Vec3(1100, 600, 200),
                                  Vec3(550, 690, -130)},
                              Vec3(-0.49f, -0.58f, 0.66f), Cor(0, 255, 0));

Forma *f1 = dynamic_cast<Forma *>(e1);
Forma *f2 = dynamic_cast<Forma *>(e2);
Forma *f3 = dynamic_cast<Forma *>(p1);
Forma *f4 = dynamic_cast<Forma *>(p2);
Forma *f5 = dynamic_cast<Forma *>(t1);

int pixelsX = 1000;
int pixelsY = 1000;

int main()
{
    // inicialização da cena
    cena.addLuz(l1);
    cena.addLuz(l2);
    cena.addForma(f1);
    cena.addForma(f2);
    cena.addForma(f3);
    cena.addForma(f4);
    cena.addForma(f5);

    render(pixelsX, pixelsY, cena, camera, "antes");

    // e1->centro = afimTransform(e1->centro,
    //                            {{1, 0, 0, -100},
    //                             {0, 1, 0, 200},
    //                             {0, 0, 1, 200},
    //                             {0, 0, 0, 1}});

    // p1->normal = afimTransform(p1->normal,
    //                            {{1, 0, 0, 0},
    //                             {0, (float)cos(PI / 6), (float)-sin(PI / 6), 0},
    //                             {0, (float)sin(PI / 6), (float)cos(PI / 6), 0},
    //                             {0, 0, 0, 1}});

    camera.W = afimTransform(camera.W,
                             {{(float)cos(PI / 6), 0, (float)sin(PI / 6), 0},
                              {0, 1, 0, 0},
                              {(float)-sin(PI / 6), 0, (float)cos(PI / 6), 0},
                              {0, 0, 0, 1}});

    camera.posicao = afimTransform(camera.posicao,
                                   {{1, 0, 0, 0},
                                    {0, 1, 0, 500},
                                    {0, 0, 1, 500},
                                    {0, 0, 0, 1}});

    camera.W = afimTransform(camera.W,
                             {{(float)cos(PI / 6), (float)sin(PI / 6), 0, 0},
                              {(float)-sin(PI / 6), (float)cos(PI / 6), 0, 0},
                              {0, 0, 1, 0},
                              {0, 0, 0, 1}});

    render(pixelsX, pixelsY, cena, camera, "depois");

    return 0;
}
