/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:29:35 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 14:45:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer.h"
#include "icommand_buffer.h"
#include "vertex_buffer.h"
#include "pfd_set.h"
#include "world_set.h"
#include "gbuffer_set.h"
#include "push_constant.h"
#include "game_state.h"
#include "texture.h"

#include "main_render_pass.h"
#include "shadow_render_pass.h"
#include "deferred_render_pass.h"
#include "ssao_render_pass.h"
#include "ssao_blur_render_pass.h"

#include "scene_pipeline.h"
#include "deferred_pipeline.h"
#include "skybox_pipeline.h"
#include "starfield_pipeline.h"
#include "shadow_pipeline.h"
#include "debug_tex_pipeline.h"
#include "ssao_pipeline.h"
#include "ssao_blur_pipeline.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Renderer::init(ui::Window& window, const game::GameState& game) {
    m_core.init(window);
    m_device.init(m_core);
    m_swapChain.init(m_core, m_device, window);
    m_commandPool.init(m_device);

    _createCommandBuffers();

    const ICommandBuffer* transferBuffer = m_commandBuffers[(u32)CommandBufferIndex::Transfer];
    m_textureTable.init(m_device, transferBuffer);
    m_descriptorTable.init(m_device, transferBuffer);

    _createRenderPasses();

    m_descriptorPool.init(m_device, m_descriptorTable);
    m_descriptorTable.fill(m_device);

    _createPushConstants();
    _createPipelineLayouts();
    _createPipelines();
    _createFences();
    _createGfxSemaphores();

#if ENABLE_FRUSTUM_CULLING
    VertexBuffer::init(m_device, game);
#else
    VertexBuffer::init(m_device, transferBuffer, game);
#endif

    LDEBUG("Renderer initialized.");
}

void Renderer::destroy() {
    VertexBuffer::destroy(m_device);
    m_textureTable.destroy(m_device);
    m_descriptorTable.destroy(m_device);
    m_descriptorPool.destroy(m_device);

    _destroyGfxSemaphores();
    _destroyFences();
    _destroyPipelines();
    _destroyPipelineLayouts();
    _destroyPushConstants();
    _destroyRenderPasses();
    _destroyCommandBuffers();

    m_commandPool.destroy(m_device);
    m_swapChain.destroy(m_device);
    m_device.destroy();
    m_core.destroy();

    LDEBUG("Renderer destroyed.");
}

/* ========================================================================== */

void Renderer::waitIdle() const {
    m_device.idle();
}

