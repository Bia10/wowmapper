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
	/*mpq_handler.GetMpqFilesByFilter(".wdt", &adt_files);

	for (MpqFileList::iterator adt_file = adt_files.begin();
	       adt_file != adt_files.end();
	       ++adt_file) {
	    std::cout << *adt_file << std::endl;
	}*/

	/* load to buffer */
	const char *filename = "World\\Maps\\Azeroth\\Azeroth.wdt";
	uint8_t *file_buf = NULL;
	int64_t file_size = mpq_handler.GetFileByName(filename, &file_buf);

	/* write file to hard disk */
	/*std::fstream file_out("BlackwingLair.wdt", std::fstream::out);
	file_out.write((char*)file_buf, file_size);
	file_out.close();*/

	/* initialize ADT with file buffer */
	//Adt_c adt(file_buf);
	Wdt_c wdt(file_buf, "Azeroth");

	return 0;
}
