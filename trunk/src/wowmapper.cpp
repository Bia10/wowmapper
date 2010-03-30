#include "mpqhandler_c.h"
//#include "chunks/adt/adt_c.h"
#include "wdt/wdt_c.h"
#include "chunks/m2/m2_c.h"
#include "chunks/m2/skin_c.h"
#include "chunks/adt/adt_c.h"
#include "glview_c.h"

int main(int argc, char **argv) {

	MpqHandler_c mpq_handler;
	mpq_handler.OpenFile("common-2.MPQ");

	// load to buffer
	const char *filename = "World\\maps\\Azeroth\\Azeroth.wdt";
	uint8_t *file_buf = NULL;
	int64_t size = mpq_handler.LoadFileByName(filename, &file_buf);

	// initialize ADT with file buffer
	Wdt_c wdt(file_buf, size, "World\\maps\\Azeroth\\Azeroth");
	delete [] file_buf; file_buf = NULL;

	AdtNames_t::const_iterator name = wdt.adt_names().begin();

	Points_t vertices, normals;
	Indices32_t indices, colors;

	for (int i = 0; i < 540; i++) { ++name; }

	clock_t start = clock();


	for (int i = 0; i < 128; i++) {
	  size = mpq_handler.LoadFileByName(name->c_str(), &file_buf);
	  std::cout << i << " " << name->c_str() << std::endl;
	  Adt_c adt(file_buf, size, mpq_handler);

	  InsertIndices(adt.indices(), vertices.size(), &indices);

	  vertices.insert(vertices.end(), adt.vertices().begin(), adt.vertices().end());
	  normals.insert(normals.end(), adt.normals().begin(), adt.normals().end());
	  colors.insert(colors.end(), adt.colors().begin(), adt.colors().end());

	  delete [] file_buf;
	  file_buf = NULL;

	  ++name;
	}

	std::cout << float(clock()-start) / CLOCKS_PER_SEC << " sec" << std::endl;

	std::cout << "Triangles visible: " << indices.size()/3 << std::endl;
	std::cout << vertices.front().x << " " << vertices.front().y << " " << vertices.front().z << std::endl;

  GlView_c gl_view(800,  600, "glview");
  gl_view.Initialize(indices.size());
  gl_view.SetBuffers(&vertices, &normals, &indices, &colors);
  vertices.clear(); normals.clear(); indices.clear();

  gl_view.Go();

	return 0;
}