void Renderer::render(const game::GameState& game) {
    // Prepare frame resources ---------
    m_fences[(u32)FenceIndex::DrawInFlight].await(m_device);
    m_pushConstants[(u32)PushConstantIndex::Camera]->update(game);
    m_descriptorTable.update(game);
#if ENABLE_FRUSTUM_CULLING
    VertexBuffer::update(m_device, game);
#endif
    if (m_swapChain.acquireNextImage(m_device, m_semaphores[(u32)SemaphoreIndex::ImageAvailable]) == false)
        // TODO: Handle this error
        return;
    m_fences[(u32)FenceIndex::DrawInFlight].reset(m_device);
    // --------------------------------

    const PushConstant* cameraConstant = m_pushConstants[(u32)PushConstantIndex::Camera];
    RecordInfo  recordInfo{};

    // Record offscreen commands -------
    const ICommandBuffer* offscreenBuffer = m_commandBuffers[(u32)CommandBufferIndex::Offscreen];
    offscreenBuffer->reset();
    offscreenBuffer->startRecording();
    recordInfo.m_targetIndex = 0;

#if ENABLE_SHADOW_MAPPING
    // Shadow pass
    m_renderPasses[(u32)RenderPassIndex::Shadow]->begin(offscreenBuffer, recordInfo);
    m_pipelines[(u32)PipelineIndex::ShadowPipeline]->record(offscreenBuffer);
    m_renderPasses[(u32)RenderPassIndex::Shadow]->end(offscreenBuffer);
#endif

    // Deferred pass
    cameraConstant->bind(offscreenBuffer, m_pipelineLayouts[(u32)PipelineLayoutIndex::Deferred]);
    m_renderPasses[(u32)RenderPassIndex::Deferred]->begin(offscreenBuffer, recordInfo);
    m_pipelines[(u32)PipelineIndex::Deferred]->record(offscreenBuffer);
    m_renderPasses[(u32)RenderPassIndex::Deferred]->end(offscreenBuffer);

#if ENABLE_SSAO
    // Ssao pass
    cameraConstant->bind(offscreenBuffer, m_pipelineLayouts[(u32)PipelineLayoutIndex::Ssao]);
    m_renderPasses[(u32)RenderPassIndex::Ssao]->begin(offscreenBuffer, recordInfo);
    m_pipelines[(u32)PipelineIndex::Ssao]->record(offscreenBuffer);
    m_renderPasses[(u32)RenderPassIndex::Ssao]->end(offscreenBuffer);

    // Ssao blur pass
    m_renderPasses[(u32)RenderPassIndex::SsaoBlur]->begin(offscreenBuffer, recordInfo);
    m_pipelines[(u32)PipelineIndex::SsaoBlur]->record(offscreenBuffer);
    m_renderPasses[(u32)RenderPassIndex::SsaoBlur]->end(offscreenBuffer);
#endif

    offscreenBuffer->stopRecording();
    // --------------------------------

    // Record composition commands ----
    const ICommandBuffer* drawBuffer = m_commandBuffers[(u32)CommandBufferIndex::Draw];
    drawBuffer->reset();
    drawBuffer->startRecording();

    // Main pass
    recordInfo.m_targetIndex = m_swapChain.getImageIndex();

    m_renderPasses[(u32)RenderPassIndex::Main]->begin(drawBuffer, recordInfo);
#if ENABLE_SKYBOX
    cameraConstant->bind(drawBuffer, m_pipelineLayouts[(u32)PipelineLayoutIndex::Sky]);
    m_pipelines[(u32)PipelineIndex::SkyboxPipeline]->record(drawBuffer);
    m_pipelines[(u32)PipelineIndex::StarfieldPipeline]->record(drawBuffer);
#endif

    cameraConstant->bind(drawBuffer, m_pipelineLayouts[(u32)PipelineLayoutIndex::Scene]);
    m_pipelines[(u32)PipelineIndex::ScenePipeline]->record(drawBuffer);
    if (game.getController().showDebug())
        m_pipelines[(u32)PipelineIndex::DebugPipeline]->record(drawBuffer);
    m_renderPasses[(u32)RenderPassIndex::Main]->end(drawBuffer);

    drawBuffer->stopRecording();
    // --------------------------------

    // Submit command buffer ---------
    { // Offscreen buffer
        static const std::vector<VkPipelineStageFlags>  waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        static const std::vector<VkSemaphore>           waitSemaphores = { m_semaphores[(u32)SemaphoreIndex::ImageAvailable].getSemaphore() };
        static const std::vector<VkSemaphore>           signalSemaphores = { m_semaphores[(u32)SemaphoreIndex::OffscreenFinished].getSemaphore() };
        offscreenBuffer->submitRecording(waitSemaphores, waitStages, signalSemaphores);
    }
    { // Draw buffer
        static const std::vector<VkPipelineStageFlags>  waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        static const std::vector<VkSemaphore>           waitSemaphores = { m_semaphores[(u32)SemaphoreIndex::OffscreenFinished].getSemaphore() };
        static const std::vector<VkSemaphore>           signalSemaphores = { m_semaphores[(u32)SemaphoreIndex::RenderFinished].getSemaphore() };
        drawBuffer->submitRecording(waitSemaphores, waitStages, signalSemaphores, m_fences[(u32)FenceIndex::DrawInFlight].getFence());
    }
    // --------------------------------

    // Present image ------------------
    if (m_swapChain.submitImage(m_device, m_semaphores[(u32)SemaphoreIndex::RenderFinished]) == false)
        // TODO: Handle this error
        return;
    // --------------------------------
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void Renderer::_createCommandBuffers() {
    m_commandBuffers.reserve(CMD_BUFFER_COUNT);

    // Draw buffers
    for (u32 i = 0; i < CMD_BUFFER_COUNT; ++i) m_commandBuffers[i] = m_commandPool.createCommandBuffer(m_device, CommandBufferType::DRAW);

    LDEBUG("Command buffers created.");
}

void Renderer::_createRenderPasses() {
    m_renderPasses.reserve(RENDER_PASS_COUNT);

    { // Composition pass
        m_renderPasses[(u32)RenderPassIndex::Main] = new MainRenderPass();

        MainRenderPassInfo mainPassInfo(m_swapChain.getImageViews());
        mainPassInfo.m_formats.resize(MainRenderPass::RESOURCE_COUNT, VK_FORMAT_UNDEFINED);
        mainPassInfo.m_formats[(u32)MainRenderPass::Resource::ColorImage] = m_swapChain.getImageFormat();
        mainPassInfo.m_formats[(u32)MainRenderPass::Resource::DepthImage] = m_swapChain.getDepthFormat();
        mainPassInfo.m_samples.resize(MainRenderPass::RESOURCE_COUNT, VK_SAMPLE_COUNT_1_BIT);
        mainPassInfo.m_samples[(u32)MainRenderPass::Resource::ColorImage] = m_device.getMsaaCount();
        mainPassInfo.m_samples[(u32)MainRenderPass::Resource::DepthImage] = m_device.getMsaaCount();
        mainPassInfo.m_renderPassWidth = SwapChain::getImageExtent().width;
        mainPassInfo.m_renderPassHeight = SwapChain::getImageExtent().height;
        mainPassInfo.m_targetCount = m_swapChain.getImageViews().size();
        mainPassInfo.m_targetWidth = SwapChain::getImageExtent().width;
        mainPassInfo.m_targetHeight = SwapChain::getImageExtent().height;
        m_renderPasses[(u32)RenderPassIndex::Main]->init(m_device, &mainPassInfo);
    }

    { // Deferred pass
        m_renderPasses[(u32)RenderPassIndex::Deferred] = new DeferredRenderPass();

        const ImageBuffer& positionTex = TextureTable::getTexture(TextureIndex::GBufferPosition)->getImageBuffer();
        const ImageBuffer& normalTex = TextureTable::getTexture(TextureIndex::GBufferNormal)->getImageBuffer();
        const ImageBuffer& albedoTex = TextureTable::getTexture(TextureIndex::GBufferAlbedo)->getImageBuffer();
        const ImageBuffer& depthTex = TextureTable::getTexture(TextureIndex::GBufferDepth)->getImageBuffer();

#if ENABLE_SSAO
        const ImageBuffer& normalViewTex = TextureTable::getTexture(TextureIndex::GBufferNormalView)->getImageBuffer();
        const ImageBuffer& posViewTex = TextureTable::getTexture(TextureIndex::GBufferPositionView)->getImageBuffer();
        DeferredRenderPassInfo  deferredPassInfo(positionTex, normalTex, albedoTex, depthTex, normalViewTex, posViewTex);
#else
        DeferredRenderPassInfo  deferredPassInfo(positionTex, normalTex, albedoTex, depthTex);
#endif

        deferredPassInfo.m_renderPassWidth = SwapChain::getImageExtent().width;
        deferredPassInfo.m_renderPassHeight = SwapChain::getImageExtent().height;
        deferredPassInfo.m_targetCount = 1;
        deferredPassInfo.m_targetWidth = SwapChain::getImageExtent().width;
        deferredPassInfo.m_targetHeight = SwapChain::getImageExtent().height;
        m_renderPasses[(u32)RenderPassIndex::Deferred]->init(m_device, &deferredPassInfo);
    }

#if ENABLE_SSAO
    { // SSAO pass
        m_renderPasses[(u32)RenderPassIndex::Ssao] = new SSAORenderPass();

        const ImageBuffer& ssaoTex = TextureTable::getTexture(TextureIndex::GBufferSSAO)->getImageBuffer();

        SSAORenderPassInfo  ssaoPassInfo(ssaoTex);
        ssaoPassInfo.m_formats.resize(SSAORenderPass::RESOURCE_COUNT);
        ssaoPassInfo.m_formats[(u32)SSAORenderPass::Resource::Color] = ssaoTex.getMetaData().m_format;
        ssaoPassInfo.m_samples.resize(SSAORenderPass::RESOURCE_COUNT, VK_SAMPLE_COUNT_1_BIT);
        ssaoPassInfo.m_renderPassWidth = SwapChain::getImageExtent().width;
        ssaoPassInfo.m_renderPassHeight = SwapChain::getImageExtent().height;
        ssaoPassInfo.m_targetCount = 1;
        ssaoPassInfo.m_targetWidth = SwapChain::getImageExtent().width;
        ssaoPassInfo.m_targetHeight = SwapChain::getImageExtent().height;
        m_renderPasses[(u32)RenderPassIndex::Ssao]->init(m_device, &ssaoPassInfo);
    }
    { // SSAO blur pass
        m_renderPasses[(u32)RenderPassIndex::SsaoBlur] = new SSAOBlurRenderPass();

        const ImageBuffer& ssaoBlurTex = TextureTable::getTexture(TextureIndex::GBufferSSAOBlur)->getImageBuffer();

        SSAOBlurRenderPassInfo  blurPassInfo(ssaoBlurTex);
        blurPassInfo.m_formats.resize(SSAOBlurRenderPass::RESOURCE_COUNT);
        blurPassInfo.m_formats[(u32)SSAOBlurRenderPass::Resource::Color] = ssaoBlurTex.getMetaData().m_format;
        blurPassInfo.m_samples.resize(SSAOBlurRenderPass::RESOURCE_COUNT, VK_SAMPLE_COUNT_1_BIT);
        blurPassInfo.m_renderPassWidth = SwapChain::getImageExtent().width;
        blurPassInfo.m_renderPassHeight = SwapChain::getImageExtent().height;
        blurPassInfo.m_targetCount = 1;
        blurPassInfo.m_targetWidth = SwapChain::getImageExtent().width;
        blurPassInfo.m_targetHeight = SwapChain::getImageExtent().height;
        m_renderPasses[(u32)RenderPassIndex::SsaoBlur]->init(m_device, &blurPassInfo);
    }
#endif

#if ENABLE_SHADOW_MAPPING
    // Shadow pass
    m_renderPasses[(u32)RenderPassIndex::Shadow] = new ShadowRenderPass();

    const ImageBuffer& shadowmap = TextureTable::getTexture(TextureIndex::ShadowMap)->getImageBuffer();

    ShadowRenderPassInfo shadowPassInfo(shadowmap);
    shadowPassInfo.m_formats.resize(ShadowRenderPass::RESOURCE_COUNT, shadowmap.getMetaData().m_format);
    shadowPassInfo.m_samples.resize(ShadowRenderPass::RESOURCE_COUNT, shadowmap.getMetaData().m_sampleCount);
    shadowPassInfo.m_renderPassWidth = shadowmap.getMetaData().m_width;
    shadowPassInfo.m_renderPassHeight = shadowmap.getMetaData().m_height;
    shadowPassInfo.m_targetCount = 1;
    shadowPassInfo.m_targetWidth = shadowmap.getMetaData().m_width;
    shadowPassInfo.m_targetHeight = shadowmap.getMetaData().m_height;
    m_renderPasses[(u32)RenderPassIndex::Shadow]->init(m_device, &shadowPassInfo);
#endif
}

void Renderer::_createPushConstants() {
    m_pushConstants.reserve(PUSH_CONSTANT_COUNT);

    m_pushConstants[(u32)PushConstantIndex::Camera] = new CameraPushConstant();
}

void Renderer::_createPipelineLayouts() {
    m_pipelineLayouts.resize(PIPELINE_LAYOUT_COUNT);

    std::vector<const IDescriptorSet*> sets;

    { // Scene
        sets = {
            m_descriptorTable[DescriptorSetIndex::Pfd],
            m_descriptorTable[DescriptorSetIndex::GBuffer] };
        m_pipelineLayouts[(u32)PipelineLayoutIndex::Scene].init(m_device, sets, m_pushConstants[(u32)PushConstantIndex::Camera]);
    }
    { // Deferred
        sets = { m_descriptorTable[DescriptorSetIndex::WorldData] };
        m_pipelineLayouts[(u32)PipelineLayoutIndex::Deferred].init(m_device, sets, m_pushConstants[(u32)PushConstantIndex::Camera]);
    }
#if ENABLE_SKYBOX
    { // Sky
        sets = {
            m_descriptorTable[DescriptorSetIndex::Pfd],
            m_descriptorTable[DescriptorSetIndex::WorldData] };
        m_pipelineLayouts[(u32)PipelineLayoutIndex::Sky].init(m_device, sets, m_pushConstants[(u32)PushConstantIndex::Camera]);
    }
#endif

#if ENABLE_SSAO
    { // Ssao
        sets = { m_descriptorTable[DescriptorSetIndex::Ssao] };
        m_pipelineLayouts[(u32)PipelineLayoutIndex::Ssao].init(m_device, sets, m_pushConstants[(u32)PushConstantIndex::Camera]);
    }
    { // Ssao blur
        sets = { m_descriptorTable[DescriptorSetIndex::SsaoBlur] };
        m_pipelineLayouts[(u32)PipelineLayoutIndex::SsaoBlur].init(m_device, sets);
    }
#endif

#if ENABLE_SHADOW_MAPPING
    { // Sky
        sets = { m_descriptorTable[DescriptorSetIndex::Pfd] };
        m_pipelineLayouts[(u32)PipelineLayoutIndex::Shadows].init(m_device, sets);
    }
#endif
}

void Renderer::_createPipelines() {
    m_pipelines.reserve(PIPELINE_COUNT);

    const VkRenderPass mainRenderPass = m_renderPasses[(u32)RenderPassIndex::Main]->getRenderPass();

    m_pipelines[(u32)PipelineIndex::ScenePipeline] = new ScenePipeline();
    m_pipelines[(u32)PipelineIndex::ScenePipeline]->init(m_device, mainRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Scene]);
    m_pipelines[(u32)PipelineIndex::DebugPipeline] = new DebugTexPipeline();
    m_pipelines[(u32)PipelineIndex::DebugPipeline]->init(m_device, mainRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Scene]);

#if ENABLE_SKYBOX
    m_pipelines[(u32)PipelineIndex::SkyboxPipeline] = new SkyboxPipeline();
    m_pipelines[(u32)PipelineIndex::SkyboxPipeline]->init(m_device, mainRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Sky]);
    m_pipelines[(u32)PipelineIndex::StarfieldPipeline] = new StarfieldPipeline();
    m_pipelines[(u32)PipelineIndex::StarfieldPipeline]->init(m_device, mainRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Sky]);
#endif

    const VkRenderPass deferredRenderpass = m_renderPasses[(u32)RenderPassIndex::Deferred]->getRenderPass();

    m_pipelines[(u32)PipelineIndex::Deferred] = new DeferredPipeline();
    m_pipelines[(u32)PipelineIndex::Deferred]->init(m_device, deferredRenderpass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Deferred]);

#if ENABLE_SSAO
    const VkRenderPass ssaoRenderPass = m_renderPasses[(u32)RenderPassIndex::Ssao]->getRenderPass();

    m_pipelines[(u32)PipelineIndex::Ssao] = new SSAOPipeline();
    m_pipelines[(u32)PipelineIndex::Ssao]->init(m_device, ssaoRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Ssao]);

    const VkRenderPass ssaoBlurRenderPass = m_renderPasses[(u32)RenderPassIndex::SsaoBlur]->getRenderPass();

    m_pipelines[(u32)PipelineIndex::SsaoBlur] = new SSAOBlurPipeline();
    m_pipelines[(u32)PipelineIndex::SsaoBlur]->init(m_device, ssaoBlurRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::SsaoBlur]);
