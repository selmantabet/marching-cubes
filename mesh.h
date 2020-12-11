#ifndef __MESH_H__
#define __MESH_H__

#include<vector>
#include"ext_math.h"
#include<cassert>
#include<fstream>
#include<inttypes.h>
#include<algorithm>

// This here is a simple class to store a triangle mesh.
// A triangle mesh contains a list of 3D positions (vertices)
// and a list of triangles (triples of vertex ids)
class mesh {
public:
	// Here, a vertes should have a position, a normal, and a color.
	inline mesh(void);								// default constructor
	inline mesh(const mesh& other);					// copy constructor
	inline ~mesh(void);								// default destructor
	inline mesh& operator=(const mesh& other);		// assignment operator
	inline int add_vertex(const vec3f& position,	// add a vertex consisting of position and optionally normal and color
		const vec3f& normal=vec3f(0.0f,0.0f,0.0f),	// RETURNS: vertex ID.
		const vec3f& color=vec3f(1.0f,1.0f,1.0f));
	inline int add_triangle(const vec3i& triangle);	// add a triangle i,j,k to the mesh. RETURNS triangle ID
	inline bool empty(void) const;					// true iff mesh has no triangles and no vertices
	inline void clear(void);						// empties the mesh

	inline size_t nTriangles(void) const;			// returns number of triangleS
	inline size_t nVertices(void) const;			// returns number of vertices

	inline const vec3f& position(int n) const;		// returns position of vertex n (read-only)
	inline const vec3f& normal(int n) const;		// returns normal of vertex n (read-only)
	inline const vec3f& color(int n) const;			// returns color of vertex n (read-only)
	inline const vec3i& triangle(int n) const;		// returns triangle n (read-only)

	inline vec3f& position(int n);					// returns vertex n, read/write access
	inline vec3f& normal(int n);					// returns vertex n, read/write access
	inline vec3f& color(int n);						// returns vertex n, read/write access
	inline vec3i& triangle(int n);					// returns triangle n, read/write access

	inline const vec3f* position_data(void) const;	// return position data pointer
	inline const vec3f* normal_data(void) const;	// return normal data pointer
	inline const vec3f* color_data(void) const;		// return color data pointer
	inline const vec3i* triangle_data(void) const;	// return triangle data pointer

	inline bool export_obj(const std::string& name) const;	// export the mesh as obj file for meshlab
protected:
	std::vector<vec3f>	m_position;					// actual storage of positions
	std::vector<vec3f>	m_normal;					// actual storage of normals
	std::vector<vec3f>	m_color;					// actual storage of colors
	std::vector<vec3i>	m_triangle;					// actual storage of triangles
};

inline mesh::mesh(void) {
}

inline mesh::mesh(const mesh& other) {
	*this = other;
}

inline mesh::~mesh(void) {
	clear();
}

inline mesh& mesh::operator=(const mesh& other) {
	if (other.empty()) {
		clear();
	}
	else {
		m_position = other.m_position;
		m_normal = other.m_normal;
		m_color = other.m_color;
		m_triangle = other.m_triangle;
	}
	return *this;
}

inline int mesh::add_vertex(const vec3f& position, const vec3f& normal, const vec3f& color) {
	int result = int(m_position.size());
	m_position.push_back(position); 
	m_normal.push_back(normal.normalized());
	m_color.push_back(color);
	return result;
}

inline int mesh::add_triangle(const vec3i& triangle) {
	int result = int(m_triangle.size());
	m_triangle.push_back(triangle);
	return result;
}

inline bool mesh::empty(void) const {
	return m_triangle.empty() && m_position.empty();
}
inline void mesh::clear(void) {
	m_position.clear();
	m_normal.clear();
	m_color.clear();
	m_triangle.clear();
}

inline size_t mesh::nTriangles(void) const {
	return m_triangle.size();
}

inline size_t mesh::nVertices(void) const {
	return m_position.size();
}

inline const vec3f& mesh::position(int n) const {
	assert("mesh::position() -- invalid argument" && n < m_position.size());
	return m_position[n];
}

inline const vec3f& mesh::normal(int n) const {
	assert("mesh::normal() -- invalid argument" && n < m_normal.size());
	return m_normal[n];
}

inline const vec3f& mesh::color(int n) const {
	assert("mesh::color() -- invalid argument" && n < m_color.size());
	return m_color[n];
}

inline const vec3i& mesh::triangle(int n) const {
	assert("mesh::triangle() -- invalid argument" && n < m_triangle.size());
	return m_triangle[n];
}

inline vec3f& mesh::position(int n) {
	assert("mesh::position() -- invalid argument" && n < m_position.size());
	return m_position[n];
}

inline vec3f& mesh::normal(int n) {
	assert("mesh::normal() -- invalid argument" && n < m_normal.size());
	return m_normal[n];
}

inline vec3f& mesh::color(int n) {
	assert("mesh::color() -- invalid argument" && n < m_color.size());
	return m_color[n];
}

inline vec3i& mesh::triangle(int n) {
	assert("mesh::triangle() -- invalid argument" && n < m_triangle.size());
	return m_triangle[n];
}

inline const vec3f* mesh::position_data(void) const {
	return m_position.data();
}

inline const vec3f* mesh::normal_data(void) const {
	return m_normal.data();
}

inline const vec3f* mesh::color_data(void) const {
	return m_color.data();
}

inline const vec3i* mesh::triangle_data(void) const {
	return m_triangle.data();
}

inline bool mesh::export_obj(const std::string& name) const {
	std::ofstream stream(name, std::ofstream::out);
	if (!stream.good()) return false;
	for (size_t n = 0; n < m_position.size(); n++) {
		vec3i color = (clamp(m_color[n], vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f)) * 255.0f).as<int>();
		stream << "v " << m_position[n].x << " " << m_position[n].y << " " << m_position[n].z << " " << color.r << " " << color.g << " " << color.b << std::endl;
		stream << "vn " << m_normal[n].x << " " << m_normal[n].y << " " << m_normal[n].z << std::endl;
	}
	for (size_t n = 0; n < m_triangle.size(); n++) {
		stream << "f " << (m_triangle[n].x + 1) << "//" << (m_triangle[n].x + 1) << " "
			<< (m_triangle[n].y + 1) << "//" << (m_triangle[n].y + 1) << " "
			<< (m_triangle[n].z + 1) << "//" << (m_triangle[n].z + 1) << std::endl;
	}
	stream.close();
}
#endif
