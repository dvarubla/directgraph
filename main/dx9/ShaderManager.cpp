#include <main/resource.h>
#include <main/WException.h>
#include <sstream>
#include "ShaderManager.h"
#include "Exception.h"

namespace directgraph {
    namespace dx9 {
        ShaderManager::ShaderManager(IFeatures *features, IDirect3DDevice9 *device):
                _device(device)
        {
            try {
                IFeatures::ShaderVersion vertexVer = features->getVertexShaderVer();
                IFeatures::ShaderVersion pixelVer = features->getPixelShaderVer();
                for(uint_fast16_t i = 0; i < _shaders.size(); i++){
                    _shaders[i].vertexShaderId = FIRST_VERTEX_SHADER + i;
                    _shaders[i].pixelShaderId = FIRST_PIXEL_SHADER + i;
                    _shaders[i].vertexShader = NULL;
                    _shaders[i].pixelShader = NULL;
                    _shaders[i].vertexDecl = NULL;
                    _shaders[i].haveSupport = false;
                }
                _haveShaderSupport = false;
                for(uint_fast16_t i = 0; i < _shaders.size(); i++){
                    createShader(static_cast<PixelShaderId>(i), vertexVer, pixelVer);
                    if(_shaders[i].haveSupport){
                        _haveShaderSupport = true;
                    }
                }
            } catch(const WException &){
                tryDeleteRes();
                throw;
            }
        }

