#include "mpqhandler_c.h"
#include "wdt/wdt_c.h"
#include "chunks/m2/m2_c.h"
#include "chunks/m2/skin_c.h"
#include "chunks/adt/adt_c.h"
//#include "glview_c.h"

typedef std::map<std::string, std::string> Arguments_t;
typedef std::pair<std::string, std::string> Argument_t;

bool ParseCmdlArgs(int argc, char **argv);

int main(int argc, char **argv) {
  /*if (!ParseCmdlArgs(argc, argv)) {
    return 0;
  }*/

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

	clock_t start = clock();
	int num_adts = 2;

	for (int i = 0; i < num_adts; i++) {
	  mpq_handler.LoadFile(name->c_str(), &file_buf);
	  std::cout << i << " " << name->c_str() << " " << file_buf.size()/1024 << " kb" << std::endl;
	  Adt_c adt(&file_buf, mpq_handler);

	  /*InsertIndices(adt.indices(), vertices.size(), &indices);

	  vertices.insert(vertices.end(), adt.vertices().begin(), adt.vertices().end());
	  normals.insert(normals.end(), adt.normals().begin(), adt.normals().end());
	  colors.insert(colors.end(), adt.colors().begin(), adt.colors().end());*/

	  ++name;
	}

	float time_needed = float(clock()-start) / CLOCKS_PER_SEC;
	std::cout << num_adts <<" ADTs in " << time_needed << " sec, ";
	std::cout << num_adts/time_needed << " ADTs/sec"<< std::endl;

	std::cout << "Vertices/Normals/Colors: " << vertices.size() << std::endl;

	std::cout << "Triangles: " << indices.size()/3 << std::endl;
	std::cout << vertices.front().x << " " << vertices.front().y << " " << vertices.front().z << std::endl;

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
  gl_view.Initialize(indices.size());
  gl_view.SetBuffers(&vertices, &normals, &indices, &colors);*/
  vertices.clear(); normals.clear(); indices.clear();

  //gl_view.Go();

	return 0;
}

bool ParseCmdlArgs(int argc, char **argv) {
  // no no arguments and no odd arguments
  if (argc <= 1) {
    std::cout << "Not enough or invalid arguments. Use --help to see a list.";
    std::cout << std::endl;
    return false;
  } else if (argc == 2) {
    std::string arg(argv[1]);
    std::transform(arg.begin(), arg.end(), arg.begin(), tolower);

    if (arg == "--help") {
      // print help
      std::cout << "mapper help..." << std::endl;
      exit(0);
    } else {
      return false;
    }
  }

  Arguments_t args;
  // actual parsing starts here
  for (int i = 1; i < argc; i+=2) {
    std::string arg(argv[i]);
    std::transform(arg.begin(), arg.end(), arg.begin(), tolower);
    std::string val(argv[i+1]);
    std::transform(val.begin(), val.end(), val.begin(), tolower);
  }

  return true;
}

