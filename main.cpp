#include "Renderer.cpp"
#include <iostream>
#include <fstream>
#include <sstream>

#define PI 3.1415926535f

using namespace std;

void getGeneralAttributes(stringstream &line,
                          float &Or, float &Og, float &Ob, float &kd, float &ks, float &ka, float &kr, float &kt, float &p)
{
    line >> Or;
    line >> Og;
    line >> Ob;
    line >> kd;
    line >> ks;
    line >> ka;
    line >> kr;
    line >> kt;
    line >> p;
}

Esfera *createEsfera(stringstream &line)
{
    float cx, cy, cz, r;
    line >> cx;
    line >> cy;
    line >> cz;
    line >> r;

    Vec3 centro(cx, cy, cz);

    float Or, Og, Ob, kd, ks, ka, kr, kt, p;
    getGeneralAttributes(line, Or, Og, Ob, kd, ks, ka, kr, kt, p);

    Cor c(Or, Og, Ob);

    return new Esfera(ka, kd, ks, kr, kt, p, 1.5f, centro, r, c);
}

Plano *createPlano(stringstream &line)
{
    float px, py, pz, vx, vy, vz;
    line >> px;
    line >> py;
    line >> pz;
    line >> vx;
    line >> vy;
    line >> vz;

    Vec3 pos(px, py, pz);
    Vec3 normal(vx, vy, vz);

    float Or, Og, Ob, kd, ks, ka, kr, kt, p;
    getGeneralAttributes(line, Or, Og, Ob, kd, ks, ka, kr, kt, p);

    Cor c(Or, Og, Ob);

    return new Plano(ka, kd, ks, kr, kt, p, 1.5f, pos, normal, c);
}

Malha *createMalha(stringstream &line, ifstream &file)
{
    size_t nt, nv;
    line >> nt;
    line >> nv;

    vector<Vec3> verts;
    vector<vector<int>> tvs;
    string n;

    // store all vertices
    for (int i = 0; i < nv; i++)
    {
        getline(file, n);

        stringstream vv(n);
        float vx, vy, vz;
        vv >> vx;
        vv >> vy;
        vv >> vz;
        Vec3 vertice(vx, vy, vz);
        verts.insert(verts.end(), vertice);
    }

    // store all triangles
    for (int i = 0; i < nt; i++)
    {
        getline(file, n);

        stringstream tt(n);
        int v1, v2, v3;
        tt >> v1;
        tt >> v2;
        tt >> v3;
        vector<int> triang = {v1, v2, v3};
        tvs.insert(tvs.end(), triang);
    }

    getline(file, n);
    float Or, Og, Ob, kd, ks, ka, kr, kt, p;
    getGeneralAttributes(line, Or, Og, Ob, kd, ks, ka, kr, kt, p);

    Cor c(Or, Og, Ob);

    return new Malha(ka, kd, ks, kr, kt, p, 1.5f, verts, tvs, c);
}

Forma *createForma(string &type, stringstream &line, ifstream &file)
{
    if (type == "s")
    {
        return dynamic_cast<Forma *>(createEsfera(line));
    }
    else if (type == "p")
    {
        return dynamic_cast<Forma *>(createPlano(line));
    }
    else if (type == "t")
    {
        return dynamic_cast<Forma *>(createMalha(line, file));
    }
    else
    {
        return nullptr;
    }
}

Camera createCamera(stringstream &line)
{
    float hres, vres, d, upx, upy, upz, cx, cy, cz, mx, my, mz;
    line >> hres;
    line >> vres;
    line >> d;
    line >> upx;
    line >> upy;
    line >> upz;
    line >> cx;
    line >> cy;
    line >> cz;
    line >> mx;
    line >> my;
    line >> mz;

    Vec3 up(upx, upy, upz);
    Vec3 pos(cx, cy, cz);
    Vec3 mira(mx, my, mz);

    return Camera(pos, mira, up, d, vres, hres);
}

Luz createLuz(stringstream &line)
{
    float lx, ly, lz, Or, Og, Ob;
    line >> lx;
    line >> ly;
    line >> lz;
    line >> Or;
    line >> Og;
    line >> Ob;

    Vec3 pos(lx, ly, lz);
    Cor c(Or, Og, Ob);

    return Luz(pos, c);
}

