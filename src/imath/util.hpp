namespace imath {

template <typename T>
bool isPowOfTwo(const T &val) {
    return (val & (val - 1)) == 0;
}

}