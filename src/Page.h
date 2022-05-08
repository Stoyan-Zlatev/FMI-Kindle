#pragma once
#include "GlobalConstants.h"
#include "MyString.h"
#include <fstream>

class Page
{
	size_t pageNumber;
	MyString content;
	void setPageNumber(size_t pageNumber);
public:
	Page();
	Page(const MyString& pageContent, size_t pageNumber);

	void setPageContent(const MyString& pageContent);
	
	void saveToFile(std::fstream& file);
	void readFromFile(std::fstream& file);

	size_t getPageNumber() const;
	const MyString getPageContent() const;
};