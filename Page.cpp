#include "Page.h"
#include "MyString.h"
#include <cstring>
#pragma warning(disable:4996)

Page::Page() : Page("", 0) {}

Page::Page(const MyString& pageContent, size_t pageNumber)
{
	setPageContent(pageContent);
	setPageNumber(pageNumber);
}

void Page::setPageContent(const MyString& pageContent)
{
	this->content = pageContent;
}

void Page::setPageNumber(size_t pageNumber)
{
	this->pageNumber = pageNumber;
}

void Page::saveToFile(std::fstream& file)
{
	file.write((const char*)&pageNumber, sizeof(pageNumber));

	size_t contentSize = content.getSize();
	file.write((const char*)&contentSize, sizeof(contentSize));
	file.write((const char*)content.c_str(),content.getSize());
}

void Page::readFromFile(std::fstream& file) 
{
	file.read((char*)&pageNumber, sizeof(pageNumber));

	size_t contentSize;
	file.read((char*)&contentSize, sizeof(contentSize));
	char* data = new char[contentSize + 1];
	file.read((char*)data, contentSize);
	data[contentSize] = '\0';
	content = MyString(data);
	delete[] data;
}

size_t Page::getPageNumber() const
{
	return pageNumber;
}

const MyString Page::getPageContent() const
{
	return content;
}