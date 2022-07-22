#pragma once
#include "GlobalConstants.h"
#include "MyString.h"
#include <fstream>

class Page
{
	MyString content;
public:
	Page();
	Page(const MyString& pageContent);

	void setPageContent(const MyString& pageContent);
	
	void saveToFile(std::ofstream& file) const;
	void readFromFile(std::ifstream& file);

	const MyString getPageContent() const;
};