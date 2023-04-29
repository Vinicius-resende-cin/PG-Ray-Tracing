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
    stringstream cc(n);
    float Or, Og, Ob, kd, ks, ka, kr, kt, p;
    getGeneralAttributes(cc, Or, Og, Ob, kd, ks, ka, kr, kt, p);

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

Luz *createLuz(stringstream &line)
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

    return new Luz(pos, c);
}

Plano *createPLuz(stringstream &line, Luz *&l)
{
    float lx, ly, lz, nx, ny, nz, Or, Og, Ob;
    line >> lx;
    line >> ly;
    line >> lz;
    line >> nx;
    line >> ny;
    line >> nz;
    line >> Or;
    line >> Og;
    line >> Ob;

    Vec3 pos(lx, ly, lz);
    Vec3 n(nx, ny, nz);
    Cor c(Or, Og, Ob);

    PlanoLuz *pl = new PlanoLuz(pos, n, c);
    l = dynamic_cast<Luz *>(pl);
    return new Plano(0, 1, 0, 0, 0, 0, 0, pos - n * 0.01f, n, Cor(255));
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
            else if (type == "pl")
            {
                Luz *l;
                Plano *p = createPLuz(line, l);
                cena.addLuz(l);

                // Forma *p1 = dynamic_cast<Forma *>(p);
                // cena.addForma(p1);
            }
            else if (type == "a")
            {
                float Ir, Ig, Ib;
                line >> Ir;
                line >> Ig;
                line >> Ib;

                cena.globalLight = Cor(Ir, Ig, Ib);
            }
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
};

int main()
{
    string filename;
    cin >> filename;

    Cena cena;
    Camera camera;
    if (!generateScene(filename, cena, camera))
    {
        cout << "Erro! Cena não gerada" << endl;
        return 1;
    }

    int pixelsX = camera.largura;
    int pixelsY = camera.altura;
    render(pixelsX, pixelsY, cena, camera, "antes");

    camera.translate(0, 450, 500);
    camera.rotate(Vec3(0, -1, 1), -PI / 6);
    // camera.scale(1.6f, 0.9f, 1);

    render(pixelsX, pixelsY, cena, camera, "depois");

    return 0;
}
