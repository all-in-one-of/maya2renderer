/*
Metric
Copyright (C) 2006 Yangli Hector Yee

This program is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program;
if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "Metric.h"
#include "CompareArgs.h"
#include "RGBAImage.h"
#include "LPyramid.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265f
#endif

/*
* Given the adaptation luminance, this function returns the
* threshold of visibility in cd per m^2
* TVI means Threshold vs Intensity function
* This version comes from Ward Larson Siggraph 1997
*/ 

float tvi(float adaptation_luminance)
{
      // returns the threshold luminance given the adaptation luminance
      // units are candelas per meter squared

      float log_a, r, result; 
      log_a = log10f(adaptation_luminance);

      if (log_a < -3.94f) {
            r = -2.86f;
      } else if (log_a < -1.44f) {
            r = powf(0.405f * log_a + 1.6f , 2.18f) - 2.86f;
      } else if (log_a < -0.0184f) {
            r = log_a - 0.395f;
      } else if (log_a < 1.9f) {
            r = powf(0.249f * log_a + 0.65f, 2.7f) - 0.72f;
      } else {
            r = log_a - 1.255f;
      }

      result = powf(10.0f , r); 

      return result;

} 

// computes the contrast sensitivity function (Barten SPIE 1989)
// given the cycles per degree (cpd) and luminance (lum)
float csf(float cpd, float lum)
{
	float a, b, result; 
	
	a = 440.0f * powf((1.0f + 0.7f / lum), -0.2f);
	b = 0.3f * powf((1.0f + 100.0f / lum), 0.15f);
		
	result = a * cpd * expf(-b * cpd) * sqrtf(1.0f + 0.06f * expf(b * cpd)); 
	
	return result;	
}

/*
* Visual Masking Function
* from Daly 1993
*/
float mask(float contrast)
{
      float a, b, result;
      a = powf(392.498f * contrast,  0.7f);
      b = powf(0.0153f * a, 4.0f);
      result = powf(1.0f + b, 0.25f); 

      return result;
} 

// convert Adobe RGB (1998) with reference white D65 to XYZ
void AdobeRGBToXYZ(float r, float g, float b, float &x, float &y, float &z)
{
	// matrix is from http://www.brucelindbloom.com/
	x = r * 0.576700f + g * 0.185556f + b * 0.188212f;
	y = r * 0.297361f + g * 0.627355f + b * 0.0752847f;
	z = r * 0.0270328f + g * 0.0706879f + b * 0.991248f;
}

