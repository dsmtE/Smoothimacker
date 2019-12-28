#pragma once

#include <vector>
#include <array>
#include <string>

#include <glm/glm.hpp>
#include <algorithm> // for swap

namespace world {


	template <typename T>
	class  Octree {

	protected:
		//---------- Variables ----------//
		Octree<T>* _parent;
		std::array<Octree<T>*, 8> _children; // using array because constant size ( 8 children )
		T* _val;

		uint8_t _currentSubId; // use uint8_t because max 8 children
		const uint8_t _depth; // use uint8_t because depth will never exceed 256 

		//---------- private functions ----------//

		bool validCoordinate(const glm::uvec3& pos) const;
		unsigned int getMortonId(const glm::uvec3& pos);
		// bool trySimplify();

	public:
		Octree(const uint8_t& depth = 0); // root constructor 
		Octree(Octree<T>* parent, const uint8_t& currentSubId);
		~Octree();

		Octree<T>* getChild(const uint8_t& id);

		bool hasChildren() const;
		inline bool isLeaf() const { return _depth == 0; };
		inline Octree<T>* getParent() const { return _parent; };
		inline unsigned int size() const { return unsigned int(1 << _depth); };
		inline uint8_t depth() const { return _depth; };
		inline unsigned int capacity() const { unsigned int s = size(); return s * s * s; }

		//---------- Operators & functions ----------//

		/// return ref to ptr to value of this Octree (nullptr if not exist)
		//T*& getthisValue();

		/// return copy to ptr to value with it's position (nullptr if not exist)
		T*& getValue(const glm::uvec3& pos);
		T* getPtrValue(const glm::uvec3& pos);
		/// same as previous but create new ptr is not exist and return pointer 
		/// use ptr of ptr to be able to modify the value outside and because
		/// the value necessarily exists or has just been created 
		/// (nullptr if it has just been created )
		T** getOrCreateValue(const glm::uvec3& pos);
		/// delete value at the given position if it exists 
		/// return true if one value was deleted
		bool delValue(const glm::uvec3& pos);
		/// set value if aleady exist, create new if not exist using copy constuctor of T
		T*& setValue(const glm::uvec3& pos, const T& val);
	};

	template <typename T>
	class  ListLinkedOctree {

	protected:
		//---------- Variables ----------//
		std::vector<T> _data; // array where data are store
		Octree<unsigned int> _octree; // octree where data are processed by storing coresponding id
		std::vector<unsigned int**> _valueOctreePtr; // storing pointer of the pointer witch storing id in octree (used to remove value with complexity in O(1))	const uint8_t _depth;

		//---------- private functions ----------//
		// TODO
		/// swap the value passed in parameter with the last element of our array
		void swapWithEnd(unsigned int*& id);

	public:
		ListLinkedOctree(const uint8_t& depth = 0); // root constructor 
		~ListLinkedOctree() = default;

		//---------- Getters & Setters ----------// 
		inline const std::vector<T>& vector() const { return _data; }; // return const ref to our vector of value
		inline unsigned int size() const { return _octree.size(); };
		inline uint8_t depth() const { return _octree.depth(); };
		inline unsigned int capacity() const { return _octree.capacity(); };

		//---------- Operators & functions ----------//
		/// return value at given position if exist else throw string error 
		/// catch(string const& str) {cerr << str << endl; }
		T& getValue(const glm::uvec3& pos);
		T* getPtrValue(const glm::uvec3& pos);
		/// del value in our data array and octree coresponding id
		bool delValue(const glm::uvec3& pos);
		/// set value at given position
		bool setValue(const glm::uvec3& pos, const T& val);
	};

	template <typename T>
	ListLinkedOctree<T>::ListLinkedOctree(const uint8_t& depth) : _octree(depth) {
		_data.reserve(size()); // reserve max size (octree size)
	}

	template <typename T>
	void ListLinkedOctree<T>::swapWithEnd(unsigned int*& id) {
		std::swap(_data[*id], _data.back());// swap value in data vector
		std::swap(_valueOctreePtr[*id], _valueOctreePtr.back()); // swap ref value of id in our octree
	}

	template <typename T>
	T& ListLinkedOctree<T>::getValue(const glm::uvec3& pos) {
		unsigned int* idPtr = _octree.getValue(pos);
		if (idPtr != nullptr) {
			return _data[*idPtr];
		}
		else {
			throw std::string("OctreeError :: nothing exist at this position impossible");
		}
	}

