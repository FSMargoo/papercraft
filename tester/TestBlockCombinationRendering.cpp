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
 * \file TestBlockCombinationRendering.cpp
 * \brief Test the Block Combination Rendering
 */

#include <include/renderer/BlockRender.h>
#include <include/assets/AssetManager.h>
#include <include/singleton/Singleton.h>

#include <easyx.h>

sk_sp<VRenderInterface> GLInterface;

#define WIDTH  640
#define HEIGHT 480

sk_sp<SkTextBlob> TextBlob;
SkPaint			 *Paint;
SkPaint			 *BlurPaint;

PBlockMap* blockMap;

GLFWwindow *GLWindow;

/**
 * Init OpenGL interface object
 */
void InitGLInterface();
/**
 * Create GLFW window
 */
void InitWindow();
/**
 * Init Skia resource
 */
void InitResource();
/**
 * Init the block map
 */
void InitBlockMap();
/**
 * The frame buffer call back function
 */
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height);
/**
 * The error call back function of GLFW
 * @param Error The error code
 * @param Description The description string of the error
 */
void ErrorCallBack(int Error, const char *Description);
/**
 * Call Skia API to draw context
 */
void Draw(int Width, int Height);

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	InitWindow();
	InitResource();
	InitBlockMap();
	InitGLInterface();

	Draw(WIDTH, HEIGHT);
	while (!glfwWindowShouldClose(GLWindow)) {
		glfwPollEvents();
	}

	return 0;
}

void InitBlockMap() {
	auto stone		   = PGetSingleton<PAssetManager>().GetBlock("stone");
	auto redstone	   = PGetSingleton<PAssetManager>().GetBlock("redstone_block");
	auto emerald	   = PGetSingleton<PAssetManager>().GetBlock("emerald_block");
	auto diamond	   = PGetSingleton<PAssetManager>().GetBlock("diamond_block");
	auto stoneBlock	   = PBlock::RegisterBlock<PBlock>("papercraft:stone", stone);
	auto redstoneBlock = PBlock::RegisterBlock<PBlock>("papercraft:redstone_block", redstone);
	auto emeraldBlock  = PBlock::RegisterBlock<PBlock>("papercraft:emerald_block", emerald);
	auto diamondBlock  = PBlock::RegisterBlock<PBlock>("papercraft:diamond_block", diamond);
	PBlockMap::BlockMap map;
	IMAGE image;
	loadimage(&image, L"./testMap.png");
	auto buffer = GetImageBuffer(&image);

	for (auto y = 0; y < image.getheight(); ++y) {
		for (auto x = 0; x < image.getwidth(); ++x) {
			SkColor color = buffer[x + y * image.getwidth()];
			if (color == SK_ColorBLACK) {
				auto block = stoneBlock->Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (color == SK_ColorBLUE) {
				auto block = diamondBlock->Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (color == SK_ColorGREEN) {
				auto block = emeraldBlock->Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (color == SK_ColorRED) {
				auto block = redstoneBlock->Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
		}
	}

	blockMap = new PBlockMap(map);
}
void InitGLInterface() {
	GLInterface = sk_make_sp<VRenderInterface>();
}
void InitWindow() {
	glfwSetErrorCallback(ErrorCallBack);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	GLWindow = glfwCreateWindow(WIDTH, HEIGHT, "Skia Tester", nullptr, nullptr);
	if (!GLWindow) {
		printf("Failed to create GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetFramebufferSizeCallback(GLWindow, FrameBufferCallBack);

	glfwMakeContextCurrent(GLWindow);
}
void InitResource() {
}
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height) {
	Draw(Width, Height);
}
void Draw(int Width, int Height) {
	sk_sp<VRenderTarget> glRenderTarget =
		sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = Width, .Height = Height, .X = 0, .Y = 0});
	sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(GLInterface);
	sk_sp<VSurface>		  glSurface =
		sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);

	auto canvas = glSurface->GetNativeSurface()->getCanvas();
	PBlockReputableRenderer renderer(*blockMap);
	auto camera = std::make_unique<PCamera>(0, 0, 800, 800);
	auto blockCanvas = renderer.RenderImage(800, 800, camera.get());

	canvas->flush();
	glSurface->GetNativeSurface()->draw(blockCanvas.get()->GetNativeSurface()->getCanvas(), 0, 0);
	glContext->GetNativeContext()->flushAndSubmit();

	glfwSwapBuffers(GLWindow);
}
void ErrorCallBack(int Error, const char *Description) {
	fputs(Description, stderr);
}