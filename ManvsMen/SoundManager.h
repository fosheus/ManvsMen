#pragma once
#include <map>
#include <iostream>
#include "SFML\Audio.hpp"
class SoundManager
{
public:
	SoundManager();
	~SoundManager();
private : 
	SoundManager(const SoundManager&);
	SoundManager& operator= (const SoundManager&);
public :
	const sf::SoundBuffer* get_sound(const std::string& filename);

private :
	std::map<std::string, sf::SoundBuffer> sounds_;
	std::vector< std::string > resource_directories_;
};

