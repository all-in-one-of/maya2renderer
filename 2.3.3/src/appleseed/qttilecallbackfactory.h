
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2012 Francois Beaune, Jupiter Jazz Limited
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#ifndef APPLESEED_STUDIO_MAINWINDOW_RENDERING_QTTILECALLBACKFACTORY_H
#define APPLESEED_STUDIO_MAINWINDOW_RENDERING_QTTILECALLBACKFACTORY_H

// appleseed.renderer headers.
#include "renderer/api/rendering.h"

// Forward declarations.
namespace appleseed { namespace studio { class IRenderWidget; } }

namespace appleseed {
//namespace studio {

class QtTileCallbackFactory
  : public renderer::ITileCallbackFactory
{
  public:
    // Constructor.
    QtTileCallbackFactory(
        //IRenderWidget*  render_widget,
        const bool      highlight_tiles);

    // Delete this instance.
    virtual void release();

    // Return a new instance of the class.
    virtual renderer::ITileCallback* create();

  private:
    //IRenderWidget*      m_render_widget;
    const bool          m_highlight_tiles;
};

//}       // namespace studio
}       // namespace appleseed

#endif  // !APPLESEED_STUDIO_MAINWINDOW_RENDERING_QTTILECALLBACKFACTORY_H