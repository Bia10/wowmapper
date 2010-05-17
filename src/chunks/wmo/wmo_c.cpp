#include "wmo_c.h"
#include "../m2/m2_c.h"

Wmo_c::Wmo_c(Buffer_t *buffer, const std::string &wmo_name, ModelMap_t *mm)
    : Model_c(buffer),
      wmo_name_(wmo_name),
      model_map_(*mm),
      mohd_(this, 0xc),
      motx_(this, mohd_.GetOffsetToNext()),
      momt_(this, motx_.GetOffsetToNext()),
      mogn_(this, momt_.GetOffsetToNext()),
      mogi_(this, mogn_.GetOffsetToNext()),
      mosb_(this, mogi_.GetOffsetToNext()),
      mopv_(this, mosb_.GetOffsetToNext()),
      mopt_(this, mopv_.GetOffsetToNext()),
      mopr_(this, mopt_.GetOffsetToNext()),
      movv_(this, mopr_.GetOffsetToNext()),
      movb_(this, movv_.GetOffsetToNext()),
      molt_(this, movb_.GetOffsetToNext()),
      mods_(this, molt_.GetOffsetToNext()),
      modn_(this, mods_.GetOffsetToNext()),
      modd_(this, modn_.GetOffsetToNext()) {

}

Wmo_c::~Wmo_c() {
  for (SubWmos_t::iterator sw = sub_wmos_.begin();
       sw !=  sub_wmos_.end();
       ++sw) {
    delete *sw;
  }
}

void Wmo_c::LoadWmo(MpqHandler_c &mpq_h) {
  Buffer_t sw_buf;
  // load all sub wmos
  for (uint32_t i = 0; i < mohd_.num_groups; i++) {
    // build sub wmo names
    char num[5]; sprintf(num, "_%03d", i);
    std::string sw_name;
    // "Azeroth\xyz\SomeBuilding\SomeBuilding.wmo"-4 + "_001" + ".wmo" ->
    // "Azeroth\xyz\SomeBuilding\SomeBuilding_001.wmo"
    sw_name = wmo_name_.substr(0, wmo_name_.size()-4)+num+std::string(".wmo");

    // load sub wmos
    mpq_h.LoadFile(sw_name.c_str(), &sw_buf);
    sub_wmos_.push_back(new SubWmo_c(&sw_buf));
  }
}

void Wmo_c::GetWmo(Mesh_c *mesh) const {
  Indices32_t indices;
  Vertices_t vertices;
  Normals_t normals;

  // merge all sub wmos into one big mesh
  for (SubWmos_t::const_iterator sw = sub_wmos_.begin();
       sw != sub_wmos_.end();
       ++sw) {
    Indices32_t idx((*sw)->indices().begin(), (*sw)->indices().end());
    InsertIndices(idx, vertices.size(), &indices);

    // mark vertices with no collision for removal
    /*const MopyChunk_s::MaterialInfos_t &mat_infos = (*sw)->mopy().mat_infos;
    size_t off = vertices.size();
    for (size_t i = 0; i < mat_infos.size(); i++) {
      if(mat_infos[i].id & 0x2) {
        indices[off+i*3+0] = 0xffffffff;
        indices[off+i*3+1] = 0xffffffff;
        indices[off+i*3+2] = 0xffffffff;
      }
    }*/

    vertices.insert(vertices.end(), (*sw)->vertices().begin(), (*sw)->vertices().end());
    normals.insert(normals.end(), (*sw)->normals().begin(), (*sw)->normals().end());
  }

  //RearrangeBuffers(&indices, &vertices, &normals);
  mesh->SetColors(&Colors_t(vertices.size(), 0xff0066cc));
  mesh->SetGeometry(&indices, &vertices, &normals);
}

M2_c* Wmo_c::GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin) {
  // check if m2 is already in our map
  ModelMap_t::iterator found = model_map_.find(filename);
  if (found != model_map_.end()) {
    return reinterpret_cast<M2_c*>(found->second);
  } else {
    // not in map, so load it
    Buffer_t buf;
    mpq_h.LoadFile(filename.c_str(), &buf);
    // create new entry in map
    M2_c *m2 = new M2_c(&buf);
    model_map_.insert(ModelPair_t(filename, m2));

    if (loadSkin) {
      std::string skin_name(filename);
      RreplaceWoWExt(skin_name, ".m2", "00.skin", &skin_name);
      // load skin
      mpq_h.LoadFile(skin_name.c_str(), &buf);
      m2->skin = Skin_p(new Skin_c(&buf));
    }

    return m2;
  }
}

void Wmo_c::LoadDoodads(MpqHandler_c &mpq_h, bool loadSkin) {
  for (ModdChunk_s::DoodadInfos_t::iterator info = modd_.doodad_infos.begin();
       info != modd_.doodad_infos.end();
       ++info) {
    std::string filename(ToLower(modn_.doodad_names.c_str()+info->name_off));
    RreplaceWoWExt(filename, ".mdx", ".m2", &filename);
    RreplaceWoWExt(filename, ".mdl", ".m2", &filename);

    doodads_.push_back(WmoDoodad_s());
    WmoDoodad_s &doodad = doodads_.back();

    doodad.m2 = GetM2(mpq_h, filename, loadSkin);
    doodad.info = &(*info);
  }
}

void Wmo_c::GetDoodads(Meshes_t *meshes, const Vec3_t &pos, const Vec3_t &rot) const {
  for (WmoDoodads_t::const_iterator doodad = doodads_.begin();
       doodad != doodads_.end();
       ++doodad) {
    const ModdChunk_s::DoodadInfo_s *info = doodad->info;
    const M2_c *m2 = doodad->m2;

    // add doodad to meshes
    meshes->push_back(Mesh_c());
    Mesh_c *mesh = &meshes->back();


    // if we have a skin we get the real mesh
    if(m2->skin.get() != NULL) {
      m2->GetMesh(*m2->skin, mesh);
    } else {
      m2->GetBVMesh(mesh);
    }

    // have to transform wmo doodads first to align with their parent wmos
    Vertices_t vertices(mesh->vertices());
    TransformVertices(info->pos, info->rot, info->scale, &vertices, 0,
                      mesh->vertices().size());

    mesh->SetVertices(&vertices);
    mesh->SetPositon(pos);
    mesh->SetRotation(rot);
  }
}
