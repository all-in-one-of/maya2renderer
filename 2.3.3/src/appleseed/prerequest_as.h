#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#pragma once

// appleseed.renderer headers. Only include header files from renderer/api/.
#include "appleseed/renderer/api/edf.h"
#include "appleseed/renderer/api/bsdf.h"
#include "appleseed/renderer/api/camera.h"
#include "appleseed/renderer/api/color.h"
#include "appleseed/renderer/api/environment.h"
#include "appleseed/renderer/api/environmentedf.h"
#include "appleseed/renderer/api/environmentshader.h"
#include "appleseed/renderer/api/frame.h"
#include "appleseed/renderer/api/light.h"
#include "appleseed/renderer/api/log.h"
#include "appleseed/renderer/api/material.h"
#include "appleseed/renderer/api/object.h"
#include "appleseed/renderer/api/project.h"
#include "appleseed/renderer/api/rendering.h"
#include "appleseed/renderer/api/scene.h"
#include "appleseed/renderer/api/surfaceshader.h"
#include "appleseed/renderer/api/utility.h"
#include "appleseed/renderer/api/texture.h"
#include "appleseed/renderer/modeling/bsdf/bsdfmix.h"

// appleseed.foundation headers.
#include "appleseed/foundation/math/vector.h"
#include "appleseed/foundation/math/matrix.h"
#include "appleseed/foundation/math/scalar.h"
#include "appleseed/foundation/math/transform.h"
#include "appleseed/foundation/utility/containers/specializedarrays.h"
#include "appleseed/foundation/utility/autoreleaseptr.h"
#include "appleseed/foundation/utility/searchpaths.h"
#include "appleseed/foundation/utility/log.h"
#include "appleseed/foundation/image/image.h"
#include "appleseed/foundation/image/tile.h"

// mesh
#include "appleseed/foundation/mesh/imeshwalker.h"
#include "appleseed/foundation/mesh/meshbuilderbase.h"
#include "appleseed/foundation/mesh/objmeshfilewriter.h"

namespace asf = foundation;
namespace asr = renderer;

#endif//_USE_APPLESEED_