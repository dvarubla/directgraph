#include <main/resource.h>
#include <resources/common_res.h>
#include "ShaderManager.h"

namespace directgraph {
    namespace dx9 {
        ShaderManager::ShaderManager(IFeatures *features, IDirect3DDevice9 *device): _device(device) {
            createVertexShader(CENTER_BAR_V1_1, IDR_VERTEX_SHADER, _centerBarV11Shader);
            createPixelShader(ELLIPSE_P1_4, IDR_PIXEL_SHADER, _ellipseP14Shader);
            D3DVERTEXELEMENT9 decl[] = {
                    { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                    { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
                    D3DDECL_END()
            };
            _device->CreateVertexDeclaration(decl, &_centerBarV11Decl);
        }

        bool ShaderManager::supportsEllipse() {
            return true;
        }

        void ShaderManager::setEllipse() {
            _device->SetVertexDeclaration(_centerBarV11Decl);
            _device->SetVertexShader(_centerBarV11Shader);
            _device->SetPixelShader(_ellipseP14Shader);
        }

        void ShaderManager::removeShaders() {
            _device->SetVertexDeclaration(NULL);
            _device->SetVertexShader(NULL);
            _device->SetPixelShader(NULL);
        }

        void
        ShaderManager::createVertexShader(
                uint_fast32_t name, uint_fast32_t type,
                IDirect3DVertexShader9 *&shader
        ) {
            uint_fast32_t size;
            void *buffer;
            load_resource(name, type, size, buffer);
            _device->CreateVertexShader(static_cast<DWORD*>(buffer), &shader);
        }

        void ShaderManager::createPixelShader(
                uint_fast32_t name, uint_fast32_t type,
                IDirect3DPixelShader9 *&shader
        ) {
            uint_fast32_t size;
            void *buffer;
            load_resource(name, type, size, buffer);
            _device->CreatePixelShader(static_cast<DWORD*>(buffer), &shader);
        }

        ShaderManager::~ShaderManager() {
            removeShaders();
            _centerBarV11Decl->Release();
            _centerBarV11Shader->Release();
            _ellipseP14Shader->Release();
        }
    }
}
