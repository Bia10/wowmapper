#include "mpqhandler_c.h"
#include "wdt/wdt_c.h"
#include "chunks/adt/adt_c.h"
//#include "glview_c.h"


bool ParseCmdlArgs(int argc, char **argv);

int main(int argc, char **argv) {
	MpqHandler_c mpq_handler(".");

	// load to buffer
	Buffer_t file_buf;
	mpq_handler.LoadFile("World\\maps\\Azeroth\\Azeroth.wdt", &file_buf);

	// initialize ADT with file buffer
	Wdt_c wdt(&file_buf[0], file_buf.size(), "World\\maps\\Azeroth\\Azeroth");
	file_buf.clear();

	AdtNames_t::const_iterator name = wdt.adt_names().begin();

	int off_adt = 511; //atoi(argv[1]);
	int num_adts = 1; //atoi(argv[2]);

	for (int i=0;i<off_adt;i++) { ++name; }

	Indices32_t idx, col;
	Points_t vtx, norm;
	idx.reserve(256*768*num_adts);
	vtx.reserve(256*145*num_adts);
	norm.reserve(256*145*num_adts);
	col.reserve(256*768*num_adts);

	clock_t start = clock();

	for (int i = 0; i < num_adts; i++) {
	  mpq_handler.LoadFile(name->c_str(), &file_buf);
	  std::cout << i << " " << name->c_str() << " " << file_buf.size()/1024 << " kb" << std::endl;
	  Adt_c adt(&file_buf, mpq_handler);

	  const Mesh_s &terr = adt.terrain();
	  const Meshes_t &doods = adt.m2s();
	  const Meshes_t &wmos = adt.wmos();

	  InsertIndices(terr.idx, vtx.size(), &idx);
    vtx.insert(vtx.end(), terr.vtx.begin(), terr.vtx.end());
    norm.insert(norm.end(), terr.norm.begin(), terr.norm.end());
    col.insert(col.end(), terr.col.begin(), terr.col.end());

    for (Meshes_t::const_iterator dood = doods.begin(); dood != doods.end(); ++dood) {
      InsertIndices(dood->idx, vtx.size(), &idx);
      vtx.insert(vtx.end(), dood->vtx.begin(), dood->vtx.end());
      norm.insert(norm.end(), dood->norm.begin(), dood->norm.end());
      col.insert(col.end(), dood->col.begin(), dood->col.end());
    }

    for (Meshes_t::const_iterator wmo = wmos.begin(); wmo != wmos.end(); ++wmo) {
      InsertIndices(wmo->idx, vtx.size(), &idx);
      vtx.insert(vtx.end(), wmo->vtx.begin(), wmo->vtx.end());
      norm.insert(norm.end(), wmo->norm.begin(), wmo->norm.end());
      col.insert(col.end(), wmo->col.begin(), wmo->col.end());
    }

	  ++name;
	}

	float time_needed = float(clock()-start) / CLOCKS_PER_SEC;
	std::cout << num_adts <<" ADTs in " << time_needed << " sec, ";
	std::cout << num_adts/time_needed << " ADTs/sec"<< std::endl;
	std::cout << "Triangles:" << idx.size()/3 << std::endl;

	Adt_c::CleanUp();

	/*std::fstream mesh_fs;
	mesh_fs.open("mesh.obj", std::fstream::out);

	if (mesh_fs.is_open()) {
	  for (Points_t::iterator vtx = vertices.begin(); vtx != vertices.end(); ++vtx) {
	    mesh_fs << "v " << vtx->x << " " << vtx->y << " " << vtx->z << std::endl;
	  }

	  for (int i = 0; i < indices.size(); i+=3) {
      mesh_fs << "f " << indices[i+0]+1 << " " << indices[i+1]+1 << " " << indices[i+2]+1 << std::endl;
    }
	  mesh_fs.close();
	  return 0;
	}*/

  /*GlView_c gl_view(800,  600, "glview");
  gl_view.Initialize(idx.size());
  gl_view.SetBuffers(&vtx, &norm, &idx, &col);

  gl_view.Go();*/

	return 0;
}
