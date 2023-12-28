#pragma once
#include "VertexStructs.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace VertexStructs;

class OBJModel {
public:
	OBJModel();
	OBJModel(const char* filename);
	~OBJModel();

	void LoadFromFile(const char* filename);
	const std::vector<Vertex>& GetVertexData();
	const std::vector<Triangle>& GetTrianglesData();
	ColorValue& getColorValue(const std::string& key);
	const int getVertexAmount();

private:
	std::map<std::string, ColorValue> m_MaterialMap;
	std::vector<Position> m_Positions;
	std::vector<UVCoord> m_UVCoords;
	std::vector<Normal> m_Normals;
	std::vector<Vertex> m_Vertices;
	std::vector<Triangle> m_Faces;

	bool is_OBJ_loaded;
	int m_vertexAmount;

	void Initialize();
	void LoadMaterialFromFile(const char* filename);
	void processPosition(const std::vector<std::string>& words);
	void processUV(const std::vector<std::string>& words);
	void processNormals(const std::vector<std::string>& words);
	void processVerticesAndTriangles(const std::vector<std::string>& words, const std::string material_name);

	static inline std::vector<std::string> splitString(const std::string& str, const char delimiter);
};