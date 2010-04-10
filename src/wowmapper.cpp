#include "mpqhandler_c.h"
#include "wdt/wdt_c.h"
#include "chunks/m2/m2_c.h"
#include "chunks/m2/skin_c.h"
#include "chunks/adt/adt_c.h"
#include "glview_c.h"

int main(int argc, char **argv) {

	MpqHandler_c mpq_handler(".");

	// load to buffer
	Buffer_t file_buf;
	mpq_handler.LoadFile("World\\maps\\Azeroth\\Azeroth.wdt", &file_buf);

	// initialize ADT with file buffer
	Wdt_c wdt(&file_buf[0], file_buf.size(), "World\\maps\\Azeroth\\Azeroth");
	file_buf.clear();

	AdtNames_t::const_iterator name = wdt.adt_names().begin();

	Points_t vertices, normals;
	Indices32_t indices, colors;

	vertices.reserve(20000000);
	normals.reserve(20000000);
	colors.reserve(20000000);
	indices.reserve(90000000);

	for (int i = 0; i < 256; i++) { ++name; }

	clock_t start = clock();
	int num_adts = 32;

	for (int i = 0; i < num_adts; i++) {
	  mpq_handler.LoadFile(name->c_str(), &file_buf);
	  std::cout << i << " " << name->c_str() << " " << file_buf.size()/1024 << " kb" << std::endl;
	  Adt_c adt(&file_buf, mpq_handler);

	  InsertIndices(adt.indices(), vertices.size(), &indices);

	  vertices.insert(vertices.end(), adt.vertices().begin(), adt.vertices().end());
	  normals.insert(normals.end(), adt.normals().begin(), adt.normals().end());
	  colors.insert(colors.end(), adt.colors().begin(), adt.colors().end());

	  ++name;
	}

	float time_needed = float(clock()-start) / CLOCKS_PER_SEC;
	std::cout << num_adts <<" ADTs in " << time_needed << " sec, ";
	std::cout << num_adts/time_needed << " ADTs/sec"<< std::endl;

	std::cout << "Vertices/Normals/Colors: " << vertices.size() << std::endl;

	std::cout << "Triangles: " << indices.size()/3 << std::endl;
	std::cout << vertices.front().x << " " << vertices.front().y << " " << vertices.front().z << std::endl;

  GlView_c gl_view(800,  600, "glview");
  gl_view.Initialize(indices.size());
  gl_view.SetBuffers(&vertices, &normals, &indices, &colors);
  vertices.clear(); normals.clear(); indices.clear();

  gl_view.Go();

	return 0;
}

