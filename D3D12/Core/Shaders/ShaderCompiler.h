#pragma

#include <d3d12shader.h>
#include <dxcapi.h>

class ShaderCompiler
{
public:
    ShaderCompiler();
    ~ShaderCompiler();

private:
    IDxcUtils* _library;
    IDxcCompiler3* _compiler;
    IDxcContainerReflection* _containerReflection;
};