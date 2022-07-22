#pragma once
#include "MyString.h"
#include <fstream>

class UserRating
{
	MyString username;
	size_t rating;
public:
	UserRating();
	UserRating(const MyString& user, size_t rating);

	void setRating(size_t rating);
	
	void saveToFile(std::ofstream& file) const;
	void readFromFile(std::ifstream& file);
	
	size_t getRating() const;
	const MyString getUserName() const;
};