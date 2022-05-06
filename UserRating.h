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

	void saveToFile(std::fstream& file);
	void readFromFile(std::fstream& file);
	void setRating(size_t rating);
	size_t getRating() const;
	const MyString getUserName() const;
};