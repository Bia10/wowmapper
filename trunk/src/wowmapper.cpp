#include <iostream>
#include <fstream>

#include "mpqhandler_c.h"
#include "adt/adt_c.h"
#include "wdt/wdt_c.h"
#include "displayer_c.h"

int main(int argc, char **argv) {

	MpqHandler_c mpq_handler;
	mpq_handler.OpenFile("common-2.MPQ");

	/* retrieve adt names list */
	MpqFileList adt_files;

	/* load to buffer */
	const char *filename = "World\\Maps\\Azeroth\\Azeroth.wdt";
	uint8_t *file_buf = NULL;
	mpq_handler.LoadFileByName(filename, &file_buf);

	/* write file to hard disk */
	/*std::fstream file_out("Azeroth.wdt", std::fstream::out);
	file_out.write((char*)file_buf, file_size);
	file_out.close();*/

	/* initialize ADT with file buffer */
	Wdt_c wdt(&file_buf, "World\\Maps\\Azeroth\\Azeroth");

	/*AdtList_t adt_list;
	clock_t current_time = clock();
	wdt.LoadAdts(mpq_handler, &adt_list);
	clock_t elapsed_time = clock() - current_time;

	std::cout << "Loaded " << adt_list.size() << " ADts in ";
	std::cout << float(elapsed_time) /CLOCKS_PER_SEC << " secs" << std::endl;
	AdtList_t::iterator it_adt = adt_list.begin(); */

	Displayer_c disp(480, 360, "WoW Mapper");

	mpq_handler.LoadFileByName("World\\Maps\\Azeroth\\Azeroth_32_49.adt", &file_buf);

	Adt_c adt(&file_buf);
	adt.GenerateMesh();
	glm::vec3 &pos = adt.mhdr_chunk().mcin->mcnk_index[0].mcnk->position;
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

	disp.Start(adt.map_patches(), adt.map_normals());

	return 0;
}

