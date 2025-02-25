#include "SubpassTest.h"
#include "tiny_obj_loader.h"

namespace cc {

bool SubpassTest::onInit() {
    createStandardUniformBuffers(device, &_ubos, swapchains.size());
    createStandardPipelineResources(device, &_forward, _ubos);
    createStandardPipelineResources(device, &_deferred, _ubos);

    auto obj = loadOBJ("bunny.obj");

    // vertex buffer
    const auto &positions = obj.GetAttrib().vertices;
    _vertexPositionBuffer.reset(device->createBuffer({
        gfx::BufferUsage::VERTEX,
        gfx::MemoryUsage::DEVICE,
        static_cast<uint>(positions.size() * sizeof(float)),
        3 * sizeof(float),
    }));
    _vertexPositionBuffer->update(positions.data(), positions.size() * sizeof(float));

    const auto &normals = obj.GetAttrib().normals;
    _vertexNormalBuffer.reset(device->createBuffer({
        gfx::BufferUsage::VERTEX,
        gfx::MemoryUsage::DEVICE,
        static_cast<uint>(normals.size() * sizeof(float)),
        3 * sizeof(float),
    }));
    _vertexNormalBuffer->update(normals.data(), normals.size() * sizeof(float));

    // index buffer
    const auto &     indicesInfo = obj.GetShapes()[0].mesh.indices;
    vector<uint16_t> indices;
    indices.reserve(indicesInfo.size());
    std::transform(indicesInfo.begin(), indicesInfo.end(), std::back_inserter(indices),
                   [](auto &&info) { return static_cast<uint16_t>(info.vertex_index); });

    _indexBuffer.reset(device->createBuffer({
        gfx::BufferUsage::INDEX,
        gfx::MemoryUsage::DEVICE,
        static_cast<uint>(indices.size() * sizeof(uint16_t)),
        sizeof(uint16_t),
    }));
    _indexBuffer->update(indices.data(), indices.size() * sizeof(uint16_t));

    constexpr float cameraDistance = 5.F;

    Vec4 cameraPos{cameraDistance, cameraDistance * 0.5F, cameraDistance * 0.3F, 1.F};
    Vec4 lightDir{0.F, -2.F, -1.F, 0.F};
    Vec4 lightColor{1.F, 1.F, 1.F, 1.7F};
    Vec4 skyColor{.2F, .5F, .8F, .5F};
    Vec4 groundColor{.2F, .2F, .2F, 1.F};
    Mat4 view;
    Mat4::createLookAt(Vec3(cameraPos.x, cameraPos.y, cameraPos.z),
                       Vec3(0.0F, 1.F, 0.0F), Vec3(0.0F, 1.0F, 0.F), &view);
    lightDir.normalize();
    lightDir.w = 1.F;

    for (size_t i = 0; i < swapchains.size(); ++i) {
        std::copy(view.m, view.m + 16, _ubos.getBuffer(standard::MVP, i) + 16);
        std::copy(&cameraPos.x, &cameraPos.x + 4, _ubos.getBuffer(standard::CAMERA, i));
        std::copy(&lightDir.x, &lightDir.x + 4, _ubos.getBuffer(standard::CAMERA, i) + 4);
        std::copy(&lightColor.x, &lightColor.x + 4, _ubos.getBuffer(standard::CAMERA, i) + 8);
        std::copy(&skyColor.x, &skyColor.x + 4, _ubos.getBuffer(standard::CAMERA, i) + 12);
        std::copy(&groundColor.x, &groundColor.x + 4, _ubos.getBuffer(standard::CAMERA, i) + 16);
    }

    gfx::InputAssemblerInfo inputAssemblerInfo;
    inputAssemblerInfo.attributes = _deferred.gbufferShader->getAttributes();
    inputAssemblerInfo.vertexBuffers.emplace_back(_vertexPositionBuffer.get());
    inputAssemblerInfo.vertexBuffers.emplace_back(_vertexNormalBuffer.get());
    inputAssemblerInfo.indexBuffer = _indexBuffer.get();
    _inputAssembler.reset(device->createInputAssembler(inputAssemblerInfo));

    _generalBarriers.push_back(device->getGeneralBarrier({
        gfx::AccessFlagBit::TRANSFER_WRITE,
        gfx::AccessFlagBit::VERTEX_SHADER_READ_UNIFORM_BUFFER |
            gfx::AccessFlagBit::FRAGMENT_SHADER_READ_UNIFORM_BUFFER |
            gfx::AccessFlagBit::VERTEX_BUFFER |
            gfx::AccessFlagBit::INDEX_BUFFER,
    }));

    _generalBarriers.push_back(device->getGeneralBarrier({
        gfx::AccessFlagBit::TRANSFER_WRITE,
        gfx::AccessFlagBit::VERTEX_SHADER_READ_UNIFORM_BUFFER |
            gfx::AccessFlagBit::FRAGMENT_SHADER_READ_UNIFORM_BUFFER,
    }));

    return true;
}

void SubpassTest::onSpacePressed() {
    _useDeferred = !_useDeferred;
    CC_LOG_INFO("Shading mode switched to: %s", _useDeferred ? "Deferred" : "Forward");
}

void SubpassTest::onTick() {
    uint  generalBarrierIdx = _frameCount ? 1 : 0;
    auto *commandBuffer    = commandBuffers[0];
    Vec4  colors[]{
        {252 / 255.F, 23 / 255.F, 3 / 255.F, 1.0F},
        {3 / 255.F, 252 / 255.F, 23 / 255.F, 1.0F},
    };

    device->acquire(swapchains);
    commandBuffer->begin();

    for (size_t i = 0; i < swapchains.size(); ++i) {
        auto *swapchain = swapchains[i];
        Mat4::createRotationY(_time * (i ? -1.F : 1.F), &_worldMatrix);
        std::copy(_worldMatrix.m, _worldMatrix.m + 16, _ubos.getBuffer(standard::MVP, i));

        gfx::Extent orientedSize = TestBaseI::getOrientedSurfaceSize(swapchain);
        TestBaseI::createPerspective(60.0F,
                                     static_cast<float>(orientedSize.width) / static_cast<float>(orientedSize.height),
                                     0.01F, 1000.0F, &_projectionMatrix, swapchain);
        std::copy(_projectionMatrix.m, _projectionMatrix.m + 16, _ubos.getBuffer(standard::MVP, i) + 32);

        std::copy(&colors[i].x, &colors[i].x + 4, _ubos.getBuffer(standard::COLOR, i));
    }

    _ubos.update();
    if (TestBaseI::MANUAL_BARRIER) {
        commandBuffer->pipelineBarrier(_generalBarriers[generalBarrierIdx]);
    }

    fg.reset();

    for (size_t i = 0; i < swapchains.size(); ++i) {
        auto *    swapchain = swapchains[i];
        auto *    fbo       = fbos[i];
        gfx::Rect renderArea{0, 0, swapchain->getWidth(), swapchain->getHeight()};

        if (_useDeferred) {
            _deferred.recordCommandBuffer(device, commandBuffer, fbo, renderArea, _clearColor, [=]() {
                commandBuffer->bindInputAssembler(_inputAssembler.get());
                _ubos.bindDescriptorSet(commandBuffer, 0, i);
                commandBuffer->draw(_inputAssembler.get());
            });
        } else {
            _forward.recordCommandBuffer(device, commandBuffer, fbo, renderArea, _clearColor, [=]() {
                commandBuffer->bindInputAssembler(_inputAssembler.get());
                _ubos.bindDescriptorSet(commandBuffer, 0, i);
                commandBuffer->draw(_inputAssembler.get());
            });
        }
    }

    fg.compile();
    //fg.exportGraphViz("fg_vis.dot");
    fg.execute();

    commandBuffer->end();

    device->flushCommands(commandBuffers);
    device->getQueue()->submit(commandBuffers);
    device->present();
}

} // namespace cc
