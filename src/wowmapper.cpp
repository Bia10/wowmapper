#include <iostream>
#include <fstream>

#include "mpqhandler_c.h"
#include "adt/adt_c.h"

int main(int argc, char **argv) {

	MpqHandler_c mpq_handler;
	mpq_handler.OpenFile("common-2.MPQ");

	/* retrieve adt names list */
	MpqFileList adt_files;
	mpq_handler.GetMpqFilesByFilter(".adt", &adt_files);

	/* load to buffer */
	uint8_t *file_buf = NULL;
	std::cout << "Open " << *adt_files.begin() << " ..." << std::endl;
	int64_t file_size = mpq_handler.GetFileByName(adt_files.begin()->c_str(),
                                                &file_buf);

	/* write file to hard disk */
	std::fstream file_out("test.adt", std::fstream::out);
	file_out.write((char*)file_buf, file_size);
	file_out.close();

	/* initialize ADT with file buffer */
	Adt_c adt(file_buf);

	return 0;
}
