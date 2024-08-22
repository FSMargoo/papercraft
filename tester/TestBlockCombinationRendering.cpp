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

#define WIDTH  800
#define HEIGHT 800

sk_sp<SkTextBlob> TextBlob;
SkPaint			 *Paint;
SkPaint			 *BlurPaint;

PBlockMap* blockMap;

GLFWwindow *GLWindow;

class PTestLightBlock : public PBlock {
public:
	bool IsLightSource() override {
		return true;
	}
	[[nodiscard]] virtual const SkColor& GetLightColor() const {
		return Color;
	}
	[[nodiscard]] virtual unsigned short GetLightLevel() const {
		return 2;
	}

private:
	PTestLightBlock(const PString &Id, const int &X, const int &Y, PImage *Texture, SkColor LColor) : PBlock(Id, X, Y, Texture), Color(LColor) {

	}

public:
	static PTestLightBlock *Clone(PTestLightBlock *Block, const int &X, const int &Y) {
		return new PTestLightBlock(Block->_id, X, Y, Block->_texture, Block->Color);
	}

public:
	SkColor Color;
};

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
	auto redstoneBlock = PBlock::RegisterBlock<PTestLightBlock>("papercraft:redstone_block", redstone);
	redstoneBlock->Color = SK_ColorRED;
	auto emeraldBlock  = PBlock::RegisterBlock<PTestLightBlock>("papercraft:emerald_block", emerald);
	emeraldBlock->Color = SK_ColorGREEN;
	auto diamondBlock  = PBlock::RegisterBlock<PTestLightBlock>("papercraft:diamond_block", diamond);
	diamondBlock->Color = SK_ColorBLUE;
	PBlockMap::BlockMap map;
	IMAGE image;
	loadimage(&image, L"./testMap.png");
	auto buffer = GetImageBuffer(&image);

	for (auto y = 0; y < image.getheight(); ++y) {
		for (auto x = 0; x < image.getwidth(); ++x) {
			auto color = BGR(buffer[x + y * image.getwidth()]);
			auto bufColor = RGB(GetRValue(color), GetGValue(color), GetBValue(color));
			if (bufColor == BLACK) {
				auto block = stoneBlock->Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (bufColor == RGB(32, 32, 255)) {
				auto block = diamondBlock->PBlock::Clone<PTestLightBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (bufColor == RGB(96, 255, 96)) {
				auto block = emeraldBlock->PBlock::Clone<PTestLightBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (bufColor == RGB(255, 32, 32)) {
				auto block = redstoneBlock->PBlock::Clone<PTestLightBlock>(x * 40, y * 40);
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
	canvas->clear(SK_ColorWHITE);
	canvas->drawImage(blockCanvas, 0, 0);
	canvas->flush();
	glContext->GetNativeContext()->flushAndSubmit();

	glfwSwapBuffers(GLWindow);
}
void ErrorCallBack(int Error, const char *Description) {
	fputs(Description, stderr);
}