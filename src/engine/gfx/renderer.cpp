/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:29:35 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 20:02:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer.h"
#include "main_render_pass.h"
#include "shadow_render_pass.h"
#include "icommand_buffer.h"
#include "scene_pipeline.h"
#include "skybox_pipeline.h"
#include "starfield_pipeline.h"
#include "shadow_pipeline.h"
#include "vertex_buffer.h"
#include "pfd_set.h"
#include "vox_decl.h"

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
    m_descriptorTable.init(m_device, transferBuffer, game);

    _createRenderPasses();
    _createPipelineLayout();
    _createPipelines();
    _createFences();
    _createGfxSemaphores();

    m_descriptorPool.init(m_device, m_descriptorTable);
    m_descriptorTable.fill(m_device);

#if ENABLE_FRUSTUM_CULLING
    // VertexBuffer::computeMaxVertexInstanceCount(game);
    VertexBuffer::init(m_device, game);
#else
    VertexBuffer::init(m_device, transferBuffer, game);
#endif

    LDEBUG("Renderer initialized.");
}

void Renderer::destroy() {
    VertexBuffer::destroy(m_device);
    m_descriptorTable.destroy(m_device);
    m_descriptorPool.destroy(m_device);

    _destroyGfxSemaphores();
    _destroyFences();
    _destroyPipelines();
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
    m_descriptorTable.update(game);

    // Retrieve swap chain image ------
    m_fences[(u32)FenceIndex::DrawInFlight].await(m_device);

#if ENABLE_FRUSTUM_CULLING
    VertexBuffer::update(m_device, game);
#endif

    if (m_swapChain.acquireNextImage(m_device, m_semaphores[(u32)SemaphoreIndex::ImageAvailable]) == false)
        // TODO: Handle this error
        return;
    m_fences[(u32)FenceIndex::DrawInFlight].reset(m_device);
    // --------------------------------

    // Record command buffer ---------
    const ICommandBuffer* drawBuffer = m_commandBuffers[(u32)CommandBufferIndex::Draw];
    const RenderPass* mainRenderPass = m_renderPasses[(u32)RenderPassIndex::Main];

    drawBuffer->reset();
    drawBuffer->startRecording();

    VkViewport viewport{};
    viewport.width = (f32)mainRenderPass->getWidth();
    viewport.height = (f32)mainRenderPass->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(drawBuffer->getBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { mainRenderPass->getWidth(), mainRenderPass->getHeight() };
    vkCmdSetScissor(drawBuffer->getBuffer(), 0, 1, &scissor);

#if ENABLE_SHADOW_MAPPING
    const RenderPass* shadowRenderPass = m_renderPasses[(u32)RenderPassIndex::Shadow];

    shadowRenderPass->begin(drawBuffer, {});
    m_pipelines[(u32)PipelineIndex::ShadowPipeline]->record(m_pipelineLayout, m_descriptorTable, drawBuffer);
    shadowRenderPass->end(drawBuffer);
#endif

    RecordInfo  mainRecordInfo{};
    mainRecordInfo.m_targetIndex = m_swapChain.getImageIndex();
    mainRenderPass->begin(drawBuffer, mainRecordInfo);
#if ENABLE_SKYBOX
    m_pipelines[(u32)PipelineIndex::SkyboxPipeline]->record(m_pipelineLayout, m_descriptorTable, drawBuffer);
    m_pipelines[(u32)PipelineIndex::StarfieldPipeline]->record(m_pipelineLayout, m_descriptorTable, drawBuffer);
#endif
    m_pipelines[(u32)PipelineIndex::ScenePipeline]->record(m_pipelineLayout, m_descriptorTable, drawBuffer);
    mainRenderPass->end(drawBuffer);

    drawBuffer->stopRecording();
    // --------------------------------

    // Submit command buffer ---------
    static const std::vector<VkSemaphore>           signalSemaphores = { m_semaphores[(u32)SemaphoreIndex::RenderFinished].getSemaphore() };
    static const std::vector<VkSemaphore>           waitSemaphores = { m_semaphores[(u32)SemaphoreIndex::ImageAvailable].getSemaphore() };
    static const std::vector<VkPipelineStageFlags>  waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    drawBuffer->submitRecording(waitSemaphores, waitStages, signalSemaphores, m_fences[(u32)FenceIndex::DrawInFlight]);
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
    // Draw buffers
    for (u32 i = 0; i < CMD_BUFFER_COUNT; ++i) m_commandBuffers[i] = m_commandPool.createCommandBuffer(m_device, CommandBufferType::DRAW);

    LDEBUG("Command buffers created.");
}

void Renderer::_createRenderPasses() {
    m_renderPasses[(u32)RenderPassIndex::Main] = new MainRenderPass();

    MainRenderPassInfo mainPassInfo(m_swapChain.getImageViews());
    mainPassInfo.m_formats.resize(MainRenderPass::RESOURCE_COUNT, VK_FORMAT_UNDEFINED);
    mainPassInfo.m_formats[(u32)MainRenderPass::Resource::ColorImage] = m_swapChain.getImageFormat();
    mainPassInfo.m_formats[(u32)MainRenderPass::Resource::DepthImage] = m_swapChain.getDepthFormat();
    mainPassInfo.m_samples.resize(MainRenderPass::RESOURCE_COUNT, VK_SAMPLE_COUNT_1_BIT);
    mainPassInfo.m_samples[(u32)MainRenderPass::Resource::ColorImage] = m_device.getMsaaCount();
    mainPassInfo.m_samples[(u32)MainRenderPass::Resource::DepthImage] = m_device.getMsaaCount();
    mainPassInfo.m_renderPassWidth = m_swapChain.getImageExtent().width;
    mainPassInfo.m_renderPassHeight = m_swapChain.getImageExtent().height;
    mainPassInfo.m_targetCount = m_swapChain.getImageViews().size();
    mainPassInfo.m_targetWidth = m_swapChain.getImageExtent().width;
    mainPassInfo.m_targetHeight = m_swapChain.getImageExtent().height;
    m_renderPasses[(u32)RenderPassIndex::Main]->init(m_device, &mainPassInfo);

#if ENABLE_SHADOW_MAPPING
    m_renderPasses[(u32)RenderPassIndex::Shadow] = new ShadowRenderPass();

    const ImageBuffer& shadowmap = ((const PFDSet*)m_descriptorTable[DescriptorSetIndex::Pfd])->getShadowmap();

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

void Renderer::_createPipelines() {
    m_pipelines[(u32)PipelineIndex::ScenePipeline] = new ScenePipeline();

    VkRenderPass mainRenderPass = m_renderPasses[(u32)RenderPassIndex::Main]->getRenderPass();
    m_pipelines[(u32)PipelineIndex::ScenePipeline]->init(m_device, mainRenderPass, m_pipelineLayout);

#if ENABLE_SKYBOX
    m_pipelines[(u32)PipelineIndex::SkyboxPipeline] = new SkyboxPipeline();
    m_pipelines[(u32)PipelineIndex::StarfieldPipeline] = new StarfieldPipeline();
    m_pipelines[(u32)PipelineIndex::SkyboxPipeline]->init(m_device, mainRenderPass, m_pipelineLayout);
    m_pipelines[(u32)PipelineIndex::StarfieldPipeline]->init(m_device, mainRenderPass, m_pipelineLayout);
#endif
#if ENABLE_SHADOW_MAPPING
    m_pipelines[(u32)PipelineIndex::ShadowPipeline] = new ShadowPipeline();

    VkRenderPass shadowRenderPass = m_renderPasses[(u32)RenderPassIndex::Shadow]->getRenderPass();
    m_pipelines[(u32)PipelineIndex::ShadowPipeline]->init(m_device, shadowRenderPass, m_pipelineLayout);
#endif

    LDEBUG("Pipelines created.");
}

void Renderer::_createPipelineLayout() {
    std::array<VkDescriptorSetLayout, DESCRIPTOR_TABLE_SIZE> setLayouts;
    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i)
        setLayouts[i] = m_descriptorTable[i]->getLayout();

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = DESCRIPTOR_TABLE_SIZE;
    layoutInfo.pSetLayouts = setLayouts.data();
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(m_device.getDevice(), &layoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create pipeline layout.");
}

void Renderer::_createGfxSemaphores() {
    for (GfxSemaphore& semaphore : m_semaphores) semaphore.init(m_device);
}

void Renderer::_createFences() {
    for (Fence& fence : m_fences) fence.init(m_device, VK_FENCE_CREATE_SIGNALED_BIT);
}

/* ========================================================================== */

void Renderer::_destroyCommandBuffers() {
    for (u32 i = 0; i < CMD_BUFFER_COUNT; ++i) m_commandPool.destroyBuffer(m_device, m_commandBuffers[i]);
}

void Renderer::_destroyRenderPasses() {
    for (u32 i = 0; i < RENDER_PASS_COUNT; i++) m_renderPasses[i]->destroy(m_device);
}

void Renderer::_destroyPipelines() {
    _destroyPipelineLayout();

    for (u32 i = 0; i < PIPELINE_COUNT; ++i) {
        m_pipelines[i]->destroy(m_device);
        delete m_pipelines[i];
    }
}

void Renderer::_destroyPipelineLayout() {
    vkDestroyPipelineLayout(m_device.getDevice(), m_pipelineLayout, nullptr);
}

void Renderer::_destroyGfxSemaphores() {
    for (GfxSemaphore& semaphore : m_semaphores) semaphore.destroy(m_device);
}

void Renderer::_destroyFences() {
    for (Fence& fence : m_fences) fence.destroy(m_device);
}

} // namespace vox::gfx