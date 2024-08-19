/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file SkiaPaperBase.h
 * \brief The basement definition of the skia and the GLFW
 */

#pragma once

#include <PaperRenderer/include/renderer/vRendererBase.h>
#include <PaperRenderer/include/renderer/vSurface.h>
#include <PaperRenderer/third/glfw/include/GLFW/glfw3.h>

#include <memory>
#include <string>

#include <include/String.h>

typedef struct PMessage {
	GLFWwindow *window;
	USHORT 		message;
	bool  ctrl	  : 1;
	bool  shift	  : 1;
	bool  lbutton : 1;
	bool  mbutton : 1;
	bool  rbutton : 1;
	int x;
	int y;
	int wheel;

	int vkcode;
	int scancode;
	bool extended : 1;
	bool prevdown : 1;

	TCHAR ch;
} ExMessage;

class PGLFWFailure : public std::exception {
public:
	PGLFWFailure(const PString &Info) {
		_info = ostr::format("[Paper Library] GLFW has crashed because of : {}", Info);
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};

/**
 * The image creation failure exception for std library,
 * it accepts a information string to tell user
 * what's going wrong
 */
class PImageCreationFailure : public std::exception {
public:
	PImageCreationFailure(const PString &Info) {
		_info = ostr::format("PImage creation failure: {}", Info);
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};

/**
 * The device creation failure exception for std library,
 * it accepts a information string to tell user
 * what's going wrong
 */
class PDeviceCreationFailure : public std::exception {
public:
	PDeviceCreationFailure(const PString &Info) {
		_info = ostr::format("PDevice creation failure: {}", Info);
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};

/**
 * The image wrapper for EasyX object
 */
class PImage {
public:
	/**
	 * Create the image from specified path
	 * @param Path The path of the image file
	 */
	PImage(const PString &Path);
	/**
	 * Construct the image by the specified geometry information
	 * @param Width The width of the image
	 * @param Height The height of the image
	 */
	PImage(const int &Width, const int &Height);
	/**
	 * Destruct the object, this function will delete the _image pointer
	 */
	~PImage() = default;

public:
	/**
	 * Get the width of the image
	 * @return The width of the image
	 */
	[[nodiscard]] int GetWidth() const;
	/**
	 * Get the height of the image
	 * @return The height of the image
	 */
	[[nodiscard]] int GetHeight() const;

public:
	/**
	 * Get the native Skia image object
	 * @return The sk_sp pointer referred to the image object
	 */
	sk_sp<SkImage> &GetNativeImage();

private:
	sk_sp<SkImage> _image;
	sk_sp<SkData>  _imageData;
};

/**
 * The window wrapper of EasyX
 */
class PWindow {
public:
	/**
	 * Create the window in specified geometry information and title
	 * @param Width The width of the window
	 * @param Height The height of the window
	 * @param Title The title of the window
	 */
	PWindow(const int &Width, const int &Height, const PString &Title);
	virtual ~PWindow();

public:
	/**
	 * Peek a message from the message stack
	 * @return If the message exsits, return true, nor false
	 */
	bool peekmessage(PMessage *message);

public:
	/**
	 * Close the window
	 */
	void Close();

public:
	/**
	 * Get the width of the window
	 * @return The width of the window
	 */
	int GetWidth() const;
	/**
	 * Get the height of the window
	 * @return The height of the window
	 */
	int GetHeight() const;

protected:
	int _width;
	int _height;

protected:
	GLFWwindow			   *_glfwWindow;
	sk_sp<VRenderInterface> _glInterface;
};