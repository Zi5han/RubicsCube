#include "OBJModel.h"

using namespace VertexStructs;

OBJModel::OBJModel() {
	Initialize();
}

OBJModel::OBJModel(const char* filename) {

	std::string obj_filename(filename);
	obj_filename.erase(obj_filename.length() - 3);
	std::string mtl_filename(obj_filename);

	obj_filename = obj_filename + "obj";
	mtl_filename = mtl_filename + "mtl";

	Initialize();
	LoadMaterialFromFile(mtl_filename.c_str());
	LoadFromFile(obj_filename.c_str());
	is_OBJ_loaded = true;
	vertexAmount = static_cast<int>(m_Faces.size() * 3);
}

OBJModel::~OBJModel() {
	m_MaterialMap.clear();
	m_Vertices.clear();
}

void OBJModel::Initialize() {
	is_OBJ_loaded = false;
	m_MaterialMap = std::map<std::string, ColorValue>();
	m_Vertices = std::vector<Vertex>();
}

void OBJModel::LoadFromFile(const char* filename) {
	std::ifstream file(filename, std::ios::in);
	if (!file) {
		assert(false && "File could not be loaded");
		is_OBJ_loaded = false;
		return;
	}

	std::string line;
	std::string material_name;

	while (getline(file, line)) {
		std::vector<std::string> line_words = splitString(line, ' ');

		if (line_words.size() == 0) continue;

		if (line_words[0].compare("v") == 0) {
			processPosition(line_words);
		}
		else if (line_words[0].compare("vt") == 0) {
			processUV(line_words);
		}
		else if (line_words[0].compare("vn") == 0) {
			processNormals(line_words);
		}
		else if (line_words[0].compare("usemtl") == 0) {
			material_name = "";
			for (int i = 1; i < line_words.size(); i++) {
				material_name += line_words[i] + " ";
			}
			material_name.pop_back();
		}
		else if (line_words[0].compare("f") == 0) {
			assert(line_words.size() <= 4 && "Only triangles can be loaded from .obj file");
			processVerticesAndTriangles(line_words, material_name);
		}
	}
}

const std::vector<Vertex>& OBJModel::GetVertexData() {
	return m_Vertices;
}

const std::vector<Triangle>& OBJModel::GetTrianglesData()
{
	return m_Faces;
}

ColorValue& OBJModel::getColorValue(const std::string& key)
{
	return m_MaterialMap[key];
}

const int OBJModel::getVertexAmount()
{
	return vertexAmount;
}

void OBJModel::LoadMaterialFromFile(const char* filename) {
	std::ifstream file(filename, std::ios::in);

	if (!file) {
		assert(false && "File could not be loaded");
		is_OBJ_loaded = false;
		return;
	}

	// Hier wird nur der Name und die Farbe der .mtl Datei eingelesen
	std::string line;
	std::string material_name = "";
	while (std::getline(file, line)) {
		std::vector<std::string> line_words = splitString(line, ' ');

		if (line_words.size() == 0) continue;

		if (line_words[0].compare("newmtl") == 0) {
			material_name = "";
			for (int i = 1; i < line_words.size(); i++) {
				material_name += line_words[i] + " ";
			}
			material_name.pop_back();
		}
		else if (line_words[0].compare("Kd") == 0) {
			m_MaterialMap.insert(std::make_pair(material_name,
				ColorValue(
					static_cast<float>(atof(line_words[1].c_str())),
					static_cast<float>(atof(line_words[2].c_str())),
					static_cast<float>(atof(line_words[3].c_str())))
			));
		}
	}
}

inline std::vector<std::string> OBJModel::splitString(const std::string& str, const char delimiter)
{
	std::vector<std::string> words;
	std::string word;
	std::stringstream stream(str);

	while (getline(stream, word, delimiter))
		words.push_back(word);

	return words;
}

void OBJModel::processPosition(const std::vector<std::string>& words) {
	m_Positions.push_back(Position(
		static_cast<float>(atof(words[1].c_str())),
		static_cast<float>(atof(words[2].c_str())),
		static_cast<float>(atof(words[3].c_str()))
	));
}

void OBJModel::processUV(const std::vector<std::string>& words) {
	m_UVCoords.push_back(UVCoord(
		static_cast<float>(atof(words[1].c_str())),
		static_cast<float>(atof(words[2].c_str()))
	));
}

void OBJModel::processNormals(const std::vector<std::string>& words) {
	m_Normals.push_back(Normal(
		static_cast<float>(atof(words[1].c_str())),
		static_cast<float>(atof(words[2].c_str())),
		static_cast<float>(atof(words[3].c_str()))
	));
}

void OBJModel::processVerticesAndTriangles(const std::vector<std::string>& all_coords, const std::string material_name) {
	Triangle triangle(material_name);
	for (int i = 1; i < all_coords.size(); i++) {
		std::vector<std::string> coords_array = splitString(all_coords[i], '/');
		Vertex vertex(
			m_Positions[atoi(coords_array[0].c_str()) - 1],
			m_UVCoords[atoi(coords_array[1].c_str()) - 1],
			m_Normals[atoi(coords_array[2].c_str()) - 1]);

		m_Vertices.push_back(vertex);
		triangle.vetices[i - 1] = vertex;
	}
	m_Faces.push_back(triangle);

}