#pragma once
#include <fstream>
#include "MyString.h"

void writeString(std::ofstream& outFile, const MyString& str);

void readString(std::ifstream& inFile, MyString& str);