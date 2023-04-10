#include "Intersecoes.cpp"
#include <vector>

struct Luz
{
    Vec3 posicao;
    Cor cor;

    Luz(Vec3 pos, Cor c)
    {
        posicao = pos;
        cor = c;
    }
};

class Cena
{
public:
    vector<Luz> luzes{};
    Cor cor;
    // vector<Forma> obj;

    Cena(Cor c)
    {
        cor = c;
    }

    void addLuz(Luz luz)
    {
        luzes.insert(luzes.end(), luz);
    };
};

// int main()
// {
//     cout << "Hello world!" << endl;
//     char value;
//     int luza[6];
//     int ambientes[3];
//     int val;

//     Cena cena;
//     while (cin >> value)
//     {
//         if (value == 'l')
//         {
//             for (int i = 0; i < 6; i++)
//             {
//                 cin >> val;
//                 luza[i] = val;
//             }
//             cena.luz(luza);
//         }
//         else if (value == 'a')
//         {
//             for (int i = 0; i < 3; i++)
//             {
//                 cin >> val;
//                 ambientes[i] = val;
//             }
//             cena.ambi(ambientes);
//         }
//     };
//     cout << "Hello world!" << endl;
//     return 0;
// }
