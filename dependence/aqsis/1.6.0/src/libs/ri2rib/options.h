// -*- C++ -*-
// Aqsis
// Copyright (C) 1997 - 2001, Paul C. Gregory
//
// Contact: pgregory@aqsis.org
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/** \file
 *  \brief Enums related to Options
 *  \author Lionel J. Lacour (intuition01@online.fr)
 */

#ifndef RI2RIB_OPTIONS_H
#define RI2RIB_OPTIONS_H 1

namespace libri2rib {

struct SqOptions
{
	enum EqOutputType { OutputType_Ascii, OutputType_Binary };
	enum EqCompression { Compression_None, Compression_Gzip };
	enum EqIndentation { Indentation_None, Indentation_Space, Indentation_Tab };
};

} // namespace libri2rib
#endif
