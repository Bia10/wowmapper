#include "../mpqhandler_c.h"
#include "../chunks/wdt/wdt_c.h"
#include "../chunks/adt/adt_c.h"
#include "glview_c.h"

void MergeMeshes(const Meshes_t &meshes, Mesh_c *finalMesh);

// This sample show you how to create a simple console app to display your
// maps. Console input would look like this:
// Prototype:  sample_consoleapp  DIR  WDT_DIR  ADT_OFFSET  ADT_NUM
//    -> ./sample_consoleapp . World\\maps\\Azeroth\\Azeroth 256 64
// Controls:
//  - look: mouse left click (drag)
//  - move: w, a, s, d
//  - up/down: e, q
//  - speed: h, j, k

int main(int argc, char **argv) {
  std::string mpq_dir(argv[1]);   // retrieve mpq directory if
  std::string wdt_dir(argv[2]);   // retrieve wdt directory + wdt name (without extension *.wdt)
  int off_adt = atoi(argv[3]);    // adt offset into adt names list
  int num_adts = atoi(argv[4]);   // amount of adts to load

  // print console input to see if you did everything right
  std::cout << "MPQ dir: " << mpq_dir << std::endl;
  std::cout << "WDT dir: " << wdt_dir << std::endl;
  std::cout << "ADT offset/amount: " << off_adt << "/" << num_adts << std::endl;

  // init mpq handler with directoy from mpq_dir
	MpqHandler_c mpq_h(mpq_dir);
	// load wdt for our map
	Buffer_t file_buf;
	mpq_h.LoadFile(wdt_dir + ".wdt", &file_buf);
	Wdt_c wdt(&file_buf, wdt_dir);

	// get adt names list and jump to offset
	NamesList_t::const_iterator name = wdt.names().begin();
	for (int i=0;i<off_adt;i++) { ++name; }

	// start clock
	clock_t start = clock();

	Indices32_t uids;        // unique identifiers to insert models only once
	Meshes_t meshes;         // mesh vector
	meshes.reserve(100000);  // IMPORTANT: reserve enough entries for ALL meshes

	// start loading
	for (int i = 0; i < num_adts; i++, ++name) {
	  std::cout << i << ": " << *name << std::endl;
	  // load file into buffer and pass it to adt
	  mpq_h.LoadFile(*name, &file_buf);
	  Adt_c adt(&file_buf, &uids);
	  adt.LoadWmos(mpq_h, true);     // true == detailed mesh, not collision mesh
	  adt.LoadDoodads(mpq_h, true);  // true == detailed mesh, not collision mesh

	  // retrieve terrain mesh
	  meshes.push_back(Mesh_c());
	  Mesh_c &mesh = meshes.back();
	  adt.BuildTerrain(true, &mesh); // true == remove vertices covered by water
	  // retrieve wmos, wmo-doodads and doodads
	  adt.GetWmos(&meshes, &meshes);
	  adt.GetDoodads(&meshes);
	}

	// merg all meshes into one big mesh so we can pass it to opengl
	Mesh_c mesh;
	MergeMeshes(meshes, &mesh);

	// some statistics output
	float time_needed = float(clock() - start) / CLOCKS_PER_SEC;
	std::cout << num_adts << " loaded in " << time_needed << " sec" << std::endl;
	std::cout << num_adts / time_needed << " ADTs per sec" << std::endl;

	Adt_c::CleanUp(); // buffered models

	// init opengl and display mesh
  GlView_c gl_view(800,  600, "glview");
  gl_view.Initialize(mesh.indices().size());
  gl_view.SetBuffers(&mesh.indices(), &mesh.vertices(),
                     &mesh.normals(), &mesh.colors());
  gl_view.Go();

	return 0;
}

void MergeMeshes(const Meshes_t &meshes, Mesh_c *finalMesh) {
  Indices32_t indices;
  Vertices_t vertices;
  Normals_t normals;
  Colors_t colors;

  for (Meshes_t::const_iterator mesh = meshes.begin();
       mesh != meshes.end();
       ++mesh) {
    InsertIndices(mesh->indices(), vertices.size(), &indices);

    Vertices_t mesh_vtx(mesh->vertices());
    TransformVertices(mesh->position(), mesh->rotation(), mesh->scale(),
                      &mesh_vtx, 0, mesh_vtx.size());

    vertices.insert(vertices.end(), mesh_vtx.begin(), mesh_vtx.end());
    normals.insert(normals.end(), mesh->normals().begin(), mesh->normals().end());
    colors.insert(colors.end(), mesh->colors().begin(), mesh->colors().end());
  }

  finalMesh->SetColors(&colors);
  finalMesh->SetGeometry(&indices, &vertices, &normals);
}