	template <typename T>
	T* ListLinkedOctree<T>::getPtrValue(const glm::uvec3& pos) {
		unsigned int* idPtr = _octree.getPtrValue(pos);
		if (idPtr != nullptr) {
			return &(_data[*idPtr]);
		}
		else {
			return nullptr;
		}
	}

	template <typename T>
	bool ListLinkedOctree<T>::delValue(const glm::uvec3& pos) {
		unsigned int* idPtr = _octree.getPtrValue(pos); // reference
		if (idPtr != nullptr) {
			if (*idPtr != _data.size() - 1) {
				swapWithEnd(idPtr);
			}
			_data.pop_back(); // delete last element in vector
			_valueOctreePtr.pop_back(); // delete ref to value in octree
			_octree.delValue(pos); // delete coresponding element in octree
			return true;
		}
		else {
			return false;
		}
	}

	template <typename T>
	bool ListLinkedOctree<T>::setValue(const glm::uvec3& pos, const T& val) {
		unsigned int** idPtr = _octree.getOrCreateValue(pos);
		if (*idPtr == nullptr) {
			_data.emplace_back(_val); // alocate new value using copy constructor
			*idPtr = new unsigned int(_data.size()); // create nw in octree (new using reference)(destroyed in octree destructor)
			_valueOctreePtr.push_back(idPtr); // save ptr to ptr to id in octree coresponding to value in data vector
		}
		else {
			_data[**idPtr] = val; // change value using copy constructor
		}
	}

	// -------------------------------------------------------------- //
	// --------------------------- Octree --------------------------- //
	// -------------------------------------------------------------- //


	template <typename T>
	bool Octree<T>::hasChildren() const {
		for (Octree<T>* o : _children) { // for each children
			if (o != nullptr) {
				return true; // if one exist
			}
		}
		return false;
	}

	template <typename T>
	Octree<T>::Octree(const uint8_t& depth) :
		_parent(nullptr),
		_children{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
		_val(nullptr),
		_currentSubId(0),
		_depth(depth) {
	}

	template <typename T>
	Octree<T>::Octree(Octree<T>* parent, const uint8_t& currentSubId) :
		_parent(parent),
		_children{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
		_val(nullptr),
		_currentSubId(currentSubId),
		_depth(parent->_depth - 1) {
	}

	template <typename T>
	Octree<T>* Octree<T>::getChild(const uint8_t& id) {
		return _children[id];
	}

	template <typename T>
	Octree<T>::~Octree() {
		if (_val != nullptr) {
			delete _val;
			// _val = nullptr;
		}

		for (Octree* o : _children) { // for each existing children
			if (o != nullptr) {
				delete o;
			}
		}
	}

	template <typename T>
	bool Octree<T>::validCoordinate(const glm::uvec3& pos) const {
		// unsigned int s = size();
		// return pos.x < s && pos.y < s && pos.z < s;
		return glm::all(glm::lessThan(pos, glm::uvec3(size())));
	}

	template <typename T>
	unsigned int Octree<T>::getMortonId(const glm::uvec3& pos) {
		glm::uvec3 v = glm::floor(pos / glm::uvec3(1 << (depth - 1)));
		return v.x + (v.z << 1) + (v.y << 2);
	}

	template <typename T>
	T*& Octree<T>::setValue(const glm::uvec3& pos, const T& val) {
		// we are at the lowest depth
		if (isLeaf()) {
			// there is no assigned value yet
			if (_val == nullptr) {
				// assign value and try Simplify from parent
				_val = new T(val); // use copy constructor
				// _parent.trySimplify(); // TODO LOD
				return _val;
			}
			else {
				if (*_val == val) { // value is alredy the same
					return _val;
				}
				else {
					*_val = val; // use copy to change actual value
					// _parent.trySimplify(); // TODO
					return _val;
				}
			}
		}
		else {
			assert(validCoordinate(pos)); // check bound

			//Get the Position we want to set using morton code
			unsigned int mortonMask = 1 << (_depth - 1);
			glm::uvec3 mortonPos = pos / glm::uvec3(mortonMask); // keep only needed byte for this _depth
			unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);

			// if the node with the right index alredy exist, then set the value recursively
			if (_children[mortonId] != nullptr) {
				return _children[mortonId]->setValue(pos - mortonPos * glm::uvec3(mortonMask), val);
			}
			else {
				// create this subOctree and set the value recursively
				Octree* newOctree = new Octree(this, mortonId);
				_children[mortonId] = newOctree;
				return newOctree->setValue(pos - mortonPos * glm::uvec3(mortonMask), val);
			}
		}
		// this declaration cannot normally be reached due to recursion
		assert(false);
		return false;
	}

	template <typename T>
	T*& Octree<T>::getValue(const glm::uvec3& pos) {
		if (isLeaf() || !hasChildren()) {
			return _val; // can be nullptr
		}

		assert(validCoordinate(pos)); // check bound

		//Get the Position we want to set using morton code
		unsigned mortonMask = 1 << (_depth - 1);
		glm::uvec3 mortonPos = pos / glm::uvec3(mortonMask);
		unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);

		// if the node with the right index alredy exist,
		if (_children[mortonId] != nullptr) {
			return _children[mortonId]->getValue(pos - mortonPos * glm::uvec3(mortonMask));
		}
		else {
			//If the right subOctree element doesn't exist return nullptr as default value
			throw std::string("OctreeError :: nothing exist at this position");
		}
		// this declaration cannot normally be reached due to recursion
		assert(false);
	}

