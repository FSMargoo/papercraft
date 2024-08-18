#include "../../include/mic/MCIManager.h"

PMCIInstance::PMCIInstance(PString Alias) : _alias(std::move(Alias)), _invalid(true) {

}
void PMCIInstance::ChangedAsBGM()
{
	_isBGM = true;
}
void PMCIInstance::ChangedAsGeneral(const int& X, const int& Y, const float& Range)
{
	_isBGM = false;
	_x	   = X;
	_y	   = Y;
	_range = Range;
}
void PMCIInstance::Update()
{
	_invalid = true;
	
	mciSendString(PStringToCStr<TCHAR>(ostr::format("close {}", _alias)), nullptr, 0, nullptr);
	mciSendString(PStringToCStr<TCHAR>(ostr::format("open \"{}\" alias {}", _path, _alias)), nullptr, 0, nullptr);
}
PMCIInstance* PMCIManager::OpenBGM(const PString &FilePath, const PString &Alias) {
	mciSendString(PStringToCStr<TCHAR>(ostr::format("open \"{}\" alias {}", FilePath, Alias)), nullptr, 0, nullptr);
	auto instance =  new PMCIInstance(Alias);
	instance->_isBGM = true;
	instance->_path	 = FilePath;

	return instance;
}
PMCIInstance* PMCIManager::Open(const PString &FilePath, const PString &Alias, const int &X, const int &Y, const float &Range) {
	mciSendString(PStringToCStr<TCHAR>(ostr::format("open \"{}\" alias {}", FilePath, Alias)), nullptr, 0, nullptr);
	auto instance =  new PMCIInstance(Alias);
	instance->_isBGM = false;
	instance->_x     = X;
	instance->_y     = Y;
	instance->_range = Range;
	instance->_path	 = FilePath;

	return instance;
}
void PMCIManager::Close(PMCIInstance *Instance) {
	mciSendString(PStringToCStr<TCHAR>(ostr::format("close {}", Instance->_alias)), nullptr, 0, nullptr);
	delete Instance;
}
void PMCIManager::Play(PMCIInstance *Instance, PMCIManagerCameraInterface *Camera, bool Repeat) {
	if (!Instance->_invalid)
	{
		throw PMCIManagerFailureInvalidInstance();
	}
	Instance->_invalid = false;
	if (Repeat) {
		mciSendString(PStringToCStr<TCHAR>(ostr::format("play {} repeat", Instance->_alias)), nullptr, 0, nullptr);
	}
	else {
		mciSendString(PStringToCStr<TCHAR>(ostr::format("play {}", Instance->_alias)), nullptr, 0, nullptr);
	}
}
PMCIInstance* PMCIManager::Update(PMCIInstance *Instace)
{
	bool BGM			= Instace->_isBGM;
	auto path		= Instace->_path;
	auto alias	= Instace->_alias;
	auto x			= Instace->_x;
	auto y			= Instace->_y;
	Close(Instace);
	if (BGM)
	{
		return OpenBGM(path, alias);
	}
	else
	{
		return Open(Instace->_path, Instace->_alias, x, y);
	}
}
void PMCIManager::PlayAndCloseBGM(const PString& FilePath, const PString& Alias)
{
	std::thread([](const PString& FilePath, const PString& Alias) -> void
	{
		auto BGM = OpenBGM(FilePath, Alias);
		PlayWait(BGM, nullptr);
		Close(BGM);
	}, FilePath, Alias).detach();
}
void PMCIManager::PlayWait(PMCIInstance *Instance, PMCIManagerCameraInterface *Camera)
{
	if (!Instance->_invalid)
	{
		throw PMCIManagerFailureInvalidInstance();
	}
	Instance->_invalid = false;
	wchar_t buffer[100];
	int     length;
	int     now = 0;
	mciSendString(PStringToCStr<TCHAR>(ostr::format("play {}", Instance->_alias)), nullptr, 0, nullptr);
	mciSendString(PStringToCStr<TCHAR>(ostr::format("status {} length", Instance->_alias)), buffer, 100, nullptr);
	length = _wtoi(buffer);

	while (now < length) {
		mciSendString(PStringToCStr<TCHAR>(ostr::format("status {} position", Instance->_alias)), buffer, 100, nullptr);
		now = _wtoi(buffer);
		if (!Instance->_isBGM) {
			auto distance = sqrt(pow(Camera->GetX() - Instance->_x, 2) + pow(Camera->GetY() - Instance->_y, 2));
			distance = distance > Instance->_range ? 0.f : (Instance->_range - distance) / Instance->_range;
			auto newVolume = static_cast<int>(ceil(1000 * distance));
			mciSendString(PStringToCStr<TCHAR>(ostr::format("setaudio {} volume to {}",
			                           Instance->_alias, newVolume)), nullptr, 0, nullptr);
		}

		Sleep(1);
	}
}