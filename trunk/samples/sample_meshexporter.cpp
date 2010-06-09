#include "../src/mpqhandler_c.h"
#include "../src/chunks/wdt/wdt_c.h"
#include "../src/chunks/adt/adt_c.h"

/* This sample (which is actually code used by me) exports all tiles of a map
 * with a little border around it, its ADT x/y-position, the bounding box (border
 * excluded ofc!), vertices and indices. The meshes perfectly fit further
 * processing by recast. Hopy you like it. */

typedef std::map<uint32_t, uint32_t> IndexRemap_t;
typedef std::pair<uint32_t, uint32_t> IndexPair_t;

// Basically outputs the whole mesh in one big one and gives you all tile positions
void LoadAdts(const std::string &dir, const std::string &wdtDir,
              Mesh_c *bigMesh, AdtPos_t *adtPos);

// Retrieves a tile matching a bounding rect
uint32_t GetTile(const Mesh_c &map, const Vec2_t &min,
                 const Vec2_t &max, Mesh_c *mesh);

// no error handling on arguments passed
int main(int argc, char **argv) {
  std::string mpq_dir(argv[1]); // ex.: .
  std::string wdt_dir(argv[2]); // ex.: World\\maps\\Azeroth\\Azeroth;

  Mesh_c map;
  AdtPos_t adt_pos;
  // first load adts and meshes
  LoadAdts(mpq_dir, wdt_dir, &map, &adt_pos);

  int count = 0;
  float tile_step = 533 + (1/3.0f);
  float border_width = tile_step/16;
  Vec2_t border(border_width, border_width);

  for (AdtPos_t::iterator pos = adt_pos.begin();
       pos != adt_pos.end();
       ++pos, count++) {
    // assign bounding rect values
    Vec2_t min((pos->x-32)*tile_step, (pos->y-32)*tile_step);
    Vec2_t max(min.x+tile_step, min.y+tile_step);
    Vec2_t border_min(min - border);
    Vec2_t border_max(max + border);

    Mesh_c mesh;

    if (GetTile(map, border_min, border_max, &mesh)) {
      mesh.CalcBoundingBox();
      BBox_s bbox = mesh.bbox();
      bbox.min = Vec3_t(min.x, bbox.min.y-1.0f, min.y); // -1.0f for safetiness
      bbox.max = Vec3_t(max.x, bbox.max.y+1.0f, max.y); // +1.0f for safetiness


      // now we write the final mesh to a file: X_Y.mesh
      std::stringstream ss;
      ss << pos->x << "_" << pos->y << ".mesh";
      std::fstream fs(ss.str().c_str(), std::fstream::out|std::fstream::binary);

      // print some info
      std::cout << "Get tile: " << pos->x << "/" << pos->y << std::endl;
      std::cout << "Min: " << min.x << "/" << min.y << std::endl;
      std::cout << "Max: " << max.x << "/" << max.y << std::endl;
      std::cout << "Write " << ss.str() << " to file" << std::endl << std::endl;

      // we export the bounding box ..
      Header_s bb_head = { "BBX", sizeof(BBox_s) };
      fs.write((char*)&bb_head, sizeof(bb_head));
      fs.write((char*)&bbox, sizeof(BBox_s));

      // ... position ...
      Header_s pos_head = { "POS", sizeof(AdtPos_s) };
      fs.write((char*)&pos_head, sizeof(pos_head));
      fs.write((char*)&(*pos), sizeof(AdtPos_s));

      // ... indices ...
      wm_size_t idx_mem_size = sizeof(uint32_t)*mesh.indices().size();
      Header_s idx_head = { "IDX", mesh.indices().size() };
      fs.write((char*)&idx_head, sizeof(idx_head));
      fs.write((char*)&mesh.indices()[0], idx_mem_size);

      // ... and vertices
      wm_size_t vtx_mem_size = sizeof(Vec3_t)*mesh.vertices().size();
      Header_s vtx_head = { "VTX", mesh.vertices().size() };
      fs.write((char*)&vtx_head, sizeof(vtx_head));
      fs.write((char*)&mesh.vertices()[0], vtx_mem_size);

      fs.close();
    }
  }

  return 0;
}

void LoadAdts(const std::string &dir, const std::string &wdtDir,
              Mesh_c *bigMesh, AdtPos_t *adtPos) {
  MpqHandler_c mpq_h(dir);

  // load wdt to get all adt positions
  Buffer_t file_buf;
  mpq_h.LoadFile(wdtDir + ".wdt", &file_buf);
  Wdt_c wdt(&file_buf, wdtDir);

  // start clock
  clock_t start = clock();

  // copy adt positions for later use
  AdtPos_t::const_iterator pos_begin = wdt.adt_list().begin();
  AdtPos_t::const_iterator pos_end = wdt.adt_list().end();
  adtPos->resize(wdt.adt_list().size());
  std::copy(pos_begin, pos_end, adtPos->begin());

  // size is for Azeroth without water, change it to bigger values to correctly
  // use it for other continents like Kalimdor
  Vertices_t vertices; vertices.reserve(23000000);
  Indices32_t indices; indices.reserve(105000000);

  // unique identifiers to insert models only once and model_map
  Indices32_t uids;
  ModelMap_t model_map;
  int count = 0;

  // start loading
  for (AdtPos_t::iterator pos = adtPos->begin(); pos != adtPos->end(); ++pos, count++) {
    Meshes_t meshes;
    meshes.reserve(10000);

    std::stringstream adt_name;
    adt_name << wdtDir << "_" << pos->x << "_" << pos->y << ".adt";

    mpq_h.LoadFile(adt_name.str(), &file_buf);
    std::cout << "Processing: (" << count+1 << ") " << adt_name.str() << " ";
    std::cout << file_buf.size()/1024 << " kb" << std::endl;

    Adt_c adt(&file_buf, &uids, &model_map);
    adt.LoadWmos(mpq_h, false);
    adt.LoadDoodads(mpq_h, false);

    meshes.push_back(Mesh_c());
    Mesh_c &terrain = meshes.back();

    // adt meshes to vector
    adt.BuildTerrain(true, &terrain);
    adt.GetDoodads(&meshes);
    adt.GetWmos(&meshes, &meshes);

    // merge meshes and add them into the buffer
    for (Meshes_t::const_iterator mesh = meshes.begin();
           mesh != meshes.end();
           ++mesh) {
      InsertIndices(mesh->indices(), vertices.size(), &indices);

      Vertices_t mesh_vtx(mesh->vertices());
      TransformVertices(mesh->position(), mesh->rotation(), mesh->scale(),
                        &mesh_vtx, 0, mesh_vtx.size());

      vertices.insert(vertices.end(), mesh_vtx.begin(), mesh_vtx.end());
    }
  }

  // cleanup modelmap
  for (ModelMap_t::iterator model = model_map.begin();
       model != model_map.end();
       ++model) {
    delete reinterpret_cast<Model_c*>(model->second);
  }


  std::cout << "Indices/vertices processed: " << indices.size() << "/";
  std::cout << vertices.size() << std::endl;

  bigMesh->SetVertices(&vertices);
  bigMesh->SetIndices(&indices);

  float time_needed = float(clock()-start)/CLOCKS_PER_SEC;
  std::cout << "Finished with ADTs after: " << time_needed;
  std::cout << " secs." << std::endl << std::endl;
}

uint32_t GetTile(const Mesh_c &map, const Vec2_t &min,
                 const Vec2_t &max, Mesh_c *mesh) {
  const Vertices_t &vtx = map.vertices();
  const Indices32_t &idx = map.indices();

  IndexRemap_t remap;
  wm_size_t num_tris = idx.size()/3;

  Vertices_t new_vtx;
  Indices32_t new_idx;
  uint32_t count = 0;

  // used to filter vertices matching the bounding rect
  for (wm_size_t i = 0; i < num_tris; i++) {
    Vec3_t v[3];
    v[0] = vtx[idx[i*3+0]];
    v[1] = vtx[idx[i*3+1]];
    v[2] = vtx[idx[i*3+2]];

    // look for vertices inside the bounding rect
    if (v[0].x >= min.x && v[0].x <= max.x && v[0].z >= min.y && v[0].z <= max.y &&
        v[1].x >= min.x && v[1].x <= max.x && v[1].z >= min.y && v[1].z <= max.y &&
        v[2].x >= min.x && v[2].x <= max.x && v[2].z >= min.y && v[2].z <= max.y) {
      // rearrange indices
      for (int j = 0; j < 3; j++) {
        IndexRemap_t::iterator found = remap.find(idx[i*3+j]);
        if (found == remap.end()) {
          remap.insert(IndexPair_t(idx[i*3+j], count));
          new_idx.push_back(count); // add new index
          new_vtx.push_back(v[j]);  // add new vertex
          count++;
        } else {
          new_idx.push_back(found->second);
        }
      }
    }
  }

  mesh->SetVertices(&new_vtx);
  mesh->SetIndices(&new_idx);

  return count;
}
