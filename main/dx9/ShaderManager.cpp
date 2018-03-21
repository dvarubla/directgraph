#include <main/resource.h>
#include <resources/common_res.h>
#include <main/WException.h>
#include <sstream>
#include "ShaderManager.h"
#include "Exception.h"

namespace directgraph {
    namespace dx9 {
        ShaderManager::ShaderManager(IFeatures *features, IDirect3DDevice9 *device):
                _device(device),
                _centerBarV11Shader(NULL), _ellipseP14Shader(NULL), _centerBarV11Decl(NULL),
                _texturedBarV11Shader(NULL), _texturedBarP14Shader(NULL), _texturedBarV11Decl(NULL),
                _texturedCenterBarV11Shader(NULL), _texturedEllipseP14Shader(NULL), _texturedCenterBarV11Decl(NULL),
                _supportsEllipse(false), _supportsTexturedBar(false), _supportsTexturedEllipse(false)
        {
            try {
                IFeatures::ShaderVersion vertexVer = features->getVertexShaderVer();
                IFeatures::ShaderVersion pixelVer = features->getPixelShaderVer();
                createEllipseShaders(vertexVer, pixelVer);
                createTexturedBarShaders(vertexVer, pixelVer);
                createTexturedEllipseShaders(vertexVer, pixelVer);
            } catch(const WException &){
                tryDeleteRes();
                throw;
            }
        }

        void ShaderManager::createEllipseShaders(
                const IFeatures::ShaderVersion &vertexVer,
                const IFeatures::ShaderVersion &pixelVer
        ) {
            IFeatures::ShaderVersion ellipseVertexVer = {1, 1};
            IFeatures::ShaderVersion ellipsePixelVer = {1, 4};
            if (ellipseVertexVer <= vertexVer && ellipsePixelVer <= pixelVer) {
                _supportsEllipse = true;
                createVertexShader(CENTER_BAR_V1_1, IDR_VERTEX_SHADER, _centerBarV11Shader);
                createPixelShader(ELLIPSE_P1_4, IDR_PIXEL_SHADER, _ellipseP14Shader);
                D3DVERTEXELEMENT9 decl[] = {
                        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
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
        }

        void ShaderManager::createTexturedBarShaders(const IFeatures::ShaderVersion &vertexVer,
                                                     const IFeatures::ShaderVersion &pixelVer) {
            IFeatures::ShaderVersion texturedBarVertexVer = {1, 1};
            IFeatures::ShaderVersion texturedBarPixelVer = {1, 4};
            if (texturedBarVertexVer <= vertexVer && texturedBarPixelVer <= pixelVer) {
                _supportsTexturedBar = true;
                createVertexShader(TEXTURED_BAR_V1_1, IDR_VERTEX_SHADER, _texturedBarV11Shader);
                createPixelShader(TEXTURED_BAR_P1_4, IDR_PIXEL_SHADER, _texturedBarP14Shader);
                D3DVERTEXELEMENT9 decl[] = {
                        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                        {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
                        {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                        D3DDECL_END()
                };
                if (_device->CreateVertexDeclaration(decl, &_texturedBarV11Decl) != D3D_OK) {
                    THROW_EXC_CODE(
                            Exception,
                            DX9_CANT_CREATE_VDECL,
                            L"Can't create textured bar vertex declaration"
                    );
                }
            }
        }

        void ShaderManager::createTexturedEllipseShaders(const IFeatures::ShaderVersion &vertexVer,
                                                         const IFeatures::ShaderVersion &pixelVer) {
            IFeatures::ShaderVersion texturedBarVertexVer = {1, 1};
            IFeatures::ShaderVersion texturedBarPixelVer = {1, 4};
            if (texturedBarVertexVer <= vertexVer && texturedBarPixelVer <= pixelVer) {
                _supportsTexturedEllipse = true;
                createVertexShader(TEXTURED_CENTER_BAR_V1_1, IDR_VERTEX_SHADER, _texturedCenterBarV11Shader);
                createPixelShader(TEXTURED_ELLIPSE_P1_4, IDR_PIXEL_SHADER, _texturedEllipseP14Shader);
                D3DVERTEXELEMENT9 decl[] = {
                        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                        {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
                        {0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                        D3DDECL_END()
                };
                if (_device->CreateVertexDeclaration(decl, &_texturedCenterBarV11Decl) != D3D_OK) {
                    THROW_EXC_CODE(
                            Exception,
                            DX9_CANT_CREATE_VDECL,
                            L"Can't create textured ellipse vertex declaration"
                    );
                }
            }
        }

        bool ShaderManager::supportsEllipse() {
            return _supportsEllipse;
        }

        bool ShaderManager::supportsTexturedBar() {
            return _supportsTexturedBar;
        }

        bool ShaderManager::supportsTexturedEllipse() {
            return _supportsTexturedEllipse;
        }

        void ShaderManager::setTexturedBar() {
            _device->SetVertexDeclaration(_texturedBarV11Decl);
            _device->SetVertexShader(_texturedBarV11Shader);
            _device->SetPixelShader(_texturedBarP14Shader);
        }

        void ShaderManager::setEllipse() {
            _device->SetVertexDeclaration(_centerBarV11Decl);
            _device->SetVertexShader(_centerBarV11Shader);
            _device->SetPixelShader(_ellipseP14Shader);
        }

        void ShaderManager::setTexturedEllipse() {
            _device->SetVertexDeclaration(_texturedCenterBarV11Decl);
            _device->SetVertexShader(_texturedCenterBarV11Shader);
            _device->SetPixelShader(_texturedEllipseP14Shader);
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
            IUnknown *delArr[] = {
                    _centerBarV11Decl,
                    _centerBarV11Shader,
                    _ellipseP14Shader,

                    _texturedBarV11Decl,
                    _texturedBarV11Shader,
                    _texturedBarP14Shader,

                    _texturedCenterBarV11Decl,
                    _texturedCenterBarV11Shader,
                    _texturedEllipseP14Shader
            };
            for(uint_fast32_t i = 0; i < sizeof(delArr) / sizeof(IUnknown*); i++){
                if(delArr[i] != NULL){
                    delArr[i]->Release();
                }
            }
        }
    }
}