bool Yee_Compare(CompareArgs &args)
{
	if ((args.ImgA->Get_Width() != args.ImgB->Get_Width()) ||
		(args.ImgA->Get_Height() != args.ImgB->Get_Height())) {
		args.ErrorStr = "Image dimensions do not match\n";
		return false;
	}
	
	unsigned int i, dim;
	dim = args.ImgA->Get_Width() * args.ImgA->Get_Height();
	bool identical = true;
	for (i = 0; i < dim; i++) {
		if (args.ImgA->Get(i) != args.ImgB->Get(i)) {
		  identical = false;
		  break;
		}
	}
	if (identical) {
		args.ErrorStr = "Images are binary identical\n";
		return true;
	}
	
	// assuming colorspaces are in Adobe RGB (1998) convert to XYZ
	float *aX = new float[dim];
	float *aY = new float[dim];
	float *aZ = new float[dim];
	float *bX = new float[dim];
	float *bY = new float[dim];
	float *bZ = new float[dim];
	float *aLum = new float[dim];
	float *bLum = new float[dim];
	
	if (args.Verbose) printf("Converting RGB to XYZ\n");
	
	unsigned int x, y, w, h;
	w = args.ImgA->Get_Width();
	h = args.ImgA->Get_Height();
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			float r, g, b;
			i = x + y * w;
			r = powf(args.ImgA->Get_Red(i) / 255.0f, args.Gamma);
			g = powf(args.ImgA->Get_Green(i) / 255.0f, args.Gamma);
			b = powf(args.ImgA->Get_Blue(i) / 255.0f, args.Gamma);						
			AdobeRGBToXYZ(r,g,b,aX[i],aY[i],aZ[i]);
			r = powf(args.ImgB->Get_Red(i) / 255.0f, args.Gamma);
			g = powf(args.ImgB->Get_Green(i) / 255.0f, args.Gamma);
			b = powf(args.ImgB->Get_Blue(i) / 255.0f, args.Gamma);						
			AdobeRGBToXYZ(r,g,b,bX[i],bY[i],bZ[i]);
			aLum[i] = aY[i] * args.Luminance;
			bLum[i] = bY[i] * args.Luminance;
		}
	}
	
	if (args.Verbose) printf("Constructing Laplacian Pyramids\n");
	
	LPyramid *la = new LPyramid(aLum, w, h);
	LPyramid *lb = new LPyramid(bLum, w, h);
	
	float num_one_degree_pixels = 2 * tan( args.FieldOfView * 0.5 * M_PI / 180) * 180 / M_PI;
	float pixels_per_degree = w / num_one_degree_pixels;
	
	if (args.Verbose) printf("Performing test\n");
	
	float num_pixels = 1;
	unsigned int adaptation_level = 0;
	for (i = 0; i < MAX_PYR_LEVELS; i++) {
		adaptation_level = i;
		if (num_pixels > num_one_degree_pixels) break;
		num_pixels *= 2;
	}
	
	float cpd[MAX_PYR_LEVELS];
	cpd[0] = 0.5 * pixels_per_degree;
	for (i = 1; i < MAX_PYR_LEVELS; i++) cpd[i] = 0.5 * cpd[i - 1];
	float csf_max = csf(3.248, 100.0);
	
	float F_freq[MAX_PYR_LEVELS - 2];
	for (i = 0; i < MAX_PYR_LEVELS - 2; i++) F_freq[i] = csf_max / csf( cpd[i], 100.0);
	
	unsigned int pixels_failed = 0;
	for (y = 0; y < h; y++) {
	  for (x = 0; x < w; x++) {
		float contrast[MAX_PYR_LEVELS - 2];
		float sum_contrast = 0;
		for (i = 0; i < MAX_PYR_LEVELS - 2; i++) {
			float n1 = fabsf(la->Get_Value(x,y,i) - la->Get_Value(x,y,i + 1));
			float n2 = fabsf(lb->Get_Value(x,y,i) - lb->Get_Value(x,y,i + 1));
			float numerator = (n1 > n2) ? n1 : n2;
			float d1 = fabsf(la->Get_Value(x,y,i+2));
			float d2 = fabsf(lb->Get_Value(x,y,i+2));
			float denominator = (d1 > d2) ? d1 : d2;
			if (denominator < 1e-5) denominator = 1e-5;
			contrast[i] = numerator / denominator;
			sum_contrast += contrast[i];
		}
		if (sum_contrast < 1e-5) sum_contrast = 1e-5;
		float F_mask[MAX_PYR_LEVELS - 2];
		float adapt = la->Get_Value(x,y,adaptation_level) + lb->Get_Value(x,y,adaptation_level);
		adapt *= 0.5;
		if (adapt < 1e-5) adapt = 1e-5;
		for (i = 0; i < MAX_PYR_LEVELS - 2; i++) {
			F_mask[i] = mask(contrast[i] * csf(cpd[i], adapt)); 
		}
		float factor = 0;
		for (i = 0; i < MAX_PYR_LEVELS - 2; i++) {
			factor += contrast[i] * F_freq[i] * F_mask[i] / sum_contrast;
		}
		if (factor < 1) factor = 1;
		if (factor > 10) factor = 10;
		float delta = fabsf(la->Get_Value(x,y,0) - lb->Get_Value(x,y,0));
		if (delta > factor * tvi(adapt)) {
			pixels_failed++;
		}
	  }
	}
	
	if (aX) delete[] aX;
	if (aY) delete[] aY;
	if (aZ) delete[] aZ;
	if (bX) delete[] bX;
	if (bY) delete[] bY;
	if (bZ) delete[] bZ;
	if (aLum) delete[] aLum;
	if (bLum) delete[] bLum;
	if (la) delete la;
	if (lb) delete lb;
	
	if (pixels_failed < args.ThresholdPixels) {
		args.ErrorStr = "Images are perceptually indistinguishable\n";
		return true;
	}
	
	char different[100];
	sprintf(different, "%d pixels are different\n", pixels_failed);

	args.ErrorStr = "Images are visibly different\n";
	args.ErrorStr += different;
	return false;
}