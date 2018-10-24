#pragma once
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

class ImageManager
{
public:
	ImageManager();
	~ImageManager();

private:
	ImageManager(const ImageManager&);
	ImageManager& operator =(const ImageManager&);

public:
	const sf::Texture&	get_image(const std::string& filename);
	void delete_image(const sf::Texture& image);
	void delete_image(const std::string& filename);
	void add_resource_directory(const std::string& directory);
	void remove_resource_directory(const std::string& directory);

private:
	std::map< std::string, sf::Texture > images_;
	std::vector< std::string > resource_directories_;
};
