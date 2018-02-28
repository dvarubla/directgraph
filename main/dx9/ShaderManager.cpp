#include <main/resource.h>
#include <resources/common_res.h>
#include <main/WException.h>
#include <sstream>
#include "ShaderManager.h"
#include "Exception.h"

namespace directgraph {
    namespace dx9 {
        ShaderManager::ShaderManager(IFeatures *features, IDirect3DDevice9 *device):
                _device(device), _supportsEllipse(false),
                _centerBarV11Shader(NULL), _ellipseP14Shader(NULL), _centerBarV11Decl(NULL) {
            try {
                IFeatures::ShaderVersion vertexVer = features->getVertexShaderVer();
                IFeatures::ShaderVersion pixelVer = features->getPixelShaderVer();
                IFeatures::ShaderVersion ellipseVertexVer = {1, 1};
                IFeatures::ShaderVersion ellipsePixelVer = {1, 4};
                if (ellipseVertexVer <= vertexVer && ellipsePixelVer <= pixelVer) {
                    _supportsEllipse = true;
                    createVertexShader(CENTER_BAR_V1_1, IDR_VERTEX_SHADER, _centerBarV11Shader);
                    createPixelShader(ELLIPSE_P1_4, IDR_PIXEL_SHADER, _ellipseP14Shader);
                    D3DVERTEXELEMENT9 decl[] = {
                            {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                            {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                            D3DDECL_END()
                    };
                    if (_device->CreateVertexDeclaration(decl, &_centerBarV11Decl) != D3D_OK) {
                        THROW_EXC_CODE(
                                Exception,
                                DX9_CANT_CREATE_VDECL,
                                L"Can't create ellipse vertex declaration"
                        );
                    }
                }
            } catch(const WException &){
                tryDeleteRes();
                throw;
            }
        }

        bool ShaderManager::supportsEllipse() {
            return _supportsEllipse;
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
            try {
                load_resource(name, type, size, buffer);
            } catch(const WException &ex){
                THROW_EXC_CODE(
                        Exception,
                        DX9_CANT_CREATE_SHADER,
                        L"Can't load vertex shader name: " + to_wstring(name) + L" type: " + to_wstring(type)
                        + L" (" + ex.getString() + L")"
                );
            }
            if(_device->CreateVertexShader(static_cast<DWORD*>(buffer), &shader) != D3D_OK){
                THROW_EXC_CODE(
                        Exception,
                        DX9_CANT_CREATE_SHADER,
                        L"Can't create vertex shader name: " + to_wstring(name) + L" type: " + to_wstring(type)
                );
            }
        }

        void ShaderManager::createPixelShader(
                uint_fast32_t name, uint_fast32_t type,
                IDirect3DPixelShader9 *&shader
        ) {
            uint_fast32_t size;
            void *buffer;
            try {
                load_resource(name, type, size, buffer);
            } catch(const WException &ex){
                THROW_EXC_CODE(
                        Exception,
                        DX9_CANT_CREATE_SHADER,
                        L"Can't load pixel shader name: " + to_wstring(name) + L" type: " + to_wstring(type)
                        + L" (" + ex.getString() + L")"
                );
            }
            if(_device->CreatePixelShader(static_cast<DWORD*>(buffer), &shader) != D3D_OK){
                THROW_EXC_CODE(
                        Exception,
                        DX9_CANT_CREATE_SHADER,
                        L"Can't create pixel shader name: " + to_wstring(name) + L" type: " + to_wstring(type)
                );
            }
        }

        ShaderManager::~ShaderManager() {
            removeShaders();
            tryDeleteRes();
        }

        void ShaderManager::tryDeleteRes() {
            if(_centerBarV11Decl != NULL) {
                _centerBarV11Decl->Release();
            }
            if(_centerBarV11Shader != NULL) {
                _centerBarV11Shader->Release();
            }
            if(_ellipseP14Shader != NULL) {
                _ellipseP14Shader->Release();
            }
        }
    }
}
