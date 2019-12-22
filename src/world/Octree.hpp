#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace world {
    template <typename T> 
	class  Octree {

	private:
		//---------- Variables ----------//
        Octree* _parent;
		std::vector<Octree*> _children; // using array because constant size ( 8 children )
		T* _val;
		unsigned int _depth;
		unsigned int _currentSubId;

		//---------- private functions ----------//

		bool validCoordinate(const glm::uvec3 &pos) const;
		unsigned int getMortonId(const glm::uvec3 &pos);
		bool trySimplify();

	public:
		Octree(const unsigned int &depth); // root constructor 
		Octree(Octree<T>* parent, const unsigned int &currentSubId);
		~Octree();

		inline bool hasChildren() const { return !_children.empty(); }
		inline unsigned int size() const { return (1 << _depth); };
		inline unsigned int depth() const { return _depth; };
		inline unsigned int capacity() const { unsigned int size = size(); return s*s*s; }

		//---------- Operators & functions ----------//

		T* getValue(const glm::uvec3 &pos) const; // const only read
		bool delValue(const glm::uvec3 &pos); // TODO
		bool setValue(const glm::uvec3 &pos, const T &val); 
	};


template <typename T> 
Octree<T>::Octree(const unsigned int &depth = 0) : _parent(nullptr), _children(0, nullptr), _val(nullptr), _currentSubId(0), _depth(depth) {
	_children.reserve(8); // reserve max 8 children
}


template <typename T> 
Octree<T>::Octree(Octree<T>* parent, const unsigned int &currentSubId) : _parent(parent), _children(0, nullptr), _val(nullptr), _currentSubId(currentSubId), _depth(parent._depth-1) {
	_children.reserve(8);
}

template <typename T> 
Octree<T>::~Octree() {
	if( _val != nullptr ) {
		delete _val;
		_val = nullptr;
	}

	for (Octree o : _children) { // for each existing children
		delete o;
	}
}

template <typename T> 
bool Octree<T>::validCoordinate(const glm::uvec3 &pos) const{
	unsigned int s = size();
    return pos.x < s && pos.y < s && pos.z < s;
}

template <typename T>
unsigned int Octree<T>::getMortonId(const glm::uvec3 &pos) {
  	glm::uvec3 v = glm::floor( pos / glm::uvec3( 1 << (depth-1) ) );
	return v.x + (v.z << 1) + (v.y << 2);
}


template <typename T>
bool Octree<T>::setValue(const glm::uvec3 &pos, const T &val) {

	if (_depth == 0 ) {
		if(_val == nullptr ) {
			_val = new T(val);
			_parent.trySimplify();
		}else {
			*_val = val; // use copy constructor to change value
			_parent.trySimplify();
			return true;
		}

	} else {

		assert(validCoordinate(pos)); // check bound

		unsigned mortonMask = 1 << (depth-1);
		glm::uvec3 mortonPos =  pos / glm::uvec3(mortonMask); // keep only needed byte for this _depth
		unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);
		
		//Loop over our subTree, see if we can find the node with the right index, then set the appropriate position.
		bool setValueInSubOctree = false;
		for(Octree* o : _children) {
			if(o->_currentSubId == mortonId) { // if this is the right child
				o->setValue(pos - mortonPos*glm::uvec3(mortonMask), val);
				setValueInSubOctree = true;
				break;
			}
		}

		if(!setValueInSubOctree) { //Finally, we haven't found the right subOctree element. Create the new subTree
			Octree* newOctree = new Octree(this, mortonId);
			_children.push_back(newOctree); // need to be destroyed in destructor
			newOctree->setValue(pos - mortonPos*glm::uvec3(mortonMask), val);
		}
	}
	return false; // change anything
}

template <typename T>
T* Octree<T>::getValue(const glm::uvec3 &pos) const {
	if(_depth == 0 || !hasChildren()) {
		return _val;
	}

  	assert(validCoordinate(pos)); // check bound

	unsigned mortonMask = 1 << (depth-1);
	glm::uvec3 mortonPos =  pos / glm::uvec3(mortonMask);
	unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);
	
	//Loop over our subTree, see if we can find the node with the right index, then get value recursively
	for(Octree* o : _children) {
		if(o->_currentSubId == mortonId) { // if this is the right child
			return o->getValue(pos - mortonPos*glm::uvec3(mortonMask));
		}
	}
	//If the right subOctree element doesn't exist return nullptr as default value
	return nullptr;
}

template <typename T>
bool Octree<T>::delValue(const glm::uvec3 &pos) {

	if(_depth == 0) {
		if( _val != nullptr ) {
			delete _val;
			_val = nullptr;
			_parent.trySimplify(); // look at children for LOD symplify
			return true;
		}
	
	} else {
		unsigned mortonMask = 1 << (depth-1);
		glm::uvec3 mortonPos =  pos / glm::uvec3(mortonMask);
		unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);

		if ( !hasChildren() ) {
			if( _val != nullptr ) {  // Create the new height subTree with this val
				Octree* newOctree = new Octree(this, mortonId);
				newOctree->_val = new T(*_val); // deep copy of value from pointer
				newOctree->delValue(pos - mortonPos*glm::uvec3(mortonMask));
			}
		}else {
			//Loop over our subTree, see if we can find the node with the right index, then del value recursively
			for(Octree* o : _children) {
				if(o->_currentSubId == mortonId) { // if this is the right child
					o->delValue(pos - mortonPos*glm::uvec3(mortonMask));
				}
			}
			// Finally, we haven't found the right subOctree element. already empty
		}
	}
	return false; // anything change
}

template <typename T>
bool Octree<T>::trySimplify() {

	if(!hasChildren() || depth == 0) {
		return true;
	}else {
		if (_children.size() >= 8 ) { 

			T* CompareVal = _children[0];
			bool allSameValue = true;
			for (size_t i = 1; i < 8; i++) {
				if (_children[i] != CompareVal) {
					allSameValue = false;
					break;
				}
			}
			if (allSameValue) {

				// pick this value for all in this parent Octree
				if(_val == nullptr ) {
					_val = new T(*CompareVal); // deep copy of value from pointer
				}else {
					*_val = *CompareVal; // use copy constructor to change value from pointer
				}

				for (Octree o : _children) { // for each existing children
					delete o; // delete child
				}
				_parent.trySimplify(); // try to simplify parent
				return true; // change have been done
			}
			// else : any simplification possible
		}
		// else : not all child was filled, not simplification possible
		return false; // anything change
	}
}

} // namespace end