#ifndef _SHADER_VALID_CONNRCTION_H_
#define _SHADER_VALID_CONNRCTION_H_

#include "../common/prerequest_std.h"
#include "../common/prerequest_maya.h"
#include "liqtypes.h"

namespace liquidmaya
{
	class LIQUID_EXPORT ShaderConnectionMap
	{
	public:
		ShaderConnectionMap();
		~ShaderConnectionMap();

		void begin(const MString &shadertype);
		void append(const MString &plug);
		void end();
		bool hasShaderType(const char* shadertype)const;
		void getValidConnection(const char* nodename, MStringArray& connections) const;

	protected:
		std::map<const std::string, MStringArray> validConnectionMap;
		MStringArray validConnection;
		MString currentShaderType;
	};
	//
class LIQUID_EXPORT ShaderValidConnection
{
public:
	ShaderValidConnection();
	~ShaderValidConnection();


	// @nodename	e.g. "lambert1","blinn1",...
	void getValidConnection(const char* nodename,  MStringArray& connections)const;
	bool hasShaderType(const char* shadertype)const;

protected:
	void setValidConnection();
	void setValidConnection_mi();

	ShaderConnectionMap validConnectionMap;


private:
	ShaderValidConnection(const ShaderValidConnection&);
	ShaderValidConnection& operator=(const ShaderValidConnection&);
};
}//namespace liquidmaya


#endif//_SHADER_VALID_CONNRCTION_H_