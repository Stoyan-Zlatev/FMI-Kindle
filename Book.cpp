#include "Book.h"
#include "Collection.hpp"
#include "Page.h"
#include "UserRating.h"
#include <iostream>
#pragma warning(disable:4996)

Book::Book() : Book("", "") {}

Book::Book(const MyString& title, const MyString& authorName)
{
	setTitle(title);
	setAuthorName(authorName);
}

int Book::hasRated(const MyString& username)
{
	for (size_t i = 0; i < ratings.count; i++)
	{
		if (ratings.collection[i].getUserName() == username)
		{
			return i;
		}
	}

	return -1;
}

double Book::sumRating() const
{
	double sum = 0;
	for (size_t i = 0; i < ratings.getCount(); i++)
	{
		sum += ratings.collection[i].getRating();
	}

	return sum;
}

void Book::setTitle(const MyString& title)
{
	this->title = title;
}

void Book::setAuthorName(const MyString& authorName)
{
	this->authorName = authorName;
}

void Book::readFromFile(std::fstream& file)
{
	//Title load
	size_t size;
	file.read((char*)&size, sizeof(size));
	char* data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	title = MyString(data);

	//Author name load
	file.read((char*)&size, sizeof(size));
	delete[] data;
	data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	authorName = MyString(data);

	delete[] data;

	//Pages load
	file.read((char*)&pages.count, sizeof(pages.count));
	for (size_t i = 0; i < pages.count; i++)
	{
		pages.collection[i].readFromFile(file);
	}

	//Comments load
	file.read((char*)&comments.count, sizeof(comments.count));
	for (size_t i = 0; i < comments.count; i++)
	{
		comments.collection[i].readFromFile(file);
	}

	//Ratings load
	file.read((char*)&ratings.count, sizeof(ratings.count));
	for (size_t i = 0; i < ratings.count; i++)
	{
		ratings.collection[i].readFromFile(file);
	}
}

void Book::saveToFile(std::fstream& file)
{
	//Title saved
	size_t size = title.getSize();
	file.write((const char*)&size, sizeof(size));
	file.write((const char*)title.c_str(), title.getSize());

	//Author saved
	size = authorName.getSize();
	file.write((const char*)&size, sizeof(size));
	file.write((const char*)authorName.c_str(), authorName.getSize());

	//Pages saved
	file.write((const char*)&pages.count, sizeof(pages.count));
	for (size_t i = 0; i < pages.count; i++)
	{
		pages.collection[i].saveToFile(file);
	}

	//Comments saved
	file.write((const char*)&comments.count, sizeof(comments.count));
	for (size_t i = 0; i < comments.count; i++)
	{
		comments.collection[i].saveToFile(file);
	}

	//Ratings saved
	file.write((const char*)&ratings.count, sizeof(ratings.count));
	for (size_t i = 0; i < ratings.count; i++)
	{
		ratings.collection[i].saveToFile(file);
	}
}

void Book::addComment(const MyString& username, const MyString& comment)
{
	comments.add(Comment(username, comment));
}

void Book::addPage(const MyString& content, int pageNumber)
{
	pages.add(Page(content, pageNumber));
}

void Book::editRate(const MyString& username, int rating)
{
	int userRatingIndex = hasRated(username);
	if (userRatingIndex == -1)
	{
		throw std::invalid_argument("You haven't rated this book yet!");
	}

	ratings.edit(UserRating(username, rating), userRatingIndex);
}

void Book::editPage(const MyString& content, int pageNumber)
{
	pages.edit(Page(content, pageNumber), pageNumber);
}

void Book::removeLastPage()
{
	pages.remove();
}

void Book::rate(const MyString& username, int rating)
{
	if (hasRated(username) >= 0)
	{
		throw std::invalid_argument("You have already rated this book!");
	}

	ratings.add(UserRating(username, rating));
}

void Book::printComments() const
{
	if (comments.getCount() == 0)
	{
		std::cout << "Comment section is empty!" << std::endl;
	}
	else
	{
		for (size_t i = 0; i < comments.getCount(); i++)
		{
			std::cout << comments.collection[i].getUserName() << " wrote: " << comments.collection[i].getContent() << std::endl;
		}
	}
}

void Book::printRatings() const
{
	if (ratings.getCount() == 0)
	{
		std::cout << "This book has no ratings yet!" << std::endl;
	}
	else
	{
		for (size_t i = 0; i < ratings.getCount(); i++)
		{
			std::cout << ratings.collection[i].getUserName() << " rated " <<
						 ratings.collection[i].getRating() << " stars!" << std::endl;
		}
		std::cout << "Average rate: " << getRating() << std::endl;
	}
}

void Book::printPageByIndex(int index) const
{
	std::cout << "Page " << index + 1 << ": " << pages.collection[index].getPageContent() << std::endl;
}

size_t Book::getPagesCount() const
{
	return pages.count;
}

double Book::getRating() const
{
	double ratingsSum = sumRating();
	return ratingsSum / (double)ratings.getCount();
}

const MyString Book::getTitle() const
{
	return title;
}

const MyString Book::getAuthorName() const
{
	return authorName;
}