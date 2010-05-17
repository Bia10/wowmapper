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
  for (wm_size_t i = 0; i < vertices_.size(); i++) {
    bbox_.min.x = std::min(bbox_.min.x, vertices_[i].x);
    bbox_.min.y = std::min(bbox_.min.y, vertices_[i].y);
    bbox_.min.z = std::min(bbox_.min.z, vertices_[i].z);
    bbox_.max.x = std::max(bbox_.max.x, vertices_[i].x);
    bbox_.max.y = std::max(bbox_.max.y, vertices_[i].y);
    bbox_.max.z = std::max(bbox_.max.z, vertices_[i].z);
  }
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

void Mesh_c::SetBoundingBox(const BBox_s &bbox) {
  bbox_ = bbox;
}
