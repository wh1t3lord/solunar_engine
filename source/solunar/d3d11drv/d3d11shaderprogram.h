#ifndef GRAPHICS_D3D11_D3D11SHADERPROGRAM_H
#define GRAPHICS_D3D11_D3D11SHADERPROGRAM_H

#include "graphics/shaderprogram.h"

namespace solunar
{

class D3D11Device;

class D3D11ShaderProgram : public IShaderProgram
{
public:
	D3D11ShaderProgram(D3D11Device* device, const char* vstext, const char* pstext, const char* defines = nullptr, InputLayoutDesc* inputLayout = nullptr, int inputLayoutCount = 0);
	~D3D11ShaderProgram();

	void Create(D3D11Device* device, const char* vstext, const char* pstext, const char* defines = nullptr, InputLayoutDesc* inputLayout = nullptr, int inputLayoutCount = 0);
	void Destroy();

	void createInputLayout(D3D11Device* device, ID3DBlob* vertexShaderBlob);

	ID3D11InputLayout* getInputLayout() { return m_inputLayout; }
	ID3D11VertexShader* getVertexShader() { return m_vertexShader; }
	ID3D11PixelShader* getPixelShader() { return m_pixelShader; }

	void setDebugName(const char* vsname, const char* psname);

private:
	ID3D11InputLayout* m_inputLayout;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
};

}

#endif