/**
 * \file MCIManager.h
 * \brief MCI Device manager for paper library
 */

#pragma once

#include <include/String.h>

#include <Windows.h>
#include <mmsystem.h>
#include <thread>

#pragma comment(lib, "WINMM.LIB")

class PMCIManagerFailureInvalidInstance : public std::exception {
public:
	PMCIManagerFailureInvalidInstance() {
		_info = ostr::format("PMCIManager failure: Invalid instance");
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};

/**
 * The instance of a MCI object
 */
class PMCIInstance {
public:
	/**
	 * Create the MCI instance with its alias
	 * @param Alias The alis of the MCI instance
	 */
	explicit PMCIInstance(PString Alias);
	~PMCIInstance() = default;
public:
	/**
	 * Change this instance type into BGM
	 */
	void ChangedAsBGM();
	/**
	 * Change this instance type into general sound
	 */
	void ChangedAsGeneral(const int &X, const int &Y, const float &Range = 4.f);
	/**
	 * Update this instance to be ready for playing again
	 */
	void Update();

private:
	friend class PMCIManager;

private:
	int		_x	   = 0;
	int		_y	   = 0;
	float	_range = 4.f;
	bool	_isBGM = false;
	bool	_invalid;
	PString _alias;
	PString _path;
};

/**
 * The abstracted class for MCI manager to call the API from
 * a specified camera
 */
class PMCIManagerCameraInterface {
public:
	/**
	 * Get the Y position of the camera
	 * @return The Y position
	 */
	virtual int GetY() const = 0;
	/**
	 * Get the X position of the camera
	 * @return The X position
	 */
	virtual int GetX() const = 0;
};

/**
 * The MCI manager helper class, which allow to automatically adjust the volume of
 * MCI sounds the according to the distance between player to the specified way; Of
 * course it also have a BGM sound support
 */
class PMCIManager {
public:
	/**
	 * Open a sound file as BGM, the BGM sound won't be effected by the distance of camera
	 * position to specified position
	 * @param FilePath The file path of the BGM
	 * @param Alias The alias of the MCI device
	 * @return The MCI instance, which needing user to control the lifetime; A MCI instance should
	 * be free by the PMCIManager::Close method
	 */
	static PMCIInstance *OpenBGM(const PString &FilePath, const PString &Alias);
	/**
	 * Open a sound file as general sound, the general sound will be effected by the distance
	 * of camera position to specified position
	 * @param FilePath The file path of the BGM
	 * @param Alias The alias of the MCI device
	 * @return The MCI instance, which needing user to control the lifetime; A MCI instance should
	 * be free by the PMCIManager::Close method
	 */
	static PMCIInstance *Open(const PString &FilePath, const PString &Alias, const int &X, const int &Y,
							  const float &Range = 4.f);
	/**
	 * Close and free the MCI instance
	 * @param Instance The instance to be free
	 */
	static void Close(PMCIInstance *Instance);
	/**
	 * Play the specified sound
	 * @param Instance The instance of MCI sound
	 * @param Camera Deprecated parameter, only reserved
	 * @param Repeat Repeat the sound or not, if the value is true, this sound will repeat playing
	 */
	static void Play(PMCIInstance *Instance, PMCIManagerCameraInterface *Camera = nullptr, bool Repeat = false);
	/**
	 * Play the specified sound in BGM mode and close it from MCI device after play was done
	 * @param FilePath The file path of the sound
	 * @param Alias The alias of the sound
	 */
	static void PlayAndCloseBGM(const PString &FilePath, const PString &Alias);
	/**
	 * Play the specified sound and
	 * @param Instance The instance of the MCI sound
	 * @param Camera The camera interface base class pointer
	 */
	static void PlayWait(PMCIInstance *Instance, PMCIManagerCameraInterface *Camera);
	/**
	 * Update the instance to be played again
	 * @param Instace The instance pointer
	 * @return The new MCI instance pointer
	 */
	static PMCIInstance *Update(PMCIInstance *Instace);
};