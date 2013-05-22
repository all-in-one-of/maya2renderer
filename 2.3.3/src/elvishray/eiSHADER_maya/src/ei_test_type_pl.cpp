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

SURFACE(test_type_pl)
	DECLARE;
	DECLARE_TOKEN(i_token_name,	"");
	DECLARE_INT(i_int_name,		0);
	DECLARE_SCALAR(i_scalar_name, 0.0f);
	DECLARE_VECTOR(i_vector_name, 0.5f, 0.5f, 0.5f);
	DECLARE_POINT(i_point_name,	0.5f, 0.5f, 0.5f);
	DECLARE_NORMAL(i_normal_name, 0.5f, 0.5f, 0.5f);
	DECLARE_COLOR(i_color_name,	0.5f, 0.5f, 0.5f);
	DECLARE_TAG(i_tag_name,		eiNULL_TAG);
	DECLARE_INDEX(i_index_name,	0);
	DECLARE_BOOL(i_bool_name,		eiTRUE);
	END_DECLARE;

	static void init()	{}
	static void exit()	{}

	void init_node()
	{
	}

	void exit_node()
	{
	}

	void main(void *arg)
	{
		out->Ci = clamp(i_color_name(), color(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f));
		out->Oi = clamp(i_scalar_name(), 0.0f, 1.0f);
	}

END(test_type_pl)
