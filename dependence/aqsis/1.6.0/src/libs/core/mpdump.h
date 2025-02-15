// Aqsis
// Copyright (C) 1997 - 2001, Paul C. Gregory
//
// Contact: pgregory@aqsis.org
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/**
	\file mpdump.h
	Contains the CqMPDump class that's used for writing a dump file for debugging.
 */

#ifndef MPDUMP_H_INCLUDED
#define MPDUMP_H_INCLUDED

#include <aqsis/aqsis.h>

#include <stdio.h>

#include <aqsis/math/vecfwd.h>

namespace Aqsis {

class CqBucketProcessor;
class CqVector2D;
class CqMicroPolygon;

/**
   Dump micro polygons into a file for debugging.
 
   This class is used to write a file containing sample positions and micro
   polygons created during rendering. This file is then processed by an
   external tool to extract debugging information (such as a visualization of
   the micro polygons that have been relevant for one particular pixel).
 
   The generated file is in binary format so that the micro polygon vertices
   are exported exactly as they were generated by the renderer.
 
   The header of the file contains the value sizeof(TqFloat) as an int. The
   remainder of the file are data sets that start with an ID of type short that
   determines the type of the next data item.  Currently there are the
   following data sets:
 
   ID 1: Micro polygon definition  (4 vertices, color, opacity)
   ID 2: Pixel sample definition (Pixel position, sample index, sample position)
   ID 3: Image information (XRes, YRes)
 
   \author Matthias Baas
 */
class CqMPDump
{
	public:
		/** Constructor. */
		CqMPDump();

		/** Destructor. */
		~CqMPDump();

		/**
		   Open the dump file.

		   A previously opened file is closed.
		 */
		void open();

		/**
		   Close the dump file.
		 */
		void close();

		bool IsOpen()
		{
			return(NULL != m_outFile);
		}

		/**
			Dump global image infos.

			The method retrieves the information itself, so it must be called at
			a time this information (image resolution) is available.

			If there is no file opened, the method returns immediately.
		 */
		void dumpImageInfo();

		/**
		    Dump all pixel samples of the given bucket.

		    This method has to be called after the sample initialization in
			CqImageBuffer::RenderImage().
		 */
		void dumpPixelSamples(const CqBucketProcessor& bp);

		/**
		   Dump a pixel sample.

		      If there is no file opened, the method returns immediately.

		      \param x Pixel position that the sample belongs to
		      \param y Pixel position that the sample belongs to
		   \param idx Sample index within the pixel (x,y)
		   \param sd Sample data
			 */
		void dump(int x, int y, int idx, const CqVector2D& pos);

		/**
			Dump a micro polygon.

		    If there is no file opened, the method returns immediately.

			\param mp Micro polygon to be dumped
		*/
		void dump(const CqMicroPolygon& mp);

		/**
		   Dump a 3D vector.
		   
		   \pre m_outFile must not be NULL
		   \param v Vector that should be dumped
		*/
		void dumpVec3(const CqVector3D& v);

		/**
		   Dump a color.
		   
		   \pre m_outFile must not be NULL
		   \param c Vector that should be dumped
		*/
		void dumpCol(const CqColor& c);
	private:
		/// Output file handle
		FILE* m_outFile;
		/// Micro polygon counter
		int m_mpcount;

};


} // namespace Aqsis

#endif // MPDUMP_H_INCLUDED
