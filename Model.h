#pragma once
#include "mathlib.h"
#include <string>
#include <vector>

class Model
{
	std::vector<vec3f> vectices;
	std::vector<uint32_t> indicies;
public:
	Model() = delete;
	Model(std::string name);
};