        void ShaderManager::createShader(PixelShaderId id,
                                         const IFeatures::ShaderVersion &vertexVer,
                                         const IFeatures::ShaderVersion &pixelVer) {
            switch (id){
                case ELLIPSE_P1_4_SHADER:
                case TEXTURED_BAR_P1_4_SHADER:
                case TEXTURED_ELLIPSE_P1_4_SHADER:
                case RECTANGLE_P1_4_SHADER:
                case TEXTURED_LINE_P1_4_SHADER: {
                    IFeatures::ShaderVersion tVertexVer = {1, 1};
                    IFeatures::ShaderVersion tPixelVer = {1, 4};
                    _shaders[id].vertexShaderVer = tVertexVer;
                    _shaders[id].pixelShaderVer = tPixelVer;
                    break;
                }
                case TEXTURED_RECTANGLE_P2_0_SHADER: {
                    IFeatures::ShaderVersion tVertexVer = {1, 1};
                    IFeatures::ShaderVersion tPixelVer = {2, 0};
                    _shaders[id].vertexShaderVer = tVertexVer;
                    _shaders[id].pixelShaderVer = tPixelVer;
                    break;
                }
            }
            if (_shaders[id].vertexShaderVer <= vertexVer && _shaders[id].pixelShaderVer <= pixelVer) {
                _shaders[id].haveSupport = true;
                createVertexShader(_shaders[id].vertexShaderId, IDR_VERTEX_SHADER, _shaders[id].vertexShader);
                createPixelShader(_shaders[id].pixelShaderId, IDR_PIXEL_SHADER, _shaders[id].pixelShader);
                switch (id) {
                    case ELLIPSE_P1_4_SHADER: {
                        D3DVERTEXELEMENT9 decl[] = {
                                {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                                D3DDECL_END()
                        };
                        if (_device->CreateVertexDeclaration(decl, &_shaders[id].vertexDecl) != D3D_OK) {
                            THROW_EXC_CODE(
                                    Exception,
                                    DX9_CANT_CREATE_VDECL,
                                    L"Can't create ellipse vertex declaration"
                            );
                        }
                        break;
                    }
                    case TEXTURED_BAR_P1_4_SHADER: {
                        D3DVERTEXELEMENT9 decl[] = {
                                {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
                                {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                                D3DDECL_END()
                        };
                        if (_device->CreateVertexDeclaration(decl, &_shaders[id].vertexDecl) != D3D_OK) {
                            THROW_EXC_CODE(
                                    Exception,
                                    DX9_CANT_CREATE_VDECL,
                                    L"Can't create textured bar vertex declaration"
                            );
                        }
                        break;
                    }
                    case TEXTURED_ELLIPSE_P1_4_SHADER: {
                        D3DVERTEXELEMENT9 decl[] = {
                                {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
                                {0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                                D3DDECL_END()
                        };
                        if (_device->CreateVertexDeclaration(decl, &_shaders[id].vertexDecl) != D3D_OK) {
                            THROW_EXC_CODE(
                                    Exception,
                                    DX9_CANT_CREATE_VDECL,
                                    L"Can't create textured ellipse vertex declaration"
                            );
                        }
                        break;
                    }
                    case TEXTURED_RECTANGLE_P2_0_SHADER: {
                        D3DVERTEXELEMENT9 decl[] = {
                                {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                                D3DDECL_END()
                        };
                        if (_device->CreateVertexDeclaration(decl, &_shaders[id].vertexDecl) != D3D_OK) {
                            THROW_EXC_CODE(
                                    Exception,
                                    DX9_CANT_CREATE_VDECL,
                                    L"Can't create textured rectangle vertex declaration"
                            );
                        }
                        break;
                    }
                    case RECTANGLE_P1_4_SHADER: {
                        D3DVERTEXELEMENT9 decl[] = {
                                {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
                                D3DDECL_END()
                        };
                        if (_device->CreateVertexDeclaration(decl, &_shaders[id].vertexDecl) != D3D_OK) {
                            THROW_EXC_CODE(
                                    Exception,
                                    DX9_CANT_CREATE_VDECL,
                                    L"Can't create rectangle vertex declaration"
                            );
                        }
                        break;
                    }

                    case TEXTURED_LINE_P1_4_SHADER: {
                        D3DVERTEXELEMENT9 decl[] = {
                                {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
                                {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
                                D3DDECL_END()
                        };
                        if (_device->CreateVertexDeclaration(decl, &_shaders[id].vertexDecl) != D3D_OK) {
                            THROW_EXC_CODE(
                                    Exception,
                                    DX9_CANT_CREATE_VDECL,
                                    L"Can't create rectangle vertex declaration"
                            );
                        }
                        break;
                    }
                }
            }
        }

        bool ShaderManager::supportsEllipse() {
            return checkSupport(ELLIPSE_P1_4_SHADER);
        }

        bool ShaderManager::supportsTexturedBar() {
            return checkSupport(TEXTURED_BAR_P1_4_SHADER);
        }

        bool ShaderManager::supportsTexturedEllipse() {
            return checkSupport(TEXTURED_ELLIPSE_P1_4_SHADER);
        }

        bool ShaderManager::supportsTexturedRectangle() {
            return checkSupport(TEXTURED_RECTANGLE_P2_0_SHADER);
        }

        bool ShaderManager::supportsRectangle() {
            return checkSupport(RECTANGLE_P1_4_SHADER);
        }

        bool ShaderManager::supportsTexturedLine() {
            return checkSupport(TEXTURED_LINE_P1_4_SHADER);
        }

        bool ShaderManager::checkSupport(uint_fast16_t id) {
            return _shaders[id].haveSupport;
        }

        bool ShaderManager::supportsShaders() {
            return _haveShaderSupport;
        }

        void ShaderManager::setShader(ShaderManager::PixelShaderId id) {
            _device->SetVertexDeclaration(_shaders[id].vertexDecl);
            _device->SetVertexShader(_shaders[id].vertexShader);
            _device->SetPixelShader(_shaders[id].pixelShader);
        }

        void ShaderManager::setTexturedBar() {
            setShader(TEXTURED_BAR_P1_4_SHADER);
        }

        void ShaderManager::setEllipse() {
            setShader(ELLIPSE_P1_4_SHADER);
        }

        void ShaderManager::setTexturedEllipse() {
            setShader(TEXTURED_ELLIPSE_P1_4_SHADER);
        }

        void ShaderManager::setTexturedRectangle() {
            setShader(TEXTURED_RECTANGLE_P2_0_SHADER);
        }

        void ShaderManager::setRectangle() {
            setShader(RECTANGLE_P1_4_SHADER);
        }

        void ShaderManager::setTexturedLine() {
            setShader(TEXTURED_LINE_P1_4_SHADER);
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
            for(uint_fast32_t i = 0; i < _shaders.size(); i++){
                if(_shaders[i].vertexShader != NULL){
                    _shaders[i].vertexShader->Release();
                }
                if(_shaders[i].pixelShader != NULL){
                    _shaders[i].pixelShader->Release();
                }
                if(_shaders[i].vertexDecl != NULL){
                    _shaders[i].vertexDecl->Release();
                }
            }
        }
    }
}
