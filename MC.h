#ifndef __MC_H__
#define __MC_H__

#include"mesh.h"
#include"volume.h"
#include"ext_math.h"

// MarchingCubes Tables from (http://paulbourke.net/geometry/polygonise/)
// Also, refer to there for more information
class MarchingCubes {
public:
	MarchingCubes(const volume& V);
	~MarchingCubes(void);
	void clear(void);
	mesh compute(float isovalue);

protected:
	const volume& m_vol;
	float m_isovalue;
	inline size_t linear_address(const vec3i& vox) const;

	// TOPOLOGY OF THE CELL-- these lists store offsets 
	// for vertices of a given cell (vertex_offset)
	// and for edges (edge_v1, edge_v2)
	static const vec3i vertex_offset[8];
	static const int edges[24];
	static const vec3i DX, DY, DZ;

	// RELATED TO STEP 1 -- tagging vertices
	static constexpr const uint8_t PLUS = 1;
	static constexpr const uint8_t MINUS = 0;
	std::vector<uint8_t> m_vertex_tag;
	inline uint8_t& vertex_tag(const vec3i& vox);
	inline const uint8_t& vertex_tag(const vec3i& vox) const;
	void tag_vertices(void);

	// RELATED TO STEP 2 -- computing vertices on edges
	vec3f grid_normal(const vec3i& vox) const;
	void edge_vertex(const vec3i& pos1, const vec3i& pos2, vec3f& position, vec3f& gradient, vec3f& color) const;
	inline size_t edge_id(const vec3i& pos1, const vec3i& pos2) const;

	// RELATED TO STEP 3 -- cell codes
	uint8_t compute_cell_code(const vec3i& cell) const;

	static const int edge_table[256];
	static const int triTable[256][16];
private:
	MarchingCubes(const MarchingCubes&);		// make copy constructor inaccessible. Copying a MC object is not meaningful.
};

inline size_t MarchingCubes::linear_address(const vec3i& vox) const {
	return m_vol.linear_address(vox.x, vox.y, vox.z);
}

inline uint8_t& MarchingCubes::vertex_tag(const vec3i& vox) {
	return m_vertex_tag[linear_address(vox)];
}

inline const uint8_t& MarchingCubes::vertex_tag(const vec3i& vox) const {
	return m_vertex_tag[linear_address(vox)];
}

inline size_t MarchingCubes::edge_id(const vec3i& pos1, const vec3i& pos2) const {
	vec3i dir = std::abs(pos2 - pos1);
	assert("MarchingCubes::edge_id() -- invalid argument(s)" && dir.length() == 1);
	return size_t(dir.dot(vec3i(0, 1, 2))) * m_vol.size() + linear_address(pos1);
}


#endif

