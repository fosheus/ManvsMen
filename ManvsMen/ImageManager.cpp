#include "ImageManager.h"

ImageManager::ImageManager() : images_(), resource_directories_()
{
}

ImageManager::~ImageManager()
{
	images_.clear();
	resource_directories_.clear();
}

const sf::Texture& ImageManager::get_image(const std::string& filename)
{
	// Check, whether the image already exists
	for (std::map<std::string, sf::Texture>::const_iterator it = images_.begin();
		it != images_.end();
		++it)
	{
		if (filename == it->first)
		{
			//std::cout << "DEBUG_MESSAGE: " << filename << " using existing image.\n";
			return it->second;
		}
	}

	// The image doesen't exists. Create it and save it.
	sf::Texture image;
	

	// Search project's main directory
	if (image.loadFromFile(filename))
	{
		images_[filename] = image;
		//std::cout << "DEBUG_MESSAGE: " ²<< filename << " loading image.\n";
		images_[filename].setSmooth(true);
		return images_[filename];
	}

	// If the image has still not been found, search all registered directories
	for (std::vector< std::string >::iterator it = resource_directories_.begin();
		it != resource_directories_.end();
		++it)
	{
		if (image.loadFromFile((*it) + filename))
		{
			images_[filename] = image;
			//std::cout << "DEBUG_MESSAGE: " << filename << " loading image.\n";
			return images_[filename];
		}

	}

	std::cout << "GAME_ERROR: Texture was not found. It is filled with an empty image.\n";
	images_[filename] = image;
	return images_[filename];
}

void ImageManager::delete_image(const sf::Texture& image)
{
	for (std::map<std::string, sf::Texture>::const_iterator it = images_.begin();
		it != images_.end();
		++it)
	{
		if (&image == &it->second)
		{
			images_.erase(it);
			return;
		}
	}
}

void ImageManager::delete_image(const std::string& filename)
{
	std::map<std::string, sf::Texture>::const_iterator it = images_.find(filename);
	if (it != images_.end())
		images_.erase(it);
}

void ImageManager::add_resource_directory(const std::string& directory)
{
	// Check whether the path already exists
	for (std::vector<std::string>::const_iterator it = resource_directories_.begin();
		it != resource_directories_.end();
		++it)
	{
		// The path exists. So it isn't necessary to add id once more.
		if (directory == (*it))
			return;
	}

	// insert the directory
	resource_directories_.push_back(directory);
}

void ImageManager::remove_resource_directory(const std::string& directory)
{
	for (std::vector<std::string>::iterator it = resource_directories_.begin();
		it != resource_directories_.end(); )
	{
		// The path exists. So it isn't necessary to add id once more.
		if (directory == (*it))
			it = resource_directories_.erase(it);
		else
			++it;
	}
}