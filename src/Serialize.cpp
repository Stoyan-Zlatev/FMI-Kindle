#include "Serialize.h"

void writeString(std::ofstream& sourceFile, const MyString& str) {
	int len = str.getSize();
	sourceFile.write((const char*)&len, sizeof(int));

	sourceFile.write(str.c_str(), sizeof(char) * len);
}

void readString(std::ifstream& sourceFile, MyString& field) {
	int len = 0;
	sourceFile.read((char*)&len, sizeof(len));

	char* buff = new char[len + 1];
	sourceFile.read(buff, sizeof(char) * len);

	buff[len] = '\0';

	field = buff;

	delete[] buff;
}