#endif

#if ENABLE_SHADOW_MAPPING
    VkRenderPass shadowRenderPass = m_renderPasses[(u32)RenderPassIndex::Shadow]->getRenderPass();

    m_pipelines[(u32)PipelineIndex::ShadowPipeline] = new ShadowPipeline();
    m_pipelines[(u32)PipelineIndex::ShadowPipeline]->init(m_device, shadowRenderPass, m_pipelineLayouts[(u32)PipelineLayoutIndex::Shadows]);
#endif

    LDEBUG("Pipelines created.");
}

void Renderer::_createGfxSemaphores() {
    m_semaphores.resize(SEMAPHORE_COUNT);
    for (GfxSemaphore& semaphore : m_semaphores) semaphore.init(m_device);
}

void Renderer::_createFences() {
    m_fences.resize(FENCE_COUNT);
    for (Fence& fence : m_fences) fence.init(m_device, VK_FENCE_CREATE_SIGNALED_BIT);
}

void Renderer::_destroyCommandBuffers() {
    for (u32 i = 0; i < CMD_BUFFER_COUNT; ++i) m_commandPool.destroyBuffer(m_device, m_commandBuffers[i]);
}

void Renderer::_destroyRenderPasses() {
    for (u32 i = 0; i < RENDER_PASS_COUNT; i++) {
        m_renderPasses[i]->destroy(m_device);
        delete m_renderPasses[i];
    }
}

void Renderer::_destroyPipelines() {
    for (u32 i = 0; i < PIPELINE_COUNT; ++i) {
        m_pipelines[i]->destroy(m_device);
        delete m_pipelines[i];
    }
}

void Renderer::_destroyPipelineLayouts() {
    for (PipelineLayout& layout : m_pipelineLayouts) layout.destroy(m_device);
}

void Renderer::_destroyPushConstants() {
    for (u32 i = 0; i < PUSH_CONSTANT_COUNT; ++i) {
        delete m_pushConstants[i];
        LDEBUG("Destroyed push constant " << i);
    }
}

void Renderer::_destroyGfxSemaphores() {
    for (GfxSemaphore& semaphore : m_semaphores) semaphore.destroy(m_device);
}

void Renderer::_destroyFences() {
    for (Fence& fence : m_fences) fence.destroy(m_device);
}

} // namespace vox::gfx