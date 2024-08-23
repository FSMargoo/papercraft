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
 * \file Blend.cpp
 * \brief The blend process of the rendering pipeline
 */

#include <include/renderer/BlendRender.h>
#include <include/singleton/Singleton.h>

#include <fstream>

PBlendShaderCompiler::PBlendShaderCompiler() {
	_shaderCode = ReadShader();
	auto result = SkRuntimeEffect::MakeForShader(SkString(_shaderCode));
	_shader		= result.effect;

	if (!result.errorText.isEmpty()) {
		printf("Skia failed: %s\n", result.errorText.c_str());
		_flushall();
		throw PShaderFailureSyntaxError(result.errorText.c_str());
	}
}
sk_sp<SkShader> PBlendShaderCompiler::MakeShader(sk_sp<SkShader> &LightMask, sk_sp<SkShader> &ImageShader, sk_sp<SkShader> &NormalShader) {
	SkRuntimeEffect::ChildPtr children[] = {LightMask, NormalShader, ImageShader};
	return _shader->makeShader(nullptr, {children, 3});
}
std::string PBlendShaderCompiler::ReadShader() {
	std::ifstream stream("./assets/shaders/pipeline/Blend.sksl");
	if (!stream.good()) {
		throw PShaderFailureNotFound("./assets/shader/pipeline/Blend.sksl");
	}

	std::string line;
	std::string file;
	while (!stream.eof()) {
		std::getline(stream, line);
		file += line + "\n";
	}

	return file;
}

sk_sp<SkImage> PBlendRenderer::RenderImage(sk_sp<SkSurface> &Surface, sk_sp<SkShader> &LightMask, sk_sp<SkShader> &ImageShader, sk_sp<SkShader> &NormalShader) {
	sk_sp<VRenderInterface> GLInterface = sk_make_sp<VRenderInterface>();
	auto					glCanvas	= Surface->getCanvas();
	auto					shader		= PGetSingleton<PBlendShaderCompiler>().MakeShader(LightMask, ImageShader, NormalShader);

	SkPaint shaderPlayground;
	shaderPlayground.setShader(shader);

	glCanvas->drawPaint(shaderPlayground);
	glCanvas->flush();

	Surface->flushAndSubmit();
	return Surface->makeImageSnapshot();
}