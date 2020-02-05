#include "../Include/Importer.h"
#include <fstream>

Importer::Model Importer::loadModelFromFile(std::string path)
{
    Model model;

    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        std::vector<Model::vec3> vertices;
        std::vector<Model::vec3> normals;

        std::vector<int> faceV;
        std::vector<int> faceN;
        int sizeV = 0;
        int sizeN = 0;
        int sizeF = 0;

        file.read((char*)&sizeV, 4);
        vertices.resize(sizeV);
        file.read((char*)&vertices[0], 4 * 3 * (size_t)sizeV);

        file.read((char*)&sizeN, 4);
        normals.resize(sizeN);
        file.read((char*)&normals[0], 4 * 3 * (size_t)sizeN);


        file.read((char*)&sizeF, 4);
        faceV.resize(sizeF);
        faceN.resize(sizeF);
        file.read((char*)&faceV[0], 4 * (size_t)sizeF);
        file.read((char*)&faceN[0], 4 * (size_t)sizeF);

        file.close();


        for (int& index : faceV)
            model.vertices.push_back(vertices[index]);
        
        for (int& index : faceN)
            model.normals.push_back(normals[index]);
    }

    return model;
}
