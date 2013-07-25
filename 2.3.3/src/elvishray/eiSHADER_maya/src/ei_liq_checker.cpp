/* 
 * Copyright 2010 elvish render Team
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 * http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <eiAPI/ei_shaderx.h>
#include "ei_AOVMacroDef.h"
#include <shader/_3delight/shading_utils.h>
#include <shader/_3delight/utils.h>
#include <shader/my_utils.h>

SURFACE(er_checker)//CheckerNode::getTypeName()
	DECLARE;
	DECLARE_COLOR(i_color1, 0.0f, 0.0f, 0.0f);
	DECLARE_COLOR(i_color2, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(i_xscale, 2.0f);
	DECLARE_SCALAR(i_yscale, 2.0f);
	DECLARE_COLOR(o_result, 0.0f, 0.0f, 0.0f);
	END_DECLARE;

	static void init()
	{
	}

	static void exit()
	{
	}

	void init_node()
	{
	}

	void exit_node()
	{
	}

	void main(void *arg)
	{
		out->Ci = i_color1();
		out->Oi = i_color2();
	}

END(er_checker)