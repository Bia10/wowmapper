#include "adt_c.h"
#include "../m2/m2_c.h"
#include "../m2/skin_c.h"

Adt_c::Adt_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h)
		: Chunk_c(buffer, length),
			mhdr_(this), mmdx_(this), mmid_(this), mwmo_(this),
			mwid_(this), mddf_(this), modf_(this), mh2o_(this) {
	GetSubChunk("RDHM", 4, &mhdr_);  // header
	GetSubChunk("XDMM", 4, &mmdx_);  // model filenames
	GetSubChunk("DIMM", 4, &mmid_);  // model ids (offsets to filenames)
	GetSubChunk("OMWM", 4, &mwmo_);  // wmo filenames
	GetSubChunk("DIWM", 4, &mwid_);  // wmo ids (offsets to filenames)
	GetSubChunk("FDDM", 4, &mddf_);  // doodad information
	GetSubChunk("FDOM", 4, &modf_);  // wmo information
	GetSubChunk("O2HM", 4, &mh2o_);  // water information

	GenerateTerrain();

	std::cout << "MMID: " << mmid_.name_offsets.size() << std::endl;
	std::cout << "MDDF: " << mddf_.doodad_info.size() << std::endl;

	for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
	       mcnk != mhdr_.mcin.mcnks.end();
	       ++mcnk) {
	  // get doodad filenames
	  Filenames_t filenames;
	  for (Indices32_t::iterator offset = mcnk->mcrf.doodad_offsets.begin();
         offset != mcnk->mcrf.doodad_offsets.end();
         ++offset) {
	    MddfChunk_s::DoodadInfo_s &info = mddf_.doodad_info.at(*offset);
	    std::string filename(mmdx_.m2_names.c_str() + mmid_.name_offsets.at(info.id));

	    // begin with model loading
	    std::string m2_name = filename;
      m2_name.replace(m2_name.find(".mdx", 0), 4, ".m2");

      uint8_t *m2_buf = NULL;
      int64_t m2_size = mpq_h.LoadFileByName(m2_name.c_str(), &m2_buf);
      std::cout << "Load M2: " << m2_name << std::endl;
      M2_c m2(m2_buf, m2_size);

      Points_t vertices, normals;
      m2.GetVertices(&vertices);
      m2.GetNormals(&normals);

      glm::mat4 transform(1.0f);
      transform = glm::scale(transform, glm::vec3(info.scale/1024.0f));


      transform = glm::rotate(transform, 180-info.orientation.z, glm::vec3(1,0,0));
      transform = glm::rotate(transform, 90-info.orientation.y, glm::vec3(0,1,0));
      transform = glm::rotate(transform, -info.orientation.x, glm::vec3(0,0,1));

      /*transform = glm::rotate(transform, info.orientation.y+180, glm::vec3(0, 1, 0));
      transform = glm::rotate(transform, info.orientation.z, glm::vec3(0, 0, 1));
      transform = glm::rotate(transform, info.orientation.x, glm::vec3(1, 0, 0));*/



      std::cout << "Orientation: " << info.orientation.x << " " << info.orientation.y << " " << info.orientation.z << std::endl;

      // transform vertices
      for (Points_t::iterator vtx = vertices.begin();
           vtx != vertices.end();
           ++vtx) {
        glm::vec4 vertex(vtx->x, vtx->y, vtx->z, 1.0f);
        vertex = vertex*transform;
        //vertex.y += 0.5f;

        *vtx = glm::vec3(vertex)+info.position;
      }

      vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
      normals_.insert(normals_.end(), normals.begin(), normals.end());

      std::string skin_name = filename;
      skin_name.replace(skin_name.find(".mdx", 0), 4, "00.skin");

      uint8_t *skin_buf = NULL;
      int64_t skin_size = mpq_h.LoadFileByName(skin_name.c_str(), &skin_buf);
      std::cout << "Load Skin: " << skin_name << std::endl;
      Skin_c skin(skin_buf, skin_size);

      Indices32_t indices;
      skin.GetIndices(&indices);
      InsertIndices(indices, vertices_.size()-vertices.size(), &indices_);
	  }
	}

	/*for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
	     mcnk != mhdr_.mcin.mcnks.end();
	     ++mcnk) {
	  mddf_.doodad_info.front().
	}*/

	/*std::cout << "Doodads: " << mhdr_.mcin.mcnks.at(1).mcrf.doodad_offsets.size() << std::endl;
	std::cout << "WMOs: " << mhdr_.mcin.mcnks.at(1).mcrf.wmo_offsets.size() << std::endl;

	for (Indices32_t::iterator offset = mhdr_.mcin.mcnks.at(1).mcrf.doodad_offsets.begin();
	     offset != mhdr_.mcin.mcnks.at(1).mcrf.doodad_offsets.end();
	     ++offset) {
	  try {
	    std::cout << *offset << ": " << mmdx_.m2_names.c_str()+(mmid_.name_offsets.at(*offset)) << std::endl;
	  } catch (std::exception &e) {
	    std::cout << "offset off: " << e.what() << std::endl;
	  }
	}*/

}

void Adt_c::GenerateTerrain() {
  vertices_.reserve(256*145);
  normals_.reserve(256*145);
  indices_.reserve(256*768);

  for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
       mcnk != mhdr_.mcin.mcnks.end();
       ++mcnk) {
    Points_t vertices, normals;
    Indices32_t indices;
    mcnk->mcvt.GetVertices(&vertices);
    mcnk->mcnr.GetNormals(&normals);
    mcnk->mcvt.GetIndices(&indices);

    // insert all vertices, normals, indices into their bigger arrays
    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
    normals_.insert(normals_.end(), normals.begin(), normals.end());

    // increment indices by size of vertices
    Indices32_t add(768, vertices_.size());
    std::transform(indices.begin(), indices.end(),
        add.begin(), indices.begin(), std::plus<uint32_t>());

    indices_.insert(indices_.end(), indices.begin(), indices.end());
  }
}
