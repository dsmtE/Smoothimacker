#include <iostream>
#include <array>
#include <glm/glm.hpp>

namespace world {
    template <typename T> 
	class  Octree {

	private:
		//---------- Variables ----------//
        Octree* _parent;
		std::array<Octree*, 8> _children; // using array because constant size ( 8 children )
		T* _val;
		unsigned int _currentSubId;
		unsigned int _depth;

		//---------- private functions ----------//

		bool validCoordinate(const glm::uvec3 &pos) const;
		unsigned int getMortonId(const glm::uvec3 &pos);
		bool trySimplify();

	public:
		Octree(const unsigned int &depth = 0); // root constructor 
		Octree(Octree<T>* parent, const unsigned int &currentSubId);
		~Octree();

		// Octree<T>* operator[](const uint8_t &id);
		Octree<T>* getChild(const uint8_t &id);
		
		bool hasChildren() const;
		inline bool isLeaf() const { return _depth == 0; };
		inline Octree<T>* getParent() const { return _parent; };
		inline unsigned int size() const { return (1 << _depth); };
		inline unsigned int depth() const { return _depth; };
		inline unsigned int capacity() const { unsigned int size = size(); return s*s*s; }

		//---------- Operators & functions ----------//
		T* getValue() const; // const only read
		T* getValue(const glm::uvec3 &pos) const; // const only read
		bool delValue(const glm::uvec3 &pos);
		bool setValue(const glm::uvec3 &pos, const T &val);
};


template <typename T> 
bool Octree<T>::hasChildren() const {
	for (Octree* o : _children) { // for each children
		if (o != nullptr) {
			return true; // if one exist
		}
	}
	return false;
}

template <typename T> 
Octree<T>::Octree(const unsigned int &depth) : 
		_parent(nullptr), 
		_children{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 
		_val(nullptr), 
		_currentSubId(0), 
		_depth(depth) {
}

template <typename T> 
Octree<T>::Octree(Octree<T>* parent, const unsigned int &currentSubId) : 
		_parent(parent), 
		_children{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, 
		_val(nullptr), 
		_currentSubId(currentSubId), 
		_depth(parent->_depth-1) {
}

template <typename T> 
Octree<T>* Octree<T>::getChild(const uint8_t &id) {
	return _children[id];
}

template <typename T> 
Octree<T>::~Octree() {
	if( _val != nullptr ) {
		delete _val;
		_val = nullptr;
	}

	for (Octree* o : _children) { // for each existing children
		if (o != nullptr) {
			delete o;
		}
	}
}

template <typename T> 
bool Octree<T>::validCoordinate(const glm::uvec3 &pos) const{
	// unsigned int s = size();
    // return pos.x < s && pos.y < s && pos.z < s;
	return glm::all(glm::lessThan(pos, glm::uvec3( size() )));
}

template <typename T>
unsigned int Octree<T>::getMortonId(const glm::uvec3 &pos) {
  	glm::uvec3 v = glm::floor( pos / glm::uvec3( 1 << (depth-1) ) );
	return v.x + (v.z << 1) + (v.y << 2);
}

/// return true if this octree one value change
template <typename T>
bool Octree<T>::setValue(const glm::uvec3 &pos, const T &val) {
	
	// we are at the lowest depth
	if (_depth == 0 ) {
		// there is no assigned value yet
		if(_val == nullptr ) {
			// assign value and try Simplify from parent
			_val = new T(val);
			// _parent.trySimplify(); // TODO LOD
			return true;
		}else {
			if (*_val == val) { // value is alredy the same
				return false;
			}else {
				*_val = val; // use copy constructor to change actual value
				// _parent.trySimplify(); // TODO
				return true;
			}
		}
	} else {
		assert(validCoordinate(pos)); // check bound

		//Get the Position we want to set using morton code
		unsigned int mortonMask = 1 << (_depth-1);
		glm::uvec3 mortonPos =  pos / glm::uvec3(mortonMask); // keep only needed byte for this _depth
		unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);
		
		// if the node with the right index alredy exist, then set the value recursively
		if (_children[mortonId] != nullptr) {
			return _children[mortonId]->setValue(pos - mortonPos*glm::uvec3(mortonMask), val);
		}else {
			// create this subOctree and set the value recursively
			Octree* newOctree = new Octree(this, mortonId);
			_children[mortonId] = newOctree;
			return newOctree->setValue(pos - mortonPos*glm::uvec3(mortonMask), val);
		}
	}
	// this declaration cannot normally be reached due to recursion
	assert(false);
	return false; 
}

template <typename T>
T*  Octree<T>::getValue() const {
	return _val; 
}

template <typename T>
T* Octree<T>::getValue(const glm::uvec3 &pos) const {
	if(_depth == 0 || !hasChildren()) {
		return _val; // can be nullptr
	}

  	assert(validCoordinate(pos)); // check bound

	//Get the Position we want to set using morton code
	unsigned mortonMask = 1 << (_depth-1);
	glm::uvec3 mortonPos =  pos / glm::uvec3(mortonMask);
	unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);
	
	// if the node with the right index alredy exist,
	if (_children[mortonId] != nullptr) {
		return _children[mortonId]->getValue(pos - mortonPos*glm::uvec3(mortonMask));
	}else {
		//If the right subOctree element doesn't exist return nullptr as default value
		return nullptr; 
	}
	// this declaration cannot normally be reached due to recursion
	assert(false);
	return nullptr;
}

/// return true if this octree one value change
template <typename T>
bool Octree<T>::delValue(const glm::uvec3 &pos) {

	// we are at the lowest depth
	if (_depth == 0 ) {
		// value assigned
		if(_val != nullptr ) {
			delete _val; // delete actual value
			_val = nullptr;  // set value to nullptr
			_parent->trySimplify(); // check if simplifications are possible from the parent
			return true;
		}else {
			return false; // already nullptr
		}
	} else {

		assert(validCoordinate(pos)); // check bound

		//Get the Position we want to set using morton code
		unsigned mortonMask = 1 << (_depth-1);
		glm::uvec3 mortonPos =  pos / glm::uvec3(mortonMask); // keep only needed byte for this _depth
		unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);
		
		// if the node with the right index alredy exist
		if (_children[mortonId] != nullptr) {
			// then delete the value recursively
			return _children[mortonId]->delValue(pos - mortonPos*glm::uvec3(mortonMask));
		}else {
			return false; // return false because we are already empty
		}
	}
	// this declaration cannot normally be reached due to recursion
	assert(false);
	return false;
}

/// return true if one simplification has been made
/// TODO LOD
/// at the moment, only simplifies when deleting a value.
template <typename T>
bool Octree<T>::trySimplify() {
	if(!hasChildren() || _depth == 0) {
		return false;
	}else {
		T* CompareVal = nullptr;
		for (Octree* o : _children) { // for each
			if (o != nullptr) { // existing children
				if (o->_val != CompareVal) {
					return false; // one value is different than nullptr
				}
			}
		}
		// they are all nullptr
		_parent->_children[_currentSubId] = nullptr;
		_parent->trySimplify(); // try to simplify parent
		delete this;
		return true;
	}
	// this declaration cannot normally be reached due to recursion
	assert(false);
	return false;
}

} // namespace end