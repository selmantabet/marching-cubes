#ifndef __VOLUME_H__
#define __VOLUME_H__

#include<vector>
#include<array>
#include<cassert>
#include<fstream>
#include"ext_math.h"

// Here, I provide a shallow wrapper for volumes
// Since everything is declared as inline, there is only a header file,
// no cpp file.
class volume {
public:
	inline volume(void);										// default constructor
	inline volume(const volume& other);							// copy constructor
	inline volume(int dimx, int dimy, int dimz);				// initialized constructor
	inline ~volume(void);										// destructor
	inline volume& operator=(const volume& other);				// assignment operator
	inline size_t dimension(size_t n) const;					// number of voxels along axis n
	inline size_t size(void) const;								// total number of voxels
	inline bool empty(void) const;								// true if volume empty (size = 0x0x0)
	inline void clear(void);									// empties the volume
	inline void resize(int dimx, int dimy, int dimz);			// resizes volume to desired resolutions
	inline float& operator()(int i, int j, int k);				// read/write access to voxel at ijk
	inline const float& operator()(int i, int j, int k) const;	// read-only access to voxel at ijk
	inline float& operator[](size_t n);							// read/write access to voxel at memory location n
	inline float& operator()(const vec3i& vox);					// read/write access to voxel at ijk
	inline const float& operator()(const vec3i& vox) const;		// read-only access to voxel at ijk
	inline const float& operator[](size_t n) const;				// read-only access to voxel at memory location n
	inline size_t linear_address(int i, int j, int k) const;	// computes the memory location of voxel ijk
	inline bool import_dat(const std::string& name);			// volume importer
	inline volume subsampled(void) const;						// TASK 3b
	inline volume& subsample(void);								// TASK 3b
protected:
	std::vector<float>  m_data;									// stores actual data
	std::array<int, 3>	m_dims;									// stores dimensions
};

/*
The difference between subsampled and subsample is the same as the difference between
normalized and normalize.
subsampled takes a copy of the object as an argument and returns the modified (in this case, subsampled)
version of the object, while maintaining the integrity of the original argument.
In the case of subsample, the object is taken as an argument, and modification will be 
done on the object itself rather than a copy. Effectively altering the object. This will not return anything.
*/
inline volume volume::subsampled(void) const{
	// TASK 3b: implement subsampling to half the volume's resolution
	//          along each dimension. To do so, first create a new
	//          result volume of resolution (m_dims[0]+1)/2,
	//		    (m_dims[1]+1)/2, (m_dims[2]+1)/2 (rounding up).
	//		    Then, for each voxel in new volume, average
	//          over the corresponding voxels in "Vol". 
	//          Most voxels will be the average of a 2x2x2 region,
	//          but you need to pay attention at the boundary if
	//		    one of the m_dims is odd...
	const volume& Vol(*this); // short-hand variable "Vol" is this object.

	// 1. create result volume of correct size
	volume result((m_dims[0] + 1) / 2, (m_dims[1] + 1) / 2, (m_dims[2] + 1) / 2);

	// 2. iterate all voxels in input volume
	// HINT: See below code, which in Python translates to
	//       "kmax = 2*k+1 if 2*k+1<Vol.dimension(2) else 2*k
	for (int k = 0; k < result.dimension(2); k++) {
		int kmax = 2 * k + 1 < Vol.dimension(2) ? 2 * k + 1 : 2 * k;
		for (int j = 0; j < result.dimension(1); j++) {
			int jmax = 2 * j + 1 < Vol.dimension(1) ? 2 * j + 1 : 2 * j;
			for (int i = 0; i < result.dimension(0); i++) {
				int imax = 2 * i + 1 < Vol.dimension(0) ? 2 * i + 1 : 2 * i;
				for (int z = 2 * k; z < kmax + 1; z++) {
					for (int y = 2 * j; y < jmax + 1; y++) {
						for (int x = 2 * i; x < imax + 1; x++) {
							result(i, j, k) += Vol(x, y, z);

						}
					}
				}
				result(i, j, k) /= (float(imax + 1 - 2 * i)) * (float(jmax + 1 - 2 * j)) * (float(kmax + 1 - 2 * k));
			}
		}
	// Now proceed analogously for j,i
		
	
	
				// Finally, you need three more loops to collect relevant voxels from Vol
				// These loops run from 2*{i,j,k} to (including!) {imax,jmax,kmax}. 
				// Make sure to divide the average by the correct number of voxels
				



				// Now, divide by float(nVoxels)
		

	}
	// Finally, return the result
	return result;
}

