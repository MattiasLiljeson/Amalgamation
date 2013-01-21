#include "PositionalSound.h"

PositionalSound::PositionalSound(const string p_filePath, const bool p_loop,
								 const float p_soundVolume )
{
	m_filePath = p_filePath;
	m_loop = p_loop;
	m_soundVolume = p_soundVolume;
}

const string& PositionalSound::getFileName() {
	return m_filePath;
}
void PositionalSound::setFileName(const string p_filePath) {
	m_filePath = p_filePath;
}

const float& PositionalSound::getSoundVolume() {
	return m_soundVolume;
}
void PositionalSound::setSoundVolume(const float p_soundVolume) {
	m_soundVolume = p_soundVolume;
}

const int& PositionalSound::getFileId() {
	return m_fileId;
}
void PositionalSound::setFileId(const int p_fileId) {
	m_fileId = p_fileId;
}

const float& PositionalSound::getLifeTime() {
	return m_lifeTime;
}
void PositionalSound::setLifeTime(const float p_lifeTime) {
	m_lifeTime = p_lifeTime;
}
void PositionalSound::decreaseLifeTime(const float& p_delta) {
	m_lifeTime -= p_delta;
}