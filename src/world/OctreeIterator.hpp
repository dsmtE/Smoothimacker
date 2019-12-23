// #include <iostream>
// #include <vector>
// #include <glm/glm.hpp>

// namespace world {


// iterator {
//     iterator(const iterator&);
//     ~iterator();
//     iterator& operator=(const iterator&);
//     iterator& operator++(); //prefix increment
//     reference operator*() const;
//     friend void swap(iterator& lhs, iterator& rhs); //C++11 I think
// };

// input_iterator : public virtual iterator {
//     iterator operator++(int); //postfix increment
//     value_type operator*() const;
//     pointer operator->() const;
//     friend bool operator==(const iterator&, const iterator&);
//     friend bool operator!=(const iterator&, const iterator&); 
// };