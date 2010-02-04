#include <iostream>
#include <fstream>

#include "mpqhandler_c.h"
#include "adt/adt_c.h"
#include "wdt/wdt_c.h"

int main(int argc, char **argv) {

	MpqHandler_c mpq_handler;
	mpq_handler.OpenFile("common-2.MPQ");

	/* retrieve adt names list */
	MpqFileList adt_files;

	/* load to buffer */
	const char *filename = "World\\Maps\\Kalimdor\\Kalimdor.wdt";
	uint8_t *file_buf = NULL;
	int64_t file_size = mpq_handler.LoadFileByName(filename, &file_buf);

	/* write file to hard disk */
	/*std::fstream file_out("Azeroth.wdt", std::fstream::out);
	file_out.write((char*)file_buf, file_size);
	file_out.close();*/

	/* initialize ADT with file buffer */
	Wdt_c wdt(file_buf, "World\\Maps\\Kalimdor\\Kalimdor");

	AdtList_t adt_list;
	wdt.LoadAdts(mpq_handler, &adt_list);

	return 0;
}

