
#include <fstream>

#include "tools/ice.hpp"
#include "virtual/virtualice.hpp"

i32 main(i32 argc, chr * argv[]) {

	if (argc != 2) return 1;

	std::ifstream file(argv[1], std::ios::binary);
	if (!file.is_open() || !file.good()) return 1;

	arr<u8> code;

	file.seekg(0, std::ios_base::end);
	code.count = file.tellg();
	file.seekg(0, std::ios_base::beg);

	code.data = (u8 *)malloc(code.count);
	file.read((byt *)code.data, code.count);

	file.close();

	vm::run(code);

	free(code.data);

	return 0;
}
