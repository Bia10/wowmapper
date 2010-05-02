#include "mesh_c.h"

Mesh_c::Mesh_c()
    : name_("unknown"),
      type_(MESH_TYPE_UNDEFINED),
      position_(0.0f, 0.0f, 0.0f),
      scale_(1.0f) {

}

Mesh_c::~Mesh_c() {

}

void Mesh_c::CalcBoundingBox() {

}

void Mesh_c::SetName(const std::string &name) {
  name_ = name;
}

void Mesh_c::SetType(MeshType meshType) {
  type_ = meshType;
}

void Mesh_c::SetTransformation(const Vec3_t &pos,
                               const Vec3_t &rot,
                               float scale) {
  SetPositon(pos);
  SetRotation(rot);
  SetScale(scale);
}

void Mesh_c::SetPositon(const Vec3_t &position) {
  position_ = position;
}

void Mesh_c::SetRotation(const glm::quat &quaternion) {
  quaternion_ = quaternion;
}

void Mesh_c::SetRotation(const Vec3_t &rotation) {
  rotation_ = rotation;
}

void Mesh_c::SetScale(float scale) {
  scale_ = 1.0f;
}

void Mesh_c::SetGeometry(Indices32_t *idx, Vertices_t *vtx, Normals_t *norm) {
  SetIndices(idx);
  SetVertices(vtx);
  SetNormals(norm);
}

void Mesh_c::SetIndices(Indices32_t *indices) {
  indices_.swap(*indices);
}

void Mesh_c::SetVertices(Vertices_t *vertices) {
  vertices_.swap(*vertices);
}

void Mesh_c::SetNormals(Normals_t *normals) {
  normals_.swap(*normals);
}

void Mesh_c::SetColors(Colors_t *colors) {
  colors_.swap(*colors);
}
