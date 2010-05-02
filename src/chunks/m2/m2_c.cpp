#include "m2_c.h"

M2_c::M2_c(Buffer_t *buffer) :
  Model_c(buffer), skin(NULL) {
  // get content information and resize to fit
  BlockInfo_s content_info = GetValue<BlockInfo_s>(0x3c, 0);
  content_.resize(content_info.num);
  // copy buffer and insert content
  CopyVector(GetBuffer(), content_info.offset, content_info.num, &content_);
}

void M2_c::GetMesh(const Skin_c &skin, Mesh_c *mesh) const {
  Indices32_t indices;
  Vertices_t vertices;
  Normals_t normals;

  skin.GetIndices(&indices);
  GetVertices(&vertices);
  GetNormals(&normals);

  mesh->SetColors(&Colors_t(vertices.size(), 0xff00ffff));
  mesh->SetGeometry(&indices, &vertices, &normals);
}

void M2_c::GetBVMesh(Mesh_c *mesh) const {
  // bounding volume indices, vertices and normals info
  Indices32_t indices;
  Vertices_t vertices;
  Normals_t normals;

  GetBVIndices(&indices);
  GetBVVertices(&vertices);
  GetBVNormals(&normals);

  Normals_t new_norm(vertices.size());

  for (int i = 0; i < indices.size()/3; i++) {
    new_norm[indices[i*3+0]] = normals[i];
    new_norm[indices[i*3+1]] = normals[i];
    new_norm[indices[i*3+2]] = normals[i];
  }

  mesh->SetGeometry(&indices, &vertices, &new_norm);
  mesh->SetColors(&Colors_t(mesh->vertices().size(), 0xff00ffff));
}

void M2_c::GetVertices(Vertices_t *vertices) const {
  for (M2Geometry_t::const_iterator m2g = content_.begin(); m2g
      != content_.end(); ++m2g) {
    vertices->push_back(m2g->position);
  }
}

void M2_c::GetNormals(Vertices_t *normals) const {
  for (M2Geometry_t::const_iterator m2g = content_.begin(); m2g
      != content_.end(); ++m2g) {
    normals->push_back(m2g->normal);
  }
}

void M2_c::GetBVIndices(Indices32_t *indices) const {
  BlockInfo_s bvi_info = GetValue<BlockInfo_s> (0xd8, 0);

  // copy 16-bit indices and make them 32 bit
  Indices16_t temp_idx16(bvi_info.num);
  CopyVector(GetBuffer(), bvi_info.offset, bvi_info.num, &temp_idx16);

  indices->assign(temp_idx16.begin(), temp_idx16.end());
}

void M2_c::GetBVVertices(Vertices_t *vertices) const {
  BlockInfo_s bvv_info = GetValue<BlockInfo_s> (0xe0, 0);
  vertices->resize(bvv_info.num);

  // get buffer, go to offset, how much to copy?, where to copy to, element to copy behind
  CopyVector(GetBuffer(), bvv_info.offset, bvv_info.num, vertices);
}

void M2_c::GetBVNormals(Vertices_t *normals) const {
  BlockInfo_s bvn_info = GetValue<BlockInfo_s> (0xe8, 0);
  normals->resize(bvn_info.num);

  CopyVector(GetBuffer(), bvn_info.offset, bvn_info.num, normals);
}
