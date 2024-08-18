///
/// The basic definition of EasyX
///

#pragma once

#include <graphics.h>
#include <conio.h>
#include <string>
#include <memory>

#include "String.h"

#define GetAValue(color) (((color) >> 24) & 0xFF)

/// Generate a COLORREF object by the A, R, G, B channel value
/// \param a The alpha channel value of the color
/// \param r The red channel value of the color
/// \param g The green channel value of the color
/// \param b The blue channel value of the color
/// \return The COLORREF object in specified color
COLORREF GetARGBColor(const int &a, const int &r, const int &g, const int &b);

/// The image creation failure exception for std library,
/// it accepts a information string to tell user
/// what's going wrong
class PImageCreationFailure : public std::exception
{
public:
    PImageCreationFailure(const PString &Info)
    {
        _info = ostr::format("PImage creation failure: {}", Info);
    }

    const char* what() const throw() override
    {
        return _info.c_str();
    }
    
private:
    PString _info;
};

/// The device creation failure exception for std library,
/// it accepts a information string to tell user
/// what's going wrong
class PDeviceCreationFailure : public std::exception
{
public:
    PDeviceCreationFailure(const PString &Info)
    {
        _info = ostr::format("PDevice creation failure: {}", Info);
    }
    
    const char* what() const throw() override
    {
        return _info.c_str();
    }
    
private:
    PString _info;
};

/// The image wrapper for EasyX object
class PImage
{
public:
    /// Create the image from specified path
    /// \param Path The path of the image file
    PImage(const PString &Path);
    /// Construct the image by the specified geometry information
    /// \param Width The width of the image
    /// \param Height The height of the image
    PImage(const int &Width, const int &Height);
    /// Destruct the object, this function will delete the _image pointer
    ~PImage();

public:
    /// Get the width of the image
    /// @return The width of the image
    int GetWidth() const;
    /// Get the height of the image
    /// @return The height of the image
    int GetHeight() const;

private:
    friend class PDevice;
    
private:
    IMAGE *_image;
};

/// The device wrapper of EasyX device
class PDevice
{
public:
    /// Create the device from PImage object
    /// @param Image The image object to be created in device
    /// @return The device object, the lifetime of the device should be
    /// maintained by user
    static PDevice* MakeFromImage(PImage *Image);
    /// Create the device from default device(The screen)
    /// @return The device object, the lifetime of the device should be
    /// maintained by user
    static PDevice* MakeFromScreen();

public:
    /// Raise current device as working device, every painting operation will
    /// be applyed on this device
    void RaiseAsWorking() const;
    /// Get the HDC of the device
    /// @return The HDC object of the device
    HDC GetHDC() const;
    /// Flush the device, this function will only call FlushBatchDraw in EasyX
    /// when the device is connected to the default device in EasyX
    void Flush() const;

public:
    ~PDevice() = default;
    
private:
    /// Create a device from PImage object
    /// @param Image The image object
    PDevice(PImage *Image);
    /// Create a device in default device(The screen)
    PDevice();

private:
    IMAGE  *_image;
    HDC     _hdc;
};

/// The window wrapper of EasyX
class PWindow
{
public:
    /// Create the window in specified geometry information and title 
    /// @param Width The width of the window
    /// @param Height The height of the window
    /// @param Title The title of the window
    PWindow(const int &Width, const int &Height, const PString &Title);

public:
    /// Get the device of the window
    /// @return The pointer of the device, user should maintain the lifetime
    /// of the pointer on their own
    static PDevice* GetDevice();
};