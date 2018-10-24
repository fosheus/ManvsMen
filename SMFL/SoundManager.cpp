#include "SoundManager.h"



SoundManager::SoundManager() : sounds_(),resource_directories_()
{
}


SoundManager::~SoundManager()
{
}

const sf::SoundBuffer* SoundManager::get_sound(const std::string & filename)
{
	for (std::map<std::string, sf::SoundBuffer>::const_iterator it = sounds_.begin();it != sounds_.end();++it) {
		if (filename == it->first)
		{
			return &(it->second);
		}
	}

	sf::SoundBuffer sound;


	if (sound.loadFromFile(filename))
	{
		sounds_[filename] = sound;
		return &sounds_[filename];
	}

	for (std::vector< std::string >::iterator it = resource_directories_.begin();
		it != resource_directories_.end();
		++it)
	{
		if (sound.loadFromFile((*it) + filename))
		{
			sounds_[filename] = sound;
			return &sounds_[filename];
		}

	}

	sounds_[filename] = sound;
	return &sounds_[filename];
}


