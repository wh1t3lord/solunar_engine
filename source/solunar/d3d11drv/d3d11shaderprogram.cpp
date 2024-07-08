#include "d3d11drv_pch.h"
#include "d3d11drv/d3d11shaderprogram.h"

#define CREATE_INPUT_LAYOUT_FROM_REFLECT

namespace engine
{

extern DXGI_FORMAT getDxgiFormat(ImageFormat format);

enum ShaderType
{
	ShaderType_Vertex,
	ShaderType_Pixel
};

class D3D11Include : public ID3DInclude
{
public:
	STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes);
	STDMETHOD(Close)(THIS_ LPCVOID pData);
};

STDMETHODIMP D3D11Include::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
#if 0
	DataStreamPtr fileStream = g_contentManager->openStream(pFileName);

	fileStream->seek(FileSeek::End, 0);
	long length = fileStream->tell();
	fileStream->seek(FileSeek::Begin, 0);
#endif


	return E_NOTIMPL;
}

STDMETHODIMP D3D11Include::Close(LPCVOID pData)
{
	return E_NOTIMPL;
}

ID3DBlob* createShaderFromText(const char* text, ShaderType shaderType, const char* defines = nullptr)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorTextBlob = nullptr;

	const char* entryPoint = (shaderType == ShaderType_Vertex) ? "VSMain" : "PSMain";
	const char* shaderTarget = (shaderType == ShaderType_Vertex) ? "vs_5_0" : "ps_5_0";

	std::vector<std::string> shderMacroStrings;
	if (defines && strlen(defines) > 0)
	{
		std::string definesStr = defines;
		size_t iterator = definesStr.find_first_of('\n');
		std::string exactDefineStr = definesStr.substr(0, iterator);
		shderMacroStrings.push_back(exactDefineStr);

		/*std::string definesStr = defines;

		size_t iterator = definesStr.find_first_of('\n');
		while (iterator != std::string::npos) {
			std::string exactDefineStr = definesStr.substr(0, iterator);
			Core::msg("createShaderFromText: %s", exactDefineStr.c_str());
			shderMacroStrings.push_back(exactDefineStr);
			iterator = definesStr.find_first_of('\n', iterator);
		}*/
	}

	std::vector<D3D_SHADER_MACRO> shaderMacro;
	for (auto& it : shderMacroStrings)
		shaderMacro.push_back(D3D_SHADER_MACRO{ it.c_str(), NULL });

	shaderMacro.push_back(D3D_SHADER_MACRO{ NULL, NULL });

	HRESULT hr = D3DCompile(text, strlen(text), "UNKNOWED", defines ? shaderMacro.data() : NULL,
		NULL, entryPoint, shaderTarget, D3DCOMPILE_DEBUG, 0, &shaderBlob, &errorTextBlob);
	if (FAILED(hr))
	{
		std::string errorText = "Failed to compile shader!";

		errorText += " ";
		errorText += shaderTarget;
		errorText += " ";

		if (errorTextBlob)
		{
			errorText += " ";
			errorText += "Reason: ";
			errorText += (const char*)errorTextBlob->GetBufferPointer();
			errorText += " ";

			errorTextBlob->Release();
			errorTextBlob = nullptr;
		}
		else
		{
			errorText += " ";
			errorText += "Reason: Unknown";
			errorText += " ";
		}

		Core::msg("createShaderFromText: %s", errorText.c_str());

		//std::terminate();
		//DebugBreak();
	}

	if (errorTextBlob)
	{
		errorTextBlob->Release();
		errorTextBlob = nullptr;
	}

	return shaderBlob;
}

D3D11ShaderProgram::D3D11ShaderProgram(D3D11Device* device, 
	const char* vstext, 
	const char* pstext,
	const char* defines /*= nullptr*/,
	InputLayoutDesc* inputLayout /*= nullptr*/, 
	int inputLayoutCount /*= 0*/) :
	m_inputLayout(nullptr),
	m_vertexShader(nullptr),
	m_pixelShader(nullptr)
{
	create(device, vstext, pstext, defines, inputLayout, inputLayoutCount);
}

D3D11ShaderProgram::~D3D11ShaderProgram()
{
	destroy();
}

void D3D11ShaderProgram::create(D3D11Device* device, const char* vstext, const char* pstext, const char* defines /*= nullptr*/, InputLayoutDesc* inputLayout /*= nullptr*/, int inputLayoutCount /*= 0*/)
{
	ID3DBlob* vertexShaderBlob = createShaderFromText(vstext, ShaderType_Vertex);
	device->getDevice()->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &m_vertexShader);

	if (inputLayout && inputLayoutCount > 0)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

		for (int i = 0; i < inputLayoutCount; i++)
		{
			D3D11_INPUT_ELEMENT_DESC inputElementDesc	= {};
			inputElementDesc.SemanticName				= inputLayout[i].m_semanticName;
			inputElementDesc.SemanticIndex				= inputLayout[i].m_semanticIndex;
			inputElementDesc.Format						= getDxgiFormat(inputLayout[i].m_format);
			inputElementDesc.InputSlot					= inputLayout[i].m_inputSlot;
			inputElementDesc.AlignedByteOffset			= inputLayout[i].m_alignedByteOffset;
			inputElementDesc.InstanceDataStepRate		= inputLayout[i].m_instanceDataStepRate;
			inputElementDesc.InputSlotClass				= D3D11_INPUT_PER_VERTEX_DATA;
			inputLayoutDesc.push_back(inputElementDesc);
		}

		device->getDevice()->CreateInputLayout(&inputLayoutDesc[0],
			(UINT)inputLayoutDesc.size(),
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			&m_inputLayout);
	}
#ifdef CREATE_INPUT_LAYOUT_FROM_REFLECT
	else
	{
		createInputLayout(device, vertexShaderBlob);
	}
#else
	Assert2(inputLayout, "Failed to create shader input layout");
#endif // CREATE_INPUT_LAYOUT_FROM_REFLECT

	vertexShaderBlob->Release();
	vertexShaderBlob = nullptr;

	ID3DBlob* pixelShaderBlob = createShaderFromText(pstext, ShaderType_Pixel, defines);
	device->getDevice()->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &m_pixelShader);

	pixelShaderBlob->Release();
	pixelShaderBlob = nullptr;
}

void D3D11ShaderProgram::destroy()
{
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
}

void D3D11ShaderProgram::createInputLayout(D3D11Device* device, ID3DBlob* vertexShaderBlob)
{
	// #TODO: convert engine inputLayouts to D3D11 input layout

	// Reflect shader info
	ID3D11ShaderReflection* shaderReflection = nullptr;
	D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)&shaderReflection);

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	uint32_t byteOffset = 0;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		shaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = 0;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = byteOffset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			byteOffset += 4;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			byteOffset += 8;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	device->getDevice()->CreateInputLayout(&inputLayoutDesc[0], 
		(UINT)inputLayoutDesc.size(), 
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), 
		&m_inputLayout);

	shaderReflection->Release();
}

}