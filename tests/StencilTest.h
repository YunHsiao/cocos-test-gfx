#pragma once

#include "TestBase.h"

namespace cc {

class StencilTest: public TestBaseI
{
public:
    DEFINE_CREATE_METHOD(StencilTest)
    StencilTest(const WindowInfo& info) : TestBaseI(info) {};
    ~StencilTest() = default;

public:
     virtual void tick() override;
     virtual bool initialize() override;
     virtual void destroy() override;

private:
    void createShader();
    void createBuffers();
    void createTextures();
    void createInputAssembler();
    void createPipelineState();
    
    const static uint BINDING_COUNT = 2;
    const static uint PIPELIE_COUNT = 6;
    gfx::Shader* _shader = nullptr;
    gfx::Texture* _labelTexture = nullptr;
    gfx::Texture* _uvCheckerTexture = nullptr;
    gfx::Buffer* _vertexBuffer = nullptr;
    gfx::InputAssembler* _inputAssembler = nullptr;
    gfx::Buffer* _uniformBuffer[BINDING_COUNT] = { nullptr };
    gfx::DescriptorSet* _descriptorSet[BINDING_COUNT] = { nullptr };
    gfx::DescriptorSetLayout* _descriptorSetLayout = nullptr;
    gfx::PipelineLayout* _pipelineLayout = nullptr;
    gfx::PipelineState* _pipelineState[PIPELIE_COUNT] = { nullptr };
    gfx::Sampler* _sampler = nullptr;

    struct MatrixUBO {
        Mat4 world;
        Mat4 viewProj;
    };
    MatrixUBO _uboData[BINDING_COUNT];
    
    float _time = 0.0f;
};

} // namespace cc
