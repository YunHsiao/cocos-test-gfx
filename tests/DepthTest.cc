#include "DepthTest.h"
#include "gfx-base/GFXDef-common.h"
#include "tiny_obj_loader.h"

namespace cc {

namespace {

constexpr bool USE_DEPTH_RESOLVE = false;

struct DepthResolveFramebuffer {
    explicit DepthResolveFramebuffer(gfx::Device *device, gfx::Swapchain *swapchain) {
        gfx::TextureInfo depthStecnilTexInfo;
        depthStecnilTexInfo.type       = gfx::TextureType::TEX2D;
        depthStecnilTexInfo.usage      = gfx::TextureUsageBit::DEPTH_STENCIL_ATTACHMENT | gfx::TextureUsageBit::SAMPLED;
        depthStecnilTexInfo.format     = gfx::Format::DEPTH;
        depthStecnilTexInfo.flags      = gfx::TextureFlagBit::GEN_MIPMAP;
        depthStecnilTexInfo.width      = swapchain->getWidth();
        depthStecnilTexInfo.height     = swapchain->getHeight();
        depthStecnilTexInfo.levelCount = TestBaseI::getMipmapLevelCounts(depthStecnilTexInfo.width, depthStecnilTexInfo.height);
        depthStencilTex                = device->createTexture(depthStecnilTexInfo);

        gfx::TextureViewInfo depthStencilTexViewInfo;
        depthStencilTexViewInfo.texture    = depthStencilTex;
        depthStencilTexViewInfo.format     = gfx::Format::DEPTH;
        depthStencilTexViewInfo.baseLevel  = lodLevel;
        depthStencilTexViewInfo.levelCount = 1;

        bunnyArea.width  = swapchain->getWidth() >> lodLevel;
        bunnyArea.height = swapchain->getHeight() >> lodLevel;

        depthStencilTexView = device->createTexture(depthStencilTexViewInfo);

        gfx::TextureInfo depthStecnilTexMSAAInfo;
        depthStecnilTexMSAAInfo.type    = gfx::TextureType::TEX2D;
        depthStecnilTexMSAAInfo.usage   = gfx::TextureUsageBit::DEPTH_STENCIL_ATTACHMENT;
        depthStecnilTexMSAAInfo.samples = gfx::SampleCount::MULTIPLE_BALANCE;
        depthStecnilTexMSAAInfo.format  = gfx::Format::DEPTH;
        depthStecnilTexMSAAInfo.width   = swapchain->getWidth();
        depthStecnilTexMSAAInfo.height  = swapchain->getHeight();
        depthStencilTexMSAA             = device->createTexture(depthStecnilTexMSAAInfo);

        if (USE_DEPTH_RESOLVE) {
            gfx::RenderPassInfo renderPassInfo;

            gfx::ColorAttachment &depthStencilAttachment{renderPassInfo.colorAttachments.emplace_back()};
            depthStencilAttachment.format      = gfx::Format::DEPTH;
            depthStencilAttachment.sampleCount = gfx::SampleCount::MULTIPLE_BALANCE;
            depthStencilAttachment.storeOp     = gfx::StoreOp::DISCARD;
            depthStencilAttachment.barrier     = device->getGeneralBarrier({
                gfx::AccessFlagBit::NONE,
                gfx::AccessFlagBit::DEPTH_STENCIL_ATTACHMENT_WRITE,
            });

            gfx::ColorAttachment &depthStencilResolveAttachment{renderPassInfo.colorAttachments.emplace_back()};
            depthStencilResolveAttachment.format = gfx::Format::DEPTH;
            depthStencilResolveAttachment.loadOp = gfx::LoadOp::DISCARD;
            depthStencilAttachment.barrier       = device->getGeneralBarrier({
                gfx::AccessFlagBit::NONE,
                gfx::AccessFlagBit::FRAGMENT_SHADER_READ_TEXTURE,
            });

            gfx::SubpassInfo &subpass{renderPassInfo.subpasses.emplace_back()};
            subpass.depthStencil        = 0U;
            subpass.depthStencilResolve = 1U;
            subpass.depthResolveMode    = gfx::ResolveMode::AVERAGE;
            subpass.stencilResolveMode  = gfx::ResolveMode::AVERAGE;

            renderPass = device->createRenderPass(renderPassInfo);

            gfx::FramebufferInfo fboInfo;
            fboInfo.renderPass = renderPass;
            fboInfo.colorTextures.push_back(depthStencilTexMSAA);
            fboInfo.colorTextures.push_back(depthStencilTex);
            framebuffer = device->createFramebuffer(fboInfo);
        } else {
            gfx::RenderPassInfo renderPassInfo;
            renderPassInfo.depthStencilAttachment.format  = gfx::Format::DEPTH;
            renderPassInfo.depthStencilAttachment.barrier = device->getGeneralBarrier({
                gfx::AccessFlagBit::NONE,
                gfx::AccessFlagBit::FRAGMENT_SHADER_READ_TEXTURE,
            });
            renderPass                                    = device->createRenderPass(renderPassInfo);

            gfx::FramebufferInfo fboInfo;
            fboInfo.renderPass          = renderPass;
            fboInfo.depthStencilTexture = depthStencilTexView;
            framebuffer                 = device->createFramebuffer(fboInfo);
        }
    }

    void resize(uint width, uint height) {
        if (depthStencilTexMSAA->getWidth() == width && depthStencilTexMSAA->getHeight() == height) return;

        framebuffer->destroy();

        bunnyArea.height = height >> lodLevel;
        bunnyArea.width  = width >> lodLevel;

        depthStencilTexMSAA->resize(width, height);
        depthStencilTex->resize(width, height);

        gfx::FramebufferInfo fboInfo;
        fboInfo.renderPass = renderPass;

        if (USE_DEPTH_RESOLVE) {
            fboInfo.colorTextures.push_back(depthStencilTexMSAA);
            fboInfo.colorTextures.push_back(depthStencilTex);
        } else {
            fboInfo.depthStencilTexture = depthStencilTexView;
        }

        framebuffer->initialize(fboInfo);
    }

    void destroy() {
        CC_SAFE_DESTROY_AND_DELETE(framebuffer);
        CC_SAFE_DESTROY_AND_DELETE(depthStencilTexView);
        CC_SAFE_DESTROY_AND_DELETE(depthStencilTex);
        CC_SAFE_DESTROY_AND_DELETE(depthStencilTexMSAA);
        CC_SAFE_DESTROY_AND_DELETE(renderPass);
    }

    const uint lodLevel  = 4;
    gfx::Rect  bunnyArea = gfx::Rect{0, 0, 0, 0};

    gfx::RenderPass * renderPass          = nullptr;
    gfx::Texture *    depthStencilTexMSAA = nullptr;
    gfx::Texture *    depthStencilTex     = nullptr;
    gfx::Texture *    depthStencilTexView = nullptr;
    gfx::Framebuffer *framebuffer         = nullptr;
};

struct BigTriangle {
    BigTriangle(gfx::Device *device, gfx::Framebuffer *fbo) : fbo(fbo), device(device) {
        createShader();
        createBuffers();
        createInputAssembler();
        createPipeline();
    }

    void createShader() {
        ShaderSources<StandardShaderSource> sources;
        sources.glsl4 = {
            R"(
                precision highp float;
                layout(location = 0) in vec2 a_position;
                layout(location = 1) in vec2 a_texCoord;

                layout(location = 0) out vec2 v_texCoord;

                void main() {
                    v_texCoord = a_texCoord;
                    gl_Position = vec4(a_position, 0, 1);
                }
            )",
            R"(
                precision highp float;
                layout(location = 0) in vec2 v_texCoord;
                layout(set = 0, binding = 0) uniform Near_Far_Uniform
                {
                    float u_near;
                    float u_far;
                };
                layout(set = 0, binding = 1) uniform sampler2D u_texture;
                layout(location = 0) out vec4 o_color;
                void main() {
                    float z = texture(u_texture, v_texCoord).x;
                    float viewZ = (u_near * u_far) / ((u_far - u_near) * z - u_far);
                    float depth = (viewZ + u_near) / (u_near - u_far);

                    o_color.rgb = vec3(depth);
                    o_color.a = 1.0;
                }
            )",
        };

        sources.glsl3 = {
            R"(
                in vec2 a_position;
                in vec2 a_texCoord;

                out vec2 v_texCoord;

                void main() {
                    v_texCoord = a_texCoord;
                    gl_Position = vec4(a_position, 0, 1);
                }
            )",
            R"(
                precision mediump float;
                in vec2 v_texCoord;
                uniform sampler2D u_texture;
                layout(std140) uniform Near_Far_Uniform {
                    float u_near;
                    float u_far;
                };
                out vec4 o_color;
                void main() {
                    float z = texture(u_texture, v_texCoord).x;
                    float viewZ = (u_near * u_far) / ((u_far - u_near) * z - u_far);
                    float depth = (viewZ + u_near) / (u_near - u_far);

                    o_color.rgb = vec3(depth);
                    o_color.a = 1.0;
                }
            )",
        };

        sources.glsl1 = {
            R"(
                attribute vec2 a_position;
                attribute vec2 a_texCoord;

                varying vec2 v_texCoord;

                void main() {
                    v_texCoord = a_texCoord;
                    gl_Position = vec4(a_position, 0, 1);
                }
            )",
            R"(
                precision mediump float;
                varying vec2 v_texCoord;
                uniform sampler2D u_texture;
                uniform float u_near;
                uniform float u_far;

                void main() {
                    float z = texture2D(u_texture, v_texCoord).x;
                    float viewZ = (u_near * u_far) / ((u_far - u_near) * z - u_far);
                    float depth = (viewZ + u_near) / (u_near - u_far);

                    gl_FragColor.rgb = vec3(depth);
                    gl_FragColor.a = 1.0;
                }
            )",
        };

        StandardShaderSource &source = TestBaseI::getAppropriateShaderSource(sources);

        gfx::ShaderStageList shaderStageList;
        gfx::ShaderStage     vertexShaderStage;
        vertexShaderStage.stage  = gfx::ShaderStageFlagBit::VERTEX;
        vertexShaderStage.source = source.vert;
        shaderStageList.emplace_back(std::move(vertexShaderStage));

        gfx::ShaderStage fragmentShaderStage;
        fragmentShaderStage.stage  = gfx::ShaderStageFlagBit::FRAGMENT;
        fragmentShaderStage.source = source.frag;
        shaderStageList.emplace_back(std::move(fragmentShaderStage));

        gfx::AttributeList attributeList = {
            {"a_position", gfx::Format::RG32F, false, 0, false, 0},
            {"a_texCoord", gfx::Format::RG32F, false, 0, false, 1},
        };
        gfx::UniformList               nearFarUniform   = {{"u_near", gfx::Type::FLOAT, 1}, {"u_far", gfx::Type::FLOAT, 1}};
        gfx::UniformBlockList          uniformBlockList = {{0, 0, "Near_Far_Uniform", nearFarUniform, 1}};
        gfx::UniformSamplerTextureList samplers         = {{0, 1, "u_texture", gfx::Type::SAMPLER2D, 1}};

        gfx::ShaderInfo shaderInfo;
        shaderInfo.name            = "BigTriangle";
        shaderInfo.stages          = std::move(shaderStageList);
        shaderInfo.attributes      = std::move(attributeList);
        shaderInfo.blocks          = std::move(uniformBlockList);
        shaderInfo.samplerTextures = std::move(samplers);
        shader                     = device->createShader(shaderInfo);
    }

    void createBuffers() {
        // create vertex buffer
        // UV space origin is at top-left
        float vertices[] = {-1, -4, 0.0, -1.5,
                            -1, 1, 0.0, 1.0,
                            4, 1, 2.5, 1.0};

        vertexBuffer = device->createBuffer({
            gfx::BufferUsage::VERTEX,
            gfx::MemoryUsage::DEVICE,
            sizeof(vertices),
            4 * sizeof(float),
        });
        vertexBuffer->update(vertices, sizeof(vertices));

        // create uniform buffer
        nearFarUniformBuffer = device->createBuffer({
            gfx::BufferUsage::UNIFORM,
            gfx::MemoryUsage::DEVICE,
            TestBaseI::getUBOSize(2 * sizeof(float)),
        });

        float uboData[] = {1.F, 100.0F};
        nearFarUniformBuffer->update(uboData, sizeof(uboData));
    }

    void createInputAssembler() {
        gfx::InputAssemblerInfo inputAssemblerInfo;
        inputAssemblerInfo.attributes.push_back({"a_position", gfx::Format::RG32F, false, 0, false});
        inputAssemblerInfo.attributes.push_back({"a_texCoord", gfx::Format::RG32F, false, 0, false});
        inputAssemblerInfo.vertexBuffers.emplace_back(vertexBuffer);
        inputAssembler = device->createInputAssembler(inputAssemblerInfo);
    }

    void createPipeline() {
        gfx::DescriptorSetLayoutInfo dslInfo;
        dslInfo.bindings.push_back({0, gfx::DescriptorType::UNIFORM_BUFFER, 1, gfx::ShaderStageFlagBit::FRAGMENT});
        dslInfo.bindings.push_back({1, gfx::DescriptorType::SAMPLER_TEXTURE, 1, gfx::ShaderStageFlagBit::FRAGMENT});
        descriptorSetLayout = device->createDescriptorSetLayout(dslInfo);

        pipelineLayout = device->createPipelineLayout({{descriptorSetLayout}});

        descriptorSet = device->createDescriptorSet({descriptorSetLayout});

        gfx::SamplerInfo samplerInfo;
        auto *           sampler = device->getSampler(samplerInfo);

        descriptorSet->bindBuffer(0, nearFarUniformBuffer);
        descriptorSet->bindSampler(1, sampler);
        // don't update just yet for the texture is still missing

        gfx::PipelineStateInfo pipelineInfo;
        pipelineInfo.primitive                    = gfx::PrimitiveMode::TRIANGLE_LIST;
        pipelineInfo.shader                       = shader;
        pipelineInfo.inputState.attributes        = inputAssembler->getAttributes();
        pipelineInfo.renderPass                   = fbo->getRenderPass();
        pipelineInfo.depthStencilState.depthTest  = false;
        pipelineInfo.depthStencilState.depthWrite = false;
        pipelineInfo.rasterizerState.cullMode     = gfx::CullMode::NONE;
        pipelineInfo.pipelineLayout               = pipelineLayout;

        pipelineState = device->createPipelineState(pipelineInfo);
    }

    void destroy() {
        CC_SAFE_DESTROY_AND_DELETE(shader);
        CC_SAFE_DESTROY_AND_DELETE(vertexBuffer);
        CC_SAFE_DESTROY_AND_DELETE(inputAssembler);
        CC_SAFE_DESTROY_AND_DELETE(descriptorSet);
        CC_SAFE_DESTROY_AND_DELETE(descriptorSetLayout);
        CC_SAFE_DESTROY_AND_DELETE(pipelineLayout);
        CC_SAFE_DESTROY_AND_DELETE(texture);
        CC_SAFE_DESTROY_AND_DELETE(pipelineState);
        CC_SAFE_DESTROY_AND_DELETE(nearFarUniformBuffer);
    }

    gfx::Shader *             shader               = nullptr;
    gfx::Framebuffer *        fbo                  = nullptr;
    gfx::Buffer *             vertexBuffer         = nullptr;
    gfx::Buffer *             nearFarUniformBuffer = nullptr;
    gfx::Device *             device               = nullptr;
    gfx::InputAssembler *     inputAssembler       = nullptr;
    gfx::DescriptorSet *      descriptorSet        = nullptr;
    gfx::DescriptorSetLayout *descriptorSetLayout  = nullptr;
    gfx::PipelineLayout *     pipelineLayout       = nullptr;
    gfx::Texture *            texture              = nullptr;
    gfx::PipelineState *      pipelineState        = nullptr;
};

struct Bunny {
    Bunny(gfx::Device *device, gfx::Framebuffer *fbo) : device(device) {
        createShader();
        createBuffers();
        createInputAssembler();
        createPipeline(fbo);
    }

    void createShader() {
        ShaderSources<StandardShaderSource> sources;
        sources.glsl4 = {
            R"(
                layout(location = 0) in vec3 a_position;
                layout(set = 0, binding = 0) uniform MVP_Matrix {
                    mat4 u_model;
                    mat4 u_view;
                    mat4 u_projection;
                };
                void main () {
                    vec4 pos = u_projection * u_view * u_model * vec4(a_position, 1);

                    gl_Position = pos;
                }
            )",
            R"(
                void main () {}
            )",
        };

        sources.glsl3 = {
            R"(
                in vec3 a_position;
                layout(std140) uniform MVP_Matrix {
                    mat4 u_model;
                    mat4 u_view;
                    mat4 u_projection;
                };
                void main () {
                    vec4 pos = u_projection * u_view * u_model * vec4(a_position, 1);

                    gl_Position = pos;
                }
            )",
            R"(
                precision mediump float;
                void main () {}
            )",
        };

        sources.glsl1 = {
            R"(
                attribute vec3 a_position;
                uniform mat4 u_model, u_view, u_projection;

                void main () {
                    vec4 pos = u_projection * u_view * u_model * vec4(a_position, 1);

                    gl_Position = pos;
                }
            )",
            R"(
                precision mediump float;
                void main () {}
            )",
        };

        StandardShaderSource &source = TestBaseI::getAppropriateShaderSource(sources);

        // vertex shader
        gfx::ShaderStageList shaderStageList;
        gfx::ShaderStage     vertexShaderStage;
        vertexShaderStage.stage  = gfx::ShaderStageFlagBit::VERTEX;
        vertexShaderStage.source = source.vert;
        shaderStageList.emplace_back(std::move(vertexShaderStage));

        // fragment shader
        gfx::ShaderStage fragmentShaderStage;
        fragmentShaderStage.stage  = gfx::ShaderStageFlagBit::FRAGMENT;
        fragmentShaderStage.source = source.frag;
        shaderStageList.emplace_back(std::move(fragmentShaderStage));

        gfx::AttributeList attributeList = {{"a_position", gfx::Format::RGB32F, false, 0, false, 0}};
        gfx::UniformList   mvpMatrix     = {
            {"u_model", gfx::Type::MAT4, 1},
            {"u_view", gfx::Type::MAT4, 1},
            {"u_projection", gfx::Type::MAT4, 1},
        };
        gfx::UniformBlockList uniformBlockList = {{0, 0, "MVP_Matrix", mvpMatrix, 1}};

        gfx::ShaderInfo shaderInfo;
        shaderInfo.name       = "Bunny";
        shaderInfo.attributes = std::move(attributeList);
        shaderInfo.stages     = std::move(shaderStageList);
        shaderInfo.blocks     = std::move(uniformBlockList);
        shader                = device->createShader(shaderInfo);
    }

    void createBuffers() {
        auto obj = TestBaseI::loadOBJ("bunny.obj");

        // vertex buffer
        const auto &positions = obj.GetAttrib().vertices;
        vertexBuffer          = device->createBuffer({
            gfx::BufferUsage::VERTEX,
            gfx::MemoryUsage::DEVICE,
            static_cast<uint>(positions.size() * sizeof(float)),
            3 * sizeof(float),
        });
        vertexBuffer->update(positions.data(), positions.size() * sizeof(float));

        // index buffer
        const auto &     indicesInfo = obj.GetShapes()[0].mesh.indices;
        vector<uint16_t> indices;
        indices.reserve(indicesInfo.size());
        std::transform(indicesInfo.begin(), indicesInfo.end(), std::back_inserter(indices),
                       [](auto &&info) { return static_cast<uint16_t>(info.vertex_index); });

        indexBuffer = device->createBuffer({
            gfx::BufferUsage::INDEX,
            gfx::MemoryUsage::DEVICE,
            static_cast<uint>(indices.size() * sizeof(uint16_t)),
            sizeof(uint16_t),
        });
        indexBuffer->update(indices.data(), indices.size() * sizeof(uint16_t));

        // uniform buffer
        // create uniform buffer
        gfx::BufferInfo uniformBufferInfo = {
            gfx::BufferUsage::UNIFORM,
            gfx::MemoryUsage::HOST | gfx::MemoryUsage::DEVICE,
            TestBaseI::getUBOSize(3 * sizeof(Mat4)),
        };
        for (auto &i : mvpUniformBuffer) {
            i = device->createBuffer(uniformBufferInfo);
        }
    }

    void createInputAssembler() {
        gfx::Attribute          position = {"a_position", gfx::Format::RGB32F, false, 0, false};
        gfx::InputAssemblerInfo inputAssemblerInfo;
        inputAssemblerInfo.attributes.emplace_back(std::move(position));
        inputAssemblerInfo.vertexBuffers.emplace_back(vertexBuffer);
        inputAssemblerInfo.indexBuffer = indexBuffer;
        inputAssembler                 = device->createInputAssembler(inputAssemblerInfo);
    }

    void createPipeline(gfx::Framebuffer *fbo) {
        gfx::DescriptorSetLayoutInfo dslInfo;
        dslInfo.bindings.push_back({0, gfx::DescriptorType::UNIFORM_BUFFER, 1, gfx::ShaderStageFlagBit::VERTEX});
        descriptorSetLayout = device->createDescriptorSetLayout(dslInfo);

        pipelineLayout = device->createPipelineLayout({{descriptorSetLayout}});

        for (uint i = 0U; i < BUNNY_NUM; i++) {
            descriptorSet[i] = device->createDescriptorSet({descriptorSetLayout});

            descriptorSet[i]->bindBuffer(0, mvpUniformBuffer[i]);
            descriptorSet[i]->update();
        }

        gfx::PipelineStateInfo pipelineInfo;
        pipelineInfo.primitive  = gfx::PrimitiveMode::TRIANGLE_LIST;
        pipelineInfo.shader     = shader;
        pipelineInfo.inputState = {inputAssembler->getAttributes()};
        pipelineInfo.renderPass = fbo->getRenderPass();
        pipelineInfo.blendState.targets.clear();
        pipelineInfo.pipelineLayout = pipelineLayout;

        pipelineState = device->createPipelineState(pipelineInfo);
    }

    void destroy() {
        CC_SAFE_DESTROY_AND_DELETE(shader);
        CC_SAFE_DESTROY_AND_DELETE(vertexBuffer);
        CC_SAFE_DESTROY_AND_DELETE(indexBuffer);
        CC_SAFE_DESTROY_AND_DELETE(depthTexture);
        CC_SAFE_DESTROY_AND_DELETE(inputAssembler);
        for (uint i = 0; i < BUNNY_NUM; i++) {
            CC_SAFE_DESTROY_AND_DELETE(mvpUniformBuffer[i]);
            CC_SAFE_DESTROY_AND_DELETE(descriptorSet[i]);
        }
        CC_SAFE_DESTROY_AND_DELETE(descriptorSetLayout);
        CC_SAFE_DESTROY_AND_DELETE(pipelineLayout);
        CC_SAFE_DESTROY_AND_DELETE(pipelineState);
    }
    const static uint         BUNNY_NUM                   = 2;
    gfx::Device *             device                      = nullptr;
    gfx::Shader *             shader                      = nullptr;
    gfx::Buffer *             vertexBuffer                = nullptr;
    gfx::Buffer *             indexBuffer                 = nullptr;
    gfx::Texture *            depthTexture                = nullptr;
    gfx::InputAssembler *     inputAssembler              = nullptr;
    gfx::DescriptorSetLayout *descriptorSetLayout         = nullptr;
    gfx::PipelineLayout *     pipelineLayout              = nullptr;
    gfx::Buffer *             mvpUniformBuffer[BUNNY_NUM] = {nullptr, nullptr};
    gfx::DescriptorSet *      descriptorSet[BUNNY_NUM]    = {nullptr, nullptr};
    gfx::PipelineState *      pipelineState               = nullptr;
};

BigTriangle *            bg{nullptr};
Bunny *                  bunny{nullptr};
DepthResolveFramebuffer *bunnyFBO{nullptr};
} // namespace

void DepthTexture::onDestroy() {
    CC_SAFE_DESTROY_AND_DELETE(bg);
    CC_SAFE_DESTROY_AND_DELETE(bunny);
    CC_SAFE_DESTROY_AND_DELETE(bunnyFBO);
}

void DepthTexture::onResize(gfx::Swapchain *swapchain) {
    bunnyFBO->resize(swapchain->getWidth(), swapchain->getHeight());
}

bool DepthTexture::onInit() {
    auto *swapchain = swapchains[0];
    auto *fbo       = fbos[0];

    bunnyFBO = ccnew DepthResolveFramebuffer(device, swapchain);
    bunny    = ccnew Bunny(device, bunnyFBO->framebuffer);
    bg       = ccnew BigTriangle(device, fbo);

    gfx::SamplerInfo samplerInfo;
    samplerInfo.mipFilter = gfx::Filter::POINT;
    auto *sampler         = device->getSampler(samplerInfo);

    bg->descriptorSet->bindTexture(1, bunnyFBO->depthStencilTexView);
    bg->descriptorSet->bindSampler(1, sampler);
    bg->descriptorSet->update();

    _generalBarriers.push_back(device->getGeneralBarrier({
        gfx::AccessFlagBit::TRANSFER_WRITE,
        gfx::AccessFlagBit::VERTEX_SHADER_READ_UNIFORM_BUFFER |
            gfx::AccessFlagBit::FRAGMENT_SHADER_READ_UNIFORM_BUFFER |
            gfx::AccessFlagBit::VERTEX_BUFFER |
            gfx::AccessFlagBit::INDEX_BUFFER,
    }));

    _generalBarriers.push_back(device->getGeneralBarrier({
        gfx::AccessFlagBit::TRANSFER_WRITE,
        gfx::AccessFlagBit::VERTEX_SHADER_READ_UNIFORM_BUFFER,
    }));

    return true;
}

void DepthTexture::onTick() {
    auto *swapchain = swapchains[0];
    auto *fbo       = fbos[0];

    uint generalBarrierIdx = _frameCount ? 1 : 0;

    static constexpr float CAMERA_DISTANCE = 8.F;
    _eye.set(CAMERA_DISTANCE * std::cos(_time), CAMERA_DISTANCE * 0.5F, CAMERA_DISTANCE * std::sin(_time));
    _center.set(0, 0.5F, 0);
    _up.set(0, 1.F, 0);
    Mat4::createLookAt(_eye, _center, _up, &_bunnyMatrices[1]);
    gfx::Extent orientedSize = TestBaseI::getOrientedSurfaceSize(swapchain);
    TestBaseI::createPerspective(45.F,
                                 static_cast<float>(orientedSize.width) / static_cast<float>(orientedSize.height),
                                 1.F, 10.F, &_bunnyMatrices[2], swapchain);

    gfx::Color clearColor[2] = {{1.0, 0, 0, 1.0F}};

    device->acquire(&swapchain, 1);

    for (uint i = 0; i < Bunny::BUNNY_NUM; i++) {
        _bunnyMatrices[0].m[12] = i % 2 ? -1.5F : 1.5F;
        bunny->mvpUniformBuffer[i]->update(_bunnyMatrices, sizeof(_bunnyMatrices));
    }

    gfx::Rect renderArea = {0, 0, swapchain->getWidth(), swapchain->getHeight()};

    auto *commandBuffer = commandBuffers[0];
    commandBuffer->begin();

    if (TestBaseI::MANUAL_BARRIER) {
        commandBuffer->pipelineBarrier(_generalBarriers[generalBarrierIdx]);
    }

    // render bunny
    commandBuffer->beginRenderPass(bunnyFBO->renderPass, bunnyFBO->framebuffer, bunnyFBO->bunnyArea, clearColor, 1.0F, 0);
    commandBuffer->bindPipelineState(bunny->pipelineState);
    commandBuffer->bindInputAssembler(bunny->inputAssembler);
    for (auto &i : bunny->descriptorSet) {
        commandBuffer->bindDescriptorSet(0, i);
        commandBuffer->draw(bunny->inputAssembler);
    }
    commandBuffer->endRenderPass();

    // render bg
    commandBuffer->beginRenderPass(fbo->getRenderPass(), fbo, renderArea, clearColor, 1.0F, 0);
    commandBuffer->bindInputAssembler(bg->inputAssembler);
    commandBuffer->bindPipelineState(bg->pipelineState);
    commandBuffer->bindDescriptorSet(0, bg->descriptorSet);
    commandBuffer->draw(bg->inputAssembler);
    commandBuffer->endRenderPass();

    commandBuffer->end();

    device->flushCommands(commandBuffers);
    device->getQueue()->submit(commandBuffers);
    device->present();
}

} // namespace cc
