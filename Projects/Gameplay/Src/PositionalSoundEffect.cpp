#include "PositionalSoundEffect.h"

PositionalSoundEffect::PositionalSoundEffect(string p_filePath, bool p_loop,
								 float p_soundVolume )
{
	m_filePath = p_filePath;
	m_loop = p_loop;
	m_soundVolume = p_soundVolume;
	m_soundIndex = -1;
}

const string& PositionalSoundEffect::getFileName() {
	return m_filePath;
}
void PositionalSoundEffect::setFileName(const string p_filePath) {
	m_filePath = p_filePath;
}

const float& PositionalSoundEffect::getSoundVolume() {
	return m_soundVolume;
}
void PositionalSoundEffect::setSoundVolume(const float p_soundVolume) {
	m_soundVolume = p_soundVolume;
}

const int& PositionalSoundEffect::getSoundIndex() {
	return m_soundIndex;
}
void PositionalSoundEffect::setSoundIndex(const int p_soundIndex) {
	m_soundIndex = p_soundIndex;
}

const float& PositionalSoundEffect::getLifeTime() {
	return m_lifeTime;
}
void PositionalSoundEffect::setLifeTime(const float p_lifeTime) {
	m_lifeTime = p_lifeTime;
}
void PositionalSoundEffect::decreaseLifeTime(const float& p_delta) {
	m_lifeTime -= p_delta;
}

const bool& PositionalSoundEffect::loops() const
{
	return m_loop;
}
