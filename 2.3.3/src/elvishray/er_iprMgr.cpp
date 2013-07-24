#include "er_iprMgr.h"

#include <common/mayacheck.h>
#include <liqGlobalHelpers.h>
#include "er_log.h"

#include "ercall.h"
#include "er_renderer.h"
#include "er_helper.h"

namespace elvishray
{
	IprMgr::IprMgr()
	{
		CM_TRACE_FUNC("IprMgr::IprMgr()");

	}
	//
	IprMgr::~IprMgr()
	{
		CM_TRACE_FUNC("IprMgr::~IprMgr()");
	}
	//
	void IprMgr::onAttributeChanged(MNodeMessage::AttributeMessage msg, 
		MPlug &plug, MPlug &otherPlug, void *userData)
	{
		CM_TRACE_FUNC("IprMgr::onAttributeChanged("<<msg<<","<<plug.name().asChar()<<","<<otherPlug.name().asChar()<<",userData)");

		liquidMessage2(messageInfo, "[%s], [%s], ", plug.name().asChar(), plug.partialName(true, true, true, true, true, true) );

		MStringArray strarray;
		IfMErrorWarn( plug.name().split('.', strarray) );
		MString nodeName(strarray[0]);
		MString plugName(strarray[1]);


		MString nodeType( getNodeType(nodeName) );
		//if( nodeType == "camera" )
		{
			updateCamera(plug);
		} 
		//else /* if( nodtype is shader) */
		{
			updateShader(plug);
		}

		//todo....

	}
	void IprMgr::updateCamera(const MPlug &plug)
	{
		CM_TRACE_FUNC("IprMgr::updateCamera("<<plug.name().asChar()<<")");
		MStatus status;
		OutputMgr &o = Renderer::getOutputMgr();

		MStringArray strarray;
		IfMErrorWarn( plug.name().split('.', strarray) );
		MString nodeName(strarray[0]);
		MString plugName(strarray[1]);

		MString cameraTransformNode;
		MStringArray cameraFullPaths;
		int width=0, height=0;
		{
			IfMErrorWarn(MGlobal::executeCommand("string $cam = `getAttr liquidGlobals.renderCamera`; ls -long $cam;", cameraFullPaths));
			IfMErrorWarn(MGlobal::executeCommand("firstParentOf(\""+cameraFullPaths[0]+"\")", cameraTransformNode));
			IfMErrorWarn(MGlobal::executeCommand("getAttr liquidGlobals.xResolution",width));
			IfMErrorWarn(MGlobal::executeCommand("getAttr liquidGlobals.yResolution",height));
		}
		std::string sCameraObjectName(cameraFullPaths[0].asChar());

		//camera transform matrix
		MDagPath cameraDagPath;
		getDagPathByName(cameraDagPath, cameraFullPaths[0].asChar());
		MFnCamera fnCamera(cameraDagPath, &status);
		IfMErrorWarn(status);

		MObject transformNode = cameraDagPath.transform(&status );
		IfMErrorWarn(status);
		MFnDagNode transform (transformNode, &status);
		IfMErrorWarn(status);
		MTransformationMatrix   m0(transform.transformationMatrix());
		liqMatrix m;		
		IfMErrorWarn(m0.asMatrix().get(m));

		// render region
		unsigned int left=0, right=0, bottom=0, top=0;
		IfMErrorWarn(MRenderView::getRenderRegion(left, right, bottom, top));


		o.ei_camera( sCameraObjectName.c_str() );
//we don't define 'output', so the image file will not be generated.
// 		o.ei_output("R:/MyDocuments/maya/projects/default/rmanpix/er_ipr.bmp","bmp", EI_IMG_DATA_RGB);
// 		o.ei_output_variable("color", EI_TYPE_VECTOR);
// 		o.ei_end_output();
		o.ei_window(left, right+1, height-top, height-bottom+1);
		o.ei_end_camera();
		o.ln();

		o.ei_instance( cameraTransformNode.asChar() );
		o.ei_element(	sCameraObjectName.c_str() );
		o.ei_transform( m[0][0], m[0][1], m[0][2], m[0][3],   m[1][0], m[1][1], m[1][2], m[1][3],   m[2][0], m[2][1], m[2][2], m[2][3],   m[3][0], m[3][1], m[3][2], m[3][3] );
		//_S( ei_transform(  1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  -1.95384,-2.76373,16.1852, 1.0f ) );
		o.ei_end_instance();
		o.ln();
	}
	void IprMgr::updateShader(const MPlug &plug)
	{
		CM_TRACE_FUNC("IprMgr::updateShader("<<plug.name().asChar()<<")");
		MStatus status;
		OutputMgr &o = Renderer::getOutputMgr();

		MStringArray strarray;
		IfMErrorWarn( plug.name().split('.', strarray) );
		MString nodeName(strarray[0]);
		MString plugName(strarray[1]);

		// shader parameters
		MPlugArray connectedElements;
		plug.connectedTo( connectedElements, true, false, &status );
		IfMErrorWarn(status);
		if (connectedElements.length() == 0)
		{
			o.ei_shader("", nodeName.asChar());
			if ( plug.isCompound() )
			{
				// It's a constant color... set it now and return.
				unsigned int numChildren = plug.numChildren();
				switch(numChildren)
				{
				case 3:
					{
						float r, g, b;
						plug.child(0).getValue(r);
						plug.child(1).getValue(g);
						plug.child(2).getValue(b);
						o.ei_shader_param_vector(ER_SPN(plugName).asChar(),r,g,b);
						liquidMessage2(messageInfo, "%f, %f, %f", r,g,b );
					}break;
				case 4:
					{
						float r, g, b, a;
						plug.child(0).getValue(r);
						plug.child(1).getValue(g);
						plug.child(2).getValue(b);
						plug.child(3).getValue(a);
						o.ei_shader_param_vector4(ER_SPN(plugName).asChar(),r,g,b,a);
						liquidMessage2(messageInfo, "%f, %f, %f, %f", r,g,b,a );
					}break;
				}
			} else {
				float v;
				plug.child(0).getValue(v);
				o.ei_shader_param_scalar(ER_SPN(plugName).asChar(), v);
				liquidMessage2(messageInfo, "%f", v );
			}
			o.ei_end_shader();
			o.ln();
		} else {// the plug is connected.

		}
	}


}//namespace elvishray