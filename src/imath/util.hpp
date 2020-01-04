#include <random>
#include <functional> // for bind
#include <glm/glm.hpp>
#include <chrono>

namespace imath {

    template <typename T>
    bool isPowOfTwo(const T &val) {
        return (val & (val - 1)) == 0;
    }

    glm::uvec3 genVec3(const unsigned int &size) {
        std::mt19937_64 mt64Generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<> uniformIntDistribution(0, size-1);
        auto gen = std::bind(uniformIntDistribution, mt64Generator);
        return glm::uvec3(gen(), gen(), gen());
    } 

}