#include "Page.h"
#include <cstring>
#pragma warning(disable:4996)

Page::Page() : Page("") {}

Page::Page(const MyString& pageContent)
{
	setPageContent(pageContent);
}

void Page::setPageContent(const MyString& pageContent)
{
	this->content = pageContent;
}

void Page::saveToFile(std::ofstream& file) const
{
	size_t contentSize = content.getSize();
	file.write((const char*)&contentSize, sizeof(contentSize));
	file.write((const char*)content.c_str(),content.getSize());
}

void Page::readFromFile(std::ifstream& file) 
{
	size_t contentSize;
	file.read((char*)&contentSize, sizeof(contentSize));
	
	char* data = new char[contentSize + 1];
	file.read((char*)data, contentSize);
	data[contentSize] = '\0';
	content = MyString(data);
	delete[] data;
}

const MyString Page::getPageContent() const
{
	return content;
}