#include "mpqhandler_c.h"
#include "chunks/adt/adt_c.h"
#include "wdt/wdt_c.h"

typedef std::list<Adt_c> AdtList_t;

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

	AdtList_t adt_list;

	for (AdtNames_t::const_iterator adt_name = wdt.adt_names().begin();
	     adt_name != wdt.adt_names().end();
	     ++adt_name) {
	  std::cout << *adt_name << std::endl;
	  size = mpq_handler.LoadFileByName(adt_name->c_str(), &file_buf);

	  // adt to list and assign buffer
	  Adt_c adt(file_buf, size, mpq_handler);

	  delete [] file_buf; file_buf = NULL;
	}

	//std::cout << "ADTs loaded: " << adt_list.size() << std::endl;

	/* retrieve adt names list */
	//AdtList_t adt_list;
	//wdt.LoadAdts(mpq_handler, &adt_list, 49, 0);

	//Wmo_c wmo(&file_buf);

	//mpq_handler.LoadFileByName(filename, &file_buf);

	/* write file to hard disk */
	/*std::fstream file_out("Azeroth.wdt", std::fstream::out);
	file_out.write((char*)file_buf, file_size);
	file_out.close();*/

	/*std::cout << "Loaded " << adt_list.size() << " ADts in ";
	std::cout << float(elapsed_time) /CLOCKS_PER_SEC << " secs" << std::endl;
	AdtList_t::iterator it_adt = adt_list.begin(); */

	/*AdtList_t adt_list;
	wdt.LoadAdts(mpq_handler, &adt_list, 32, 500);*/

	/*std::cout << "Generate meshes ..." << std::endl;
	for(AdtList_t::iterator adt = adt_list.begin();
	    adt != adt_list.end();
	    ++adt) {
	  (*adt)->GenerateMesh();
	}
	std::cout << "... finished!" << std::endl;*/

	/*Adt_c adt(&file_buf);
	adt.GenerateMesh();*/

	//Displayer_c disp(800, 600, "WoW Mapper");
	//disp.Start(&adt_list);

	delete [] file_buf;

	return 0;
}