bool generateScene(string filename, Cena &cena, Camera &cam)
{
    ifstream entryFile(filename);
    string entryRow;

    try
    {
        while (getline(entryFile, entryRow))
        {
            stringstream line(entryRow);
            string type;
            line >> type;

            if (type == "s" || type == "p" || type == "t")
                cena.addForma(createForma(type, line, entryFile));
            else if (type == "c")
            {
                cam = createCamera(line);
            }
            else if (type == "l")
            {
                cena.addLuz(createLuz(line));
            }
            else if (type == "a")
            {
                float Ir, Ig, Ib;
                line >> Ir;
                line >> Ig;
                line >> Ib;

                cena.cor = Cor(Ir, Ig, Ib);
            }
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
};

// // definição da câmera
// float d = 1000.0f;
// int Hy = d;
// int Hx = d;
// Vec3 camPos(0, 0, 0);
// Vec3 camDir(1, 0, 0);
// Vec3 vUp = Vec3(1, 0, 1);
// Camera camera(camPos, camDir, vUp, d, Hy, Hx);

// // definição da cena
// Cena cena(Cor(100, 100, 100));

// // definição das luzes
// Luz l1 = Luz(Vec3(800, 400, 100), Cor(255, 255, 255));
// Luz l2 = Luz(Vec3(500, -400, 100), Cor(255, 255, 255));

// // definição dos objetos
// Esfera *e1 = new Esfera(0.1f, 0.0f, 0.0f, 0.1f, 0.9f, 3, 1.5f,
//                         Vec3(1100, 0, 0), 300, Cor(0, 0, 255));
// Esfera *e2 = new Esfera(0.6f, 0.6f, 0.3f, 0.0f, 0, 3, 0,
//                         Vec3(600, 500, 500), 100, Cor(50, 255, 50));

// Plano *p1 = new Plano(0.5f, 0.5f, 0.1f, 0.3f, 0.0f, 1, 0,
//                       Vec3(0, 0, -400), Vec3(0, 0, 1), Cor(255, 255, 0));
// Plano *p2 = new Plano(0.5f, 0.5f, 0.1f, 0.3f, 0.0f, 2, 0,
//                       Vec3(1100, -400, 0), Vec3(0, 1, 1), Cor(100, 0, 255));

// Malha *t1 = new Malha(0.1f, 0.2f, 0.0f, 0.8f, 0.0f, 3, 1.5f,
//                       vector<Vec3>{
//                           Vec3(855, 350, -200),
//                           Vec3(1100, 600, 200),
//                           Vec3(550, 690, -130)},
//                       vector<vector<int>>{
//                           {0, 1, 2}},
//                       Cor(0, 255, 0));

// Forma *f1 = dynamic_cast<Forma *>(e1);
// Forma *f2 = dynamic_cast<Forma *>(e2);
// Forma *f3 = dynamic_cast<Forma *>(p1);
// Forma *f4 = dynamic_cast<Forma *>(p2);
// Forma *f5 = dynamic_cast<Forma *>(t1);

int main()
{
    int pixelsX = 1000;
    int pixelsY = 1000;

    // // inicialização da cena
    // cena.addLuz(l1);
    // // cena.addLuz(l2);
    // cena.addForma(f1);
    // cena.addForma(f2);
    // cena.addForma(f3);
    // cena.addForma(f4);
    // cena.addForma(f5);

    string filename;
    cin >> filename;

    Cena cena;
    Camera camera;
    if (!generateScene(filename, cena, camera))
    {
        cout << "Erro! Cena não gerada" << endl;
        return 1;
    }

    render(pixelsX, pixelsY, cena, camera, "antes");

    // e1->translate(-100, 200, 200);

    // p1->rotate(Vec3(1, 0, 0), PI / 6);
    // p2->rotate(Vec3(1, 0, 1), PI / 6);

    // t1->translate(0, 0, 200);
    // t1->rotate(Vec3(0.5f, -1, 1), PI / 3);

    // camera.translate(0, 450, 500);
    // camera.rotate(Vec3(0, -1, 1), -PI / 6);
    // camera.scale(1.6f, 0.9f, 1);

    render(pixelsX, pixelsY, cena, camera, "depois");

    return 0;
}
