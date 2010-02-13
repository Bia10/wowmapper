#include <iostream>
#include <fstream>

#include "mpqhandler_c.h"
#include "adt/adt_c.h"
#include "wdt/wdt_c.h"
#include "wmo/wmo_c.h"
#include "displayer_c.h"

int main(int argc, char **argv) {

	MpqHandler_c mpq_handler;
	mpq_handler.OpenFile("common-2.MPQ");

	/* load to buffer */
	const char *filename = "World\\Maps\\Azeroth\\Azeroth.wdt";
	uint8_t *file_buf = NULL;
	mpq_handler.LoadFileByName(filename, &file_buf);

	/* initialize ADT with file buffer */
	Wdt_c wdt(&file_buf, "World\\Maps\\Azeroth\\Azeroth");


	/* retrieve adt names list */
	AdtList_t adt_list;
	wdt.LoadAdts(mpq_handler, &adt_list, 16, 200);

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

	std::cout << "Generate meshes ..." << std::endl;
	for(AdtList_t::iterator adt = adt_list.begin();
	    adt != adt_list.end();
	    ++adt) {
	  (*adt)->GenerateMesh();
	}
	std::cout << "... finished!" << std::endl;

	/*Adt_c adt(&file_buf);
	adt.GenerateMesh();*/

	Displayer_c disp(800, 600, "WoW Mapper");
	disp.Start(&adt_list);

	return 0;
}

