#include "UserRating.h"
#include "MyString.h"

UserRating::UserRating() : UserRating("", 0) {}

UserRating::UserRating(const MyString& username, size_t rating)
{
	this->username = username;
	setRating(rating);
}

void UserRating::setRating(size_t rating)
{
	if (rating < 0 || rating>5)
	{
		throw std::invalid_argument("Rating should be between 0 and 5!");
	}

	this->rating = rating;
}

void UserRating::saveToFile(std::fstream& file)
{
	size_t usernameSize = username.getSize();
	file.write((const char*)&usernameSize, sizeof(usernameSize));
	file.write((const char*)username.c_str(), username.getSize());

	size_t tempRating = rating;
	file.write((const char*)&tempRating, sizeof(tempRating));
}

void UserRating::readFromFile(std::fstream& file)
{
	size_t usernameSize;
	file.read((char*)&usernameSize, sizeof(usernameSize));
	char* data = new char[usernameSize + 1];
	file.read((char*)data, usernameSize);
	data[usernameSize] = '\0';
	username = MyString(data);
	delete[] data;

	file.read((char*)&rating, sizeof(rating));
}

size_t UserRating::getRating() const
{
	return rating;
}

const MyString UserRating::getUserName() const
{
	return username;
}
