/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:29:35 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 20:42:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer.h"

#include "scene_pipeline.h"
#include "scene_render_pass.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Renderer::Renderer() {
    m_pipelines[(u32)PipelineIndex::ScenePipeline] = new ScenePipeline();
}

Renderer::~Renderer() {
    for (u32 i = 0; i < PIPELINE_COUNT; ++i) delete m_pipelines[i];
}

/* ========================================================================== */

void Renderer::init(ui::Window& window, const GameState& game) {
    m_core.init(window);
    m_device.init(m_core);
    m_swapChain.init(m_core, m_device, window);
    m_commandPool.init(m_device);

    _createCommandBuffers();

    m_descriptorTable.init(m_device);

    _createPipelines();
    _createFences();
    _createGfxSemaphores();

    m_descriptorTable.fill(m_device, game);
    m_descriptorPool.init(m_device, m_descriptorTable);

    LINFO("Renderer initialized.");
}

void Renderer::destroy() {
    m_descriptorPool.destroy(m_device);
    m_descriptorTable.destroy(m_device);

    _destroyGfxSemaphores();
    _destroyFences();
    _destroyPipelines();

    m_swapChain.destroy(m_device);
    m_device.destroy();
    m_core.destroy();
    LINFO("Renderer destroyed.");
}

/* ========================================================================== */

void Renderer::waitIdle() const {
    m_device.idle();
}

void Renderer::render() {

}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void Renderer::_createCommandBuffers() {
    m_commandBuffers[(u32)CommandBufferIndex::Draw] = m_commandPool.createCommandBuffer(m_device, CommandBufferType::DRAW);
}

void Renderer::_createPipelines() {
    std::array<RenderPassInfo*, PIPELINE_COUNT> passInfos;
    SceneRenderPassInfo scenePassInfo(m_swapChain.getImageViews());
    scenePassInfo.m_formats.reserve(SceneRenderPass::RESOURCE_COUNT);
    scenePassInfo.m_formats[(u32)SceneResource::ColorImage] = m_swapChain.getImageFormat();
    scenePassInfo.m_formats[(u32)SceneResource::DepthImage] = m_swapChain.getDepthFormat();
    scenePassInfo.m_samples.reserve(SceneRenderPass::RESOURCE_COUNT);
    scenePassInfo.m_samples[(u32)SceneResource::ColorImage] = m_device.getMsaaCount();
    scenePassInfo.m_samples[(u32)SceneResource::DepthImage] = m_device.getMsaaCount();
    scenePassInfo.m_renderPassWidth = m_swapChain.getImageExtent().width;
    scenePassInfo.m_renderPassHeight = m_swapChain.getImageExtent().height;
    scenePassInfo.m_targetCount = m_swapChain.getImageViews().size();
    scenePassInfo.m_targetWidth = m_swapChain.getImageExtent().width;
    scenePassInfo.m_targetHeight = m_swapChain.getImageExtent().height;
    passInfos[(u32)PipelineIndex::ScenePipeline] = &scenePassInfo;

    for (u32 i = 0; i < PIPELINE_COUNT; ++i) m_pipelines[i]->init(m_device, passInfos[i]);

    _createPipelineLayout();

    for (u32 i = 0; i < PIPELINE_COUNT; ++i) m_pipelines[i]->assemble(m_device, m_pipelineLayout);
}

void Renderer::_createPipelineLayout() {
    std::array<VkDescriptorSetLayout, DESCRIPTOR_TABLE_SIZE> setLayouts;
    setLayouts[(u32)DescriptorSetIndex::Mvp] = m_descriptorTable[DescriptorSetIndex::Mvp]->getLayout();

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
    for (Fence& fence : m_fences) fence.init(m_device);
}

/* ========================================================================== */

void Renderer::_destroyCommandBuffers() {
    for (u32 i = 0; i < CMD_BUFFER_COUNT; ++i) m_commandPool.destroyBuffer(m_device, m_commandBuffers[i]);
}

void Renderer::_destroyPipelines() {
    _destroyPipelineLayout();

    for (u32 i = 0; i < PIPELINE_COUNT; i++) m_pipelines[i]->destroy(m_device);
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