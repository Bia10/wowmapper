#pragma once

// geometry related typedefs
typedef glm::vec2 Vec2_t;
typedef glm::vec3 Vec3_t;

typedef std::vector<Vec2_t>       TexCoords_t;
typedef std::vector<Vec3_t>       Vertices_t;
typedef std::vector<Vec3_t>       Normals_t;
typedef std::vector<uint32_t>     Colors_t;

typedef std::vector<uint8_t>      Buffer_t;
typedef std::vector<uint16_t>     Indices16_t;
typedef std::vector<uint32_t>     Indices32_t;
typedef std::vector<uint64_t>     Indices64_t;
typedef std::vector<std::string>  Strings_t;

struct BBox_s {
  Vec3_t min;
  Vec3_t max;
};
