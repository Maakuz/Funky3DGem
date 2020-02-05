#pragma once
#include <vector>
#include <string>

class Importer
{
public:
	struct Model
	{
		struct vec3
		{
			float x, y, z;

			vec3()
			{
				x = 0;
				y = 0;
				z = 0;
			}
		};

		std::vector<vec3> vertices;
		std::vector<vec3> normals;
	};

	static Model loadModelFromFile(std::string path);
private:
};