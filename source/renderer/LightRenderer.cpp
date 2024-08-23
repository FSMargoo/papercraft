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
 * \file LightRenderer.cpp
 * \brief The renderer of light
 */

#include <include/renderer/LightRenderer.h>
#include <include/singleton/Singleton.h>

#include <fstream>

PLightShaderCompiler::PLightShaderCompiler() {
	_shaderCode = ReadShader();
	auto result = SkRuntimeEffect::MakeForShader(SkString(_shaderCode));
	_shader		= result.effect;

	if (!result.errorText.isEmpty()) {
		throw PShaderFailureSyntaxError(result.errorText.c_str());
	}
}
sk_sp<SkShader> PLightShaderCompiler::MakeShader(const PLightShapeType &Type, const float &X, const float &Y, const float &Radius,
												 const float &Range, const float &Level, const SkColor &Color, sk_sp<SkShader> &ImageShader) {
	float r = GetRValue(Color) / 255.f;
	float g = GetGValue(Color) / 255.f;
	float b = GetBValue(Color) / 255.f;
	float iResolution[9] = { 0., X, Y, Radius, Range, Level, r, g, b };
	if (Type == PLightShapeType::Circle) {
		iResolution[0] = 5.f;
	}
	else {
		iResolution[0] = 15.f;
	}

	sk_sp<SkData>			  uniform	  = SkData::MakeWithCopy(iResolution, sizeof(float) * 9);
	SkRuntimeEffect::ChildPtr children[]  = {ImageShader};

	return _shader->makeShader(uniform, {
										   children, 1
									   });
}
std::string PLightShaderCompiler::ReadShader() {
	std::ifstream stream("./assets/shader/pipeline/LightMask.sksl");
	if (!stream.good()) {
		throw PShaderFailureNotFound("./assets/shader/pipeline/LightMask.sksl");
	}

	std::string line;
	std::string file;
	while (!stream.eof()) {
		std::getline(stream, line);
		file += line + "\n";
	}

	return file;
}

PLightRenderer::PLightRenderer(PLightList &List) : _list(List) {

}
sk_sp<SkImage> PLightRenderer::RenderImage(const int &Width, const int &Height, PCamera *Camera) {
	auto surface	 = sk_make_sp<VSurface, const int &, const int &>(Width, Height);
	auto cameraBound = *Camera;
	auto canvas		 = surface->GetNativeSurface()->getCanvas();
	canvas->clear(SK_ColorBLACK);

	for (auto& light : _list) {
		sk_sp<SkShader> sampler = surface->GetNativeSurface()->makeImageSnapshot()->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
		auto shader = PGetSingleton<PLightShaderCompiler>().MakeShader(light.Shape, light.X, light.Y, light.Radius, light.Range, light.Brightness, light.Color, sampler);

		SkPaint shaderPlayground;
		shaderPlayground.setShader(shader);

		canvas->drawPaint(shaderPlayground);
		canvas->flush();
	}

	surface->GetNativeSurface()->flushAndSubmit();
	return surface->GetNativeSurface()->makeImageSnapshot();
}