#pragma once
#include <string>

struct Position {
	Position(float x_position, float y_position, float z_position) { x = x; y = y; z = z; }
	float x;
	float y;
	float z;
};

struct UVCoord {
	UVCoord(float u_coord, float v_coord) { u = u_coord; v = v_coord; }
	float u;
	float v;
};

struct Normal {
	Normal(float x_normal, float y_normal, float z_normal) { x = x_normal; y = y_normal; z = z_normal; }
	float x;
	float y;
	float z;
};

struct ColorValue {
	ColorValue() = default;
	ColorValue(int red, int green, int blue) { r = red; g = green; b = blue; }
	int r = 0, g = 0, b = 0;
};

struct Vertex {
	Vertex() = default;

	Vertex(const Position& position, const UVCoord& uv_coords, const Normal& normals) {
		Initialize(position, uv_coords, normals);
	}

	void initilizeColors(const ColorValue& colorValue) {
		r = colorValue.r;
		g = colorValue.g;
		b = colorValue.b;
	}

	float position_x, position_y, position_z;
	float normal_x, normal_y, normal_z;
	float u, v;
	int r = 0, g = 0, b = 0;

private:
	void Initialize(const Position& position, const UVCoord& uv_coords, const Normal& normals) {
		position_x = position.x;
		position_y = position.y;
		position_z = position.z;

		u = uv_coords.u;
		v = uv_coords.v;

		normal_x = normals.x;
		normal_y = normals.y;
		normal_z = normals.z;
	}
};

struct TriangleOfIndices {
	TriangleOfIndices(unsigned short a, unsigned short b, unsigned short c) {
		i[0] = a;
		i[1] = b;
		i[2] = c;
		this->material_name = material_name;
	}
	unsigned short i[3];
	std::string material_name;
};

struct Triangle {
	Triangle(const std::string material_name) {
		this->material_name = material_name;
	}
	Vertex vetices[3];
	std::string material_name;
};

