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
 * \file SkiaPaperBase.cpp
 * \brief The basement definition of the skia and the GLFW
 */

#include <include/SkiaPaperBase.h>

#include <mutex>

std::vector<PMessage> __PMessageStack;

void PGLFWFramebufferSizeCallback(GLFWwindow *Window, int Width, int Height) {
}
void PGLFWMouseMoveCallback(GLFWwindow *Window, double X, double Y) {
	

	PMessage message{};

	message.window = Window;

	message.message = WM_MOUSEMOVE;

	message.x = static_cast<int>(X);
	message.y = static_cast<int>(Y);

	__PMessageStack.emplace_back(message);
}
void PGLFWMouseClickCallback(GLFWwindow *Window, int Button, int Action, int Mods) {
	PMessage message{};
	message.window = Window;

	switch (Mods) {
	case GLFW_MOD_SHIFT: {
		message.shift = true;
		break;
	}
	case GLFW_MOD_CONTROL: {
		message.ctrl = true;
		break;
	}
	default:
		break;
	}

	bool handle = true;
	switch (Button) {
	case GLFW_MOUSE_BUTTON_LEFT: {
		if (Action == GLFW_PRESS) {
			message.message = WM_LBUTTONDOWN;
		} else {
			message.message = WM_LBUTTONUP;
		}

		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT: {
		if (Action == GLFW_PRESS) {
			message.message = WM_RBUTTONDOWN;
		} else {
			message.message = WM_RBUTTONUP;
		}

		break;
	}
	case GLFW_MOUSE_BUTTON_MIDDLE: {
		if (Action == GLFW_PRESS) {
			message.message = WM_MBUTTONDOWN;
		} else {
			message.message = WM_MBUTTONUP;
		}

		break;
	}
	default:
		handle = false;
		break;
	}

	if (handle) {
		double x;
		double y;
		glfwGetCursorPos(Window, &x, &y);

		message.x = static_cast<int>(x);
		message.y = static_cast<int>(y);

		
		__PMessageStack.emplace_back(message);
	}
}
void PGLFWMouseScrollCallback(GLFWwindow *Window, double XOffset, double YOffset) {
	

	PMessage message{};

	message.window	= Window;
	message.message = WM_MOUSEWHEEL;
	message.wheel	= YOffset;

	double x;
	double y;
	glfwGetCursorPos(Window, &x, &y);

	message.x = static_cast<int>(x);
	message.y = static_cast<int>(y);

	__PMessageStack.emplace_back(message);
}
void PGLFWKeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods) {
	

	PMessage message{};
	switch (Mods) {
	case GLFW_MOD_SHIFT: {
		message.shift = true;
		break;
	}
	case GLFW_MOD_CONTROL: {
		message.ctrl = true;
		break;
	}
	default:
		break;
	}

	message.window = Window;
	if (Action == GLFW_RELEASE) {
		message.message = WM_KEYUP;
	} else {
		message.message = WM_KEYDOWN;
	}
	message.vkcode = Key;

	double x;
	double y;
	glfwGetCursorPos(Window, &x, &y);

	message.x = static_cast<int>(x);
	message.y = static_cast<int>(y);

	__PMessageStack.emplace_back(message);
}
void PGLFWSetCharCallback(GLFWwindow *Window, unsigned int Codepoint) {
	

	PMessage message;
	message.window	= Window;
	message.message = WM_CHAR;
	message.ch		= static_cast<wchar_t>(Codepoint);

	__PMessageStack.emplace_back(message);
}

PImage::PImage(const PString &Path) {
	_imageData = SkData::MakeFromFileName(Path.c_str());
	_image	   = SkImage::MakeFromEncoded(_imageData);

	if (!_imageData || !_image) {
		throw PImageCreationFailure(Path);
	}
}
PImage::PImage(const int &Width, const int &Height) {
	sk_sp<SkSurface> rasterSurface = SkSurface::MakeRasterN32Premul(Width, Height);
	rasterSurface->getCanvas()->clear(SK_ColorTRANSPARENT);

	_image	   = rasterSurface->makeImageSnapshot();
	_imageData = _image->encodeToData();
}
int PImage::GetWidth() const {
	return _image->width();
}
int PImage::GetHeight() const {
	return _image->height();
}
sk_sp<SkImage> &PImage::GetNativeImage() {
	return _image;
}

PWindow::PWindow(const int &Width, const int &Height, const PString &Title) : _width(Width), _height(Height) {
	static bool isGLFWInit = false;
	if (!isGLFWInit) {
		isGLFWInit = true;
		if (!glfwInit()) {
			throw PGLFWFailure("Failed to init GLFW!");
		}
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_glfwWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
	if (!_glfwWindow) {
		throw PGLFWFailure("Failed to create GLFW window!");
	}

	glfwSetFramebufferSizeCallback(_glfwWindow, PGLFWFramebufferSizeCallback);
	glfwSetCursorPosCallback(_glfwWindow, PGLFWMouseMoveCallback);
	glfwSetMouseButtonCallback(_glfwWindow, PGLFWMouseClickCallback);
	glfwSetScrollCallback(_glfwWindow, PGLFWMouseScrollCallback);
	glfwSetKeyCallback(_glfwWindow, PGLFWKeyCallback);
	glfwSetCharCallback(_glfwWindow, PGLFWSetCharCallback);

	glfwMakeContextCurrent(_glfwWindow);
	_glInterface = sk_make_sp<VRenderInterface>();
}
PWindow::~PWindow() {
	if (_glfwWindow) {
		glfwDestroyWindow(_glfwWindow);
	}
}
void PWindow::Close() {
	glfwDestroyWindow(_glfwWindow);
	_glfwWindow = nullptr;
}
bool PWindow::peekmessage(PMessage *message) {
	for (auto object = __PMessageStack.begin(); object != __PMessageStack.end(); ++object) {
		if (object->window == _glfwWindow) {
			*message = *object;
			__PMessageStack.erase(object);

			return true;
		}
	}

	return false;
}
int PWindow::GetWidth() const {
	return _width;
}
int PWindow::GetHeight() const {
	return _height;
}