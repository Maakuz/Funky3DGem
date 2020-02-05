#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

constexpr const char* EXPORT_DIR = "../Resources/Models";

struct vec3 
{
    float x, y, z;

    float& operator[](int i)
    {
        if (i == 0)
            return x;

        else if (i == 1)
            return y;

        else 
            return z;
    }
};

int main()
{
    namespace fs = std::filesystem;

    fs::directory_iterator dir(fs::current_path());

    printf("Exporting files:\n");

    int i = 0;

    for (auto file : dir)
    {
        if (file.path().extension() == ".obj")
        {
            printf("%s\n", file.path().filename().string().c_str());
            std::vector<vec3> vertices;
            std::vector<vec3> normals;
            std::vector<int> faces[2];

            std::ifstream in(file);
            if (in.is_open())
            {
                std::string str;
                while (!in.eof())
                {
                    in >> str;

                    if (str == "v" || str == "V")
                    {
                        vec3 vertex;
                        in >> vertex.x;
                        in >> vertex.y;
                        in >> vertex.z;

                        vertices.push_back(vertex);
                    }

                    else if (str == "vn" || str == "VN")
                    {
                        vec3 normal;
                        in >> normal.x;
                        in >> normal.y;
                        in >> normal.z;
                        normals.push_back(normal);
                    }

                    else if (str == "f" || str == "F")
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            in >> str;

                            std::string temp;
                            int slashesEncountered = 0;
                            for (size_t j = 0; j < str.size(); j++)
                            {
                                if (str[j] != '/')
                                    temp += str[j];

                                else
                                {
                                    if (slashesEncountered == 0)
                                    {
                                        slashesEncountered++;
                                        faces[0].push_back(std::stoi(temp) - 1);
                                    }

                                    //skip UV
                                    else if (slashesEncountered == 1)
                                        slashesEncountered++;

                                    
                                    temp.clear();
                                }
                            }

                            //Normal
                            faces[1].push_back(std::stoi(temp) - 1);
                            

                        }
                    }


                }




                in.close();
            }
        
            
            //print new format

            fs::create_directory(EXPORT_DIR);
            std::string name = std::string(EXPORT_DIR) + "/" + file.path().stem().string() + ".mop";
            std::ofstream out(name, std::ios::out | std::ios::binary);

            int length = vertices.size();
            out.write((char*)&length, 4);

            for (vec3& vert : vertices)
                out.write((char*)&vert, 4 * 3);

            length = normals.size();
            out.write((char*)&length, 4);
            for (vec3& norm : normals)
                out.write((char*)&norm, 4 * 3);

            length = faces[0].size();
            out.write((char*)&length, 4);
            for (int& face : faces[0])
                out.write((char*)&face, 4);

            for (int& face : faces[1])
                out.write((char*)&face, 4);
           
            out.close();

            /*
            std::ifstream test(name, std::ios::in | std::ios::binary);
            if (test.is_open())
            {
                std::vector<vec3> testvert, testNorm;
                std::vector<int> testFace[2];
                int size1, size2, size3 = 0;
                vec3 x = {30, 30 ,30};
                test.read((char*)&size1, 4);
                testvert.resize(size1);
                test.read((char*)&testvert[0], 4 * 3 * (size_t)size1);

                test.read((char*)&size2, 4);
                testNorm.resize(size2);
                test.read((char*)&testNorm[0], 4 * 3 * (size_t)size2);


                test.read((char*)&size3, 4);
                testFace[0].resize(size3);
                testFace[1].resize(size3);
                test.read((char*)&testFace[0][0], 4 * (size_t)size3);
                test.read((char*)&testFace[1][0], 4 * (size_t)size3);



                test.close();
            }
        */
        }
    }

    int selection = 0;
    system("pause");


    return 0;
}