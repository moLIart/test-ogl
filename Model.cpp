#include "Model.h"
#include <fstream>

Model::Model(std::string name)
{
	std::ifstream is(name, std::ios::binary);
	if (is.good()) {
		std::string line;
		while (std::getline(is, line)) {
			if (line.compare(0, 2, "v ") == 0) {

			} else if (line.compare(0, 2, "f ") == 0) {

			}
		}
		is.close();
	}
}