	template <typename T>
	T* Octree<T>::getPtrValue(const glm::uvec3& pos) {
		if (isLeaf() || !hasChildren()) {
			return _val; // can be nullptr
		}

		assert(validCoordinate(pos)); // check bound

		//Get the Position we want to set using morton code
		unsigned mortonMask = 1 << (_depth - 1);
		glm::uvec3 mortonPos = pos / glm::uvec3(mortonMask);
		unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);

		// if the node with the right index alredy exist,
		if (_children[mortonId] != nullptr) {
			return _children[mortonId]->getPtrValue(pos - mortonPos * glm::uvec3(mortonMask));
		}
		else {
			//If the right subOctree element doesn't exist return nullptr as default value
			return nullptr;
		}
		// this declaration cannot normally be reached due to recursion
		assert(false);
	}


	template <typename T>
	T** Octree<T>::getOrCreateValue(const glm::uvec3& pos) {

		// we are at the lowest depth
		if (isLeaf()) {
			return &_val;
		}
		else {
			assert(validCoordinate(pos)); // check bound

			//Get the Position we want to set using morton code
			unsigned int mortonMask = 1 << (_depth - 1);
			glm::uvec3 mortonPos = pos / glm::uvec3(mortonMask); // keep only needed byte for this _depth
			unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);

			// if the node with the right index alredy exist, then set the value recursively
			if (_children[mortonId] != nullptr) {
				return _children[mortonId]->getOrCreateValue(pos - mortonPos * glm::uvec3(mortonMask));
			}
			else {
				// create this subOctree and set the value recursively
				Octree* newOctree = new Octree(this, mortonId);
				_children[mortonId] = newOctree;
				return newOctree->getOrCreateValue(pos - mortonPos * glm::uvec3(mortonMask));
			}
		}
		// this declaration cannot normally be reached due to recursion
		assert(false);
	}

	template <typename T>
	bool Octree<T>::delValue(const glm::uvec3& pos) {

		// we are at the lowest depth
		if (isLeaf()) {
			if (_val != nullptr) { // value assigned
				delete _val; // delete actual value
				_val = nullptr;  // set value to nullptr
				// TODO LOD
				// _parent->trySimplify(); // check if simplifications are possible from the parent
				return true;
			}
			else {
				return false; // already nullptr
			}
		}
		else {

			assert(validCoordinate(pos)); // check bound

			//Get the Position we want to set using morton code
			unsigned mortonMask = 1 << (_depth - 1);
			glm::uvec3 mortonPos = pos / glm::uvec3(mortonMask); // keep only needed byte for this _depth
			unsigned int mortonId = mortonPos.x + (mortonPos.z << 1) + (mortonPos.y << 2);

			// if the node with the right index alredy exist
			if (_children[mortonId] != nullptr) {
				// then delete the value recursively
				return _children[mortonId]->delValue(pos - mortonPos * glm::uvec3(mortonMask));
			}
			else {
				return false; // return false because we are already empty
			}
		}
		// this declaration cannot normally be reached due to recursion
		assert(false);
		return false;
	}

	/// return true if one simplification has been made
	/// TODO LOD
	/*
	template <typename T>
	bool Octree<T>::trySimplify() {
		if(!hasChildren() || isleaf() ) {
			return false;
		}else {
			T *CompareVal = nullptr;
			for (Octree<T> *o : _children) { // for each
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
	*/
} // namespace end