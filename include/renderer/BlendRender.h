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
 * \file Blend.h
 * \brief The blend process of the rendering pipeline
 */

#pragma once

#include "include/game/Blocks/Block.h"
#include <include/renderer/Camera.h>
#include <include/renderer/Shader.h>

#include <thirdparty/PaperRenderer/include/renderer/vSurface.h>

/**
 * The shader compiler of the blend shader
 */
class PBlendShaderCompiler {
public:
	PBlendShaderCompiler();

public:
	/**
	 * Make the light mask shader
	 * @param LightMask The mask of the lighting
	 * @param ImageShader The sub image sampler shader
	 * @return The shader object
	 */
	sk_sp<SkShader> MakeShader(sk_sp<SkShader> &LightMask, sk_sp<SkShader> &ImageShader, sk_sp<SkShader> &NormalShader);

private:
	/**
	 * Read the shader code
	 * @return The code of the shader file
	 */
	std::string ReadShader();

private:
	sk_sp<SkRuntimeEffect> 	_shader;
	std::string 			_shaderCode;
};

/**
 * The blender renderer
 */
class PBlendRenderer {
public:
	/**
	 * Construct the block-reputable renderer with the block map
	 * @param Map The map of the block
	 */
	explicit PBlendRenderer() = default;

public:
	/**
	 * Rendering the image of the lighting image in the rendering pipeline
	 * @param Surface The OpenGL created surface
	 * @param LightMask The shader of the lighting mask
	 * @param ImageShader The shader of the image
	 * @param NormalShader The shader of the normal image
	 * @return The lighting surface
	 */
	sk_sp<SkImage> RenderImage(sk_sp<SkSurface> &Surface, sk_sp<SkShader> &LightMask, sk_sp<SkShader> &ImageShader, sk_sp<SkShader> &NormalShader);
};