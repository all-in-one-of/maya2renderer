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
#include "../ei_AOVMacroDef.h"
#include <shader/_3delight/shading_utils.h>
#include <shader/_3delight/utils.h>
#include <shader/my_utils.h>

SURFACE(erFlatColor)
	DECLARE;
	DECLARE_COLOR(i_color,			1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(i_transparency,	1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(o_outColor,		0.0f, 0.0f, 0.0f);
	DECLARE_COLOR(o_outTransparency,0.0f, 0.0f, 0.0f);
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
		out->Oi = i_transparency();
		out->Ci = i_color() * out->Oi;

		o_outColor() = out->Ci;
		o_outTransparency() = out->Oi;
	}

END(erFlatColor)