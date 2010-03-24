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

	//AdtList_t adt_list;

	/*uint8_t *m2_buf = NULL;
  int64_t m2_size = mpq_handler.LoadFileByName("Character\\BloodElf\\Female\\BloodElfFemale.M2", &m2_buf);
  M2_c m2(m2_buf, m2_size);

  uint8_t *skin_buf = NULL;
  int64_t skin_size = mpq_handler.LoadFileByName("Character\\BloodElf\\Female\\BloodElfFemale00.skin", &skin_buf);
  Skin_c skin(skin_buf, skin_size);

  Points_t vertices, normals;
  m2.get_vertex_buffer(&vertices);
  m2.get_normal_buffer(&normals);
  Indices16_t indices;
  skin.get_index_buffer(&indices);

  std::cout << indices.size() << std::endl;*/

	/*for (AdtNames_t::const_iterator name = wdt.adt_names().begin();
	     name != wdt.adt_names().end();
	     ++name) {
	  file_buf = NULL;
	  size = mpq_handler.LoadFileByName(name->c_str(), &file_buf);
	  Adt_c adt(file_buf, size, mpq_handler);
	}*/

	AdtNames_t::const_iterator name = wdt.adt_names().begin();

	Points_t vertices, normals;
	Indices32_t indices;

	for (int i = 0; i < 276; i++) { ++name; }



	for (int i = 0; i < 128; i++) {
	  size = mpq_handler.LoadFileByName(name->c_str(), &file_buf);
	  Adt_c adt(file_buf, size, mpq_handler);

	  vertices.insert(vertices.end(), adt.vertices().begin(), adt.vertices().end());
	  normals.insert(normals.end(), adt.normals().begin(), adt.normals().end());

	  InsertIndices(adt.indices(), vertices.size()-adt.vertices().size(), &indices);

	  delete [] file_buf;
	  file_buf = NULL;

	  ++name;
	}

	std::cout << "Triangles visible: " << indices.size() << std::endl;

  GlView_c gl_view(800,  600, "glview");
  gl_view.Initialize(indices.size());
  gl_view.SetBuffers(&vertices, &normals, &indices);
  vertices.clear(); normals.clear(); indices.clear();

  gl_view.Go();

	return 0;
}