inline volume& volume::subsample(void) {
	*this = this->subsampled();
	return *this;
}

inline volume::volume(void) : m_dims({ 0,0,0 }) {
}

inline volume::volume(const volume& other) : m_dims({ 0,0,0 }) {
	*this = other;
}

inline volume::volume(int dimx, int dimy, int dimz) : m_dims({ 0,0,0 }) {
	resize(dimx, dimy, dimz);
}

inline volume::~volume(void) {
	clear();
}

inline volume& volume::operator=(const volume& other) {
	m_data = other.m_data;
	m_dims = other.m_dims;
	return *this;
}

inline size_t volume::dimension(size_t n) const {
	assert("volume::dimension() -- invalid argument" && n < 3);
	return m_dims[n];
}

inline size_t volume::size(void) const {
	return size_t(m_dims[0]) * size_t(m_dims[1]) * size_t(m_dims[2]);
}

inline bool volume::empty(void) const {
	return m_data.empty();
}

inline void volume::clear(void) {
	m_data.clear();
	m_dims = { 0,0,0 };
}

inline void volume::resize(int dimx, int dimy, int dimz) {
	assert("volume::resize() -- invalid argument(s)" && dimx >= 0 && dimy >= 0 && dimz >= 0);
	size_t s = size_t(dimx) * size_t(dimy) * size_t(dimz);
	if (s == 0) return clear();
	m_data.resize(s);
	m_dims = { dimx,dimy,dimz };
}

inline float& volume::operator()(int i, int j, int k) {
	return m_data[linear_address(i, j, k)];
}

inline const float& volume::operator()(int i, int j, int k) const {
	return m_data[linear_address(i, j, k)];
}

inline float& volume::operator()(const vec3i& vox) {
	return m_data[linear_address(vox.x, vox.y, vox.z)];
}

inline const float& volume::operator()(const vec3i& vox) const {
	return m_data[linear_address(vox.x, vox.y, vox.z)];
}

inline float& volume::operator[](size_t n) {
	assert("volume[] -- invalid argument" && n < size());
	return m_data[n];
}

inline const float& volume::operator[](size_t n) const {
	assert("volume[] -- invalid argument" && n < size());
	return m_data[n];
}

inline size_t volume::linear_address(int i, int j, int k) const {
	assert("volume::linear_address() -- invalid argument(s)" && i >= 0 && i < m_dims[0] && j >= 0 && j < m_dims[1] && k >= 0 && k < m_dims[2]);
	return size_t(i) + size_t(m_dims[0]) * (size_t(j) + size_t(m_dims[1]) * size_t(k));
}

inline bool volume::import_dat(const std::string& name) {
	std::ifstream stream(name, std::ifstream::binary | std::ifstream::in);
	if (!stream.good()) return false;
	uint16_t dims[3];
	try {
		stream.read(reinterpret_cast<char*>(dims), 3 * sizeof(uint16_t));
	}
	catch(...) {
		stream.close();
		return false;
	}
	
	std::vector<uint16_t> buf;
	buf.resize(size_t(dims[0]) * size_t(dims[1]) * size_t(dims[2]));
	try {
		stream.read(reinterpret_cast<char*>(buf.data()), buf.size()*sizeof(uint16_t));
	}
	catch (...) {
		stream.close();
		return false;
	}
	resize(dims[0], dims[1], dims[2]);
	for (size_t n = 0; n < size(); n++) m_data[n] = float(buf[n]) / 4095.0f;
	stream.close();
	return true;
}

#endif
