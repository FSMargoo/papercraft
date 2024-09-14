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
 * \file Entity.h
 * \brief The entity class in paper craft
 */

#pragma once

#include "include/SkiaPaperBase.h"

#include <include/game/object/Object.h>
#include <include/game/entity/IUpdatable.h>

/**
 * 实体类，委托给IUpdatable处理更新和控制，通过SetIUpatable设置IUpdatable
 */


//不知道如何实现渲染，貌似有用到2D骨骼系统


class PEntity final : public PObject {
public:
	PEntity();

	virtual ~PEntity();

	/**
	 * 更新逻辑
	 * @param deltaTime 时间间隔
	 */
	void Update(float deltaTime);

	/**
	 * 控制逻辑
	 */
	void On_Message(const PMessage &Message);

	/**
	 * 传入PIUpdatable的子类，以此实现不同的更新、控制逻辑
	 * @param updatable
	 */
	void SetIUpatable(PIUpdatable *updatable);

private:
	PIUpdatable *_updatable;
};