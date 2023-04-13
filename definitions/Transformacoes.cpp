#include <vector>
#include <iostream>
#include "Intersecoes.cpp"

using namespace std;

// Função para multiplicar duas matrizes de floats
vector<vector<float>> matrizMult(vector<vector<float>> mat1, vector<vector<float>> mat2)
{
    // cria uma matriz resultado do tamanho correto
    int rows1 = mat1.size();
    int rows2 = mat2.size();
    int cols2 = mat2[0].size();

    vector<vector<float>> result(rows1, vector<float>(cols2, 0));

    // multiplica as matrizes
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols2; j++)
        {
            for (int k = 0; k < rows2; k++)
            {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}

// Função para aplicar uma transformação afim a um vetor tridimensional
Vec3 afimTransform(Vec3 vec, vector<vector<float>> transf)
{
    vector<vector<float>> vec4 = {{vec.x}, {vec.y}, {vec.z}, {1}};         // converte para 4 dimensões
    vector<vector<float>> resultMat = matrizMult(transf, vec4);            // aplica a transformação
    Vec3 result = Vec3(resultMat[0][0], resultMat[1][0], resultMat[2][0]); // converte para 3 dimensões
    return result;
}
