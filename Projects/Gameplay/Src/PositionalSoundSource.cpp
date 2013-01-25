#include "PositionalSoundSource.h"

PositionalSoundSource::PositionalSoundSource(string p_path, string p_filename,
											 bool p_loop, float p_soundVolume )
{
	m_path = p_path;
	m_filename = p_filename;
	m_loop = p_loop;
	m_soundVolume = p_soundVolume;
	m_soundIndex = -1;
}

const string& PositionalSoundSource::getPath() {
	return m_path;
}

void PositionalSoundSource::setPath(const string p_path) {
	m_path = p_path;
}

const string& PositionalSoundSource::getFilename() {
	return m_filename;
}

void PositionalSoundSource::setFilename(const string p_filename) {
	m_filename = p_filename;
}

const float& PositionalSoundSource::getSoundVolume() {
	return m_soundVolume;
}
void PositionalSoundSource::setSoundVolume(const float p_soundVolume) {
	m_soundVolume = p_soundVolume;
}

const int& PositionalSoundSource::getSoundIndex() {
	return m_soundIndex;
}
void PositionalSoundSource::setSoundIndex(const int p_soundIndex) {
	m_soundIndex = p_soundIndex;
}

const float& PositionalSoundSource::getLifeTime() {
	return m_lifeTime;
}
void PositionalSoundSource::setLifeTime(const float p_lifeTime) {
	m_lifeTime = p_lifeTime;
}
void PositionalSoundSource::decreaseLifeTime(const float& p_delta) {
	m_lifeTime -= p_delta;
}

const bool& PositionalSoundSource::loops() const
{
	return m_loop;
}
