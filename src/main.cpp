/**
 * @file main.cpp
 * @brief Application simple avec Vulkan et GLFW avec fond noir
 */
#pragma once

#include <vulkan/vulkan.h>
#include "../include/VkBootstrap.h"
#include "../include/VkBootstrapDispatch.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <thread>
#include <iostream>

namespace vkutil {

	void transition_image(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
}

#define VK_CHECK(x) \
    do { \
        VkResult err = x; \
        if (err) { \
            std::cout << "Detected Vulkan error: " << err << std::endl; \
            abort(); \
        } \
    } while (0)

struct FrameData {
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	VkFence _renderFence;
};

constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine
{
public:
	GLFWwindow* window;
	VkSurfaceKHR _surface;// Vulkan window surface

	vkb::Instance _vkb_inst;
	vkb::PhysicalDevice _vkb_physicalDevice;
	vkb::Device _vkb_device;

	vkb::Swapchain _vkb_swapchain;
	VkFormat _swapchainImageFormat;
	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	bool _isInitialized{ false };
	int _frameNumber{ 0 };
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };

	FrameData _frames[FRAME_OVERLAP];
	FrameData& GetCurrentFrame() { return _frames[_frameNumber % FRAME_OVERLAP]; }

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	static VulkanEngine& Get();

	//initializes everything in the engine
	void Init();

	//shuts down the engine
	void Cleanup();

	//draw loop
	void Draw();

	//run main loop
	void Run();

	VkCommandPoolCreateInfo CommandPoolCreateInfo(uint32_t queueFamilyIndex,
		VkCommandPoolCreateFlags flags /*= 0*/);

	VkCommandBufferAllocateInfo CommandBufferAllocateInfo(
		VkCommandPool pool, uint32_t count /*= 1*/);

	VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags /*= 0*/);

	VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags /*= 0*/);

	VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags /*= 0*/);

	VkImageSubresourceRange static ImageSubresourceRange(VkImageAspectFlags aspectMask);

	VkSemaphoreSubmitInfo SemaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);

	VkCommandBufferSubmitInfo CommandBufferSubmitInfo(VkCommandBuffer cmd);

	VkSubmitInfo2 SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
		VkSemaphoreSubmitInfo* waitSemaphoreInfo);

private:

	void InitVulkan();
	void InitCommand();
	void InitSwapchain();
	void InitSyncStructures();
	void CreateSwapchain(uint32_t width, uint32_t height);
	void DestroySwapchain();
};

constexpr bool bUseValidationLayers = false;

VulkanEngine* loadedEngine = nullptr;

VulkanEngine& VulkanEngine::Get() { return *loadedEngine; }

VkCommandBufferSubmitInfo VulkanEngine::CommandBufferSubmitInfo(VkCommandBuffer cmd)
{
	VkCommandBufferSubmitInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	info.pNext = nullptr;
	info.commandBuffer = cmd;
	info.deviceMask = 0;

	return info;
}

VkSubmitInfo2 VulkanEngine::SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
	VkSemaphoreSubmitInfo* waitSemaphoreInfo)
{
	VkSubmitInfo2 info = {};
	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
	info.pNext = nullptr;

	info.waitSemaphoreInfoCount = waitSemaphoreInfo == nullptr ? 0 : 1;
	info.pWaitSemaphoreInfos = waitSemaphoreInfo;

	info.signalSemaphoreInfoCount = signalSemaphoreInfo == nullptr ? 0 : 1;
	info.pSignalSemaphoreInfos = signalSemaphoreInfo;

	info.commandBufferInfoCount = 1;
	info.pCommandBufferInfos = cmd;

	return info;
}

VkSemaphoreSubmitInfo VulkanEngine::SemaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore)
{
	VkSemaphoreSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.semaphore = semaphore;
	submitInfo.stageMask = stageMask;
	submitInfo.deviceIndex = 0;
	submitInfo.value = 1;

	return submitInfo;
}

VkCommandPoolCreateInfo VulkanEngine::CommandPoolCreateInfo(uint32_t queueFamilyIndex,
	VkCommandPoolCreateFlags flags /*= 0*/)
{
	VkCommandPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.queueFamilyIndex = queueFamilyIndex;
	info.flags = flags;
	return info;
}

VkImageSubresourceRange VulkanEngine::ImageSubresourceRange(VkImageAspectFlags aspectMask)
{
	VkImageSubresourceRange subImage{};
	subImage.aspectMask = aspectMask;
	subImage.baseMipLevel = 0;
	subImage.levelCount = VK_REMAINING_MIP_LEVELS;
	subImage.baseArrayLayer = 0;
	subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

	return subImage;
}

void vkutil::transition_image(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout)
{
	VkImageMemoryBarrier2 imageBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
	imageBarrier.pNext = nullptr;

	imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
	imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

	imageBarrier.oldLayout = currentLayout;
	imageBarrier.newLayout = newLayout;

	VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	imageBarrier.subresourceRange = VulkanEngine::ImageSubresourceRange(aspectMask);
	imageBarrier.image = image;

	VkDependencyInfo depInfo{};
	depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
	depInfo.pNext = nullptr;

	depInfo.imageMemoryBarrierCount = 1;
	depInfo.pImageMemoryBarriers = &imageBarrier;

	vkCmdPipelineBarrier2(cmd, &depInfo);
}

VkCommandBufferAllocateInfo VulkanEngine::CommandBufferAllocateInfo(
	VkCommandPool pool, uint32_t count /*= 1*/)
{
	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;

	info.commandPool = pool;
	info.commandBufferCount = count;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	return info;
}

VkFenceCreateInfo VulkanEngine::FenceCreateInfo(VkFenceCreateFlags flags /*= 0*/)
{
	VkFenceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;

	info.flags = flags;

	return info;
}

VkSemaphoreCreateInfo VulkanEngine::SemaphoreCreateInfo(VkSemaphoreCreateFlags flags /*= 0*/)
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	return info;
}

VkCommandBufferBeginInfo VulkanEngine::CommandBufferBeginInfo(VkCommandBufferUsageFlags flags /*= 0*/)
{
	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;

	info.pInheritanceInfo = nullptr;
	info.flags = flags;
	return info;
}


void VulkanEngine::Init()
{
	assert(loadedEngine == nullptr);
	loadedEngine = this;

	InitVulkan();
	InitSwapchain();
	InitCommand();
	InitSyncStructures();

	// everything went fine
	_isInitialized = true;
}

void VulkanEngine::Cleanup()
{
	if (_isInitialized) {

		vkDeviceWaitIdle(_vkb_device);

		for (int i = 0; i < FRAME_OVERLAP; i++) {
			vkDestroyCommandPool(_vkb_device, _frames[i]._commandPool, nullptr);
		}

		vkDestroyInstance(_vkb_inst, nullptr);

		DestroySwapchain();

		vkDestroySurfaceKHR(_vkb_inst, _surface, nullptr);
		vkDestroyDevice(_vkb_device, nullptr);

		vkb::destroy_device(_vkb_device);
		vkb::destroy_surface(_vkb_inst, _surface);
		vkb::destroy_instance(_vkb_inst);
		glfwDestroyWindow(window);
		glfwTerminate();

		for (int i = 0; i < FRAME_OVERLAP; i++) {

			//already written from before
			vkDestroyCommandPool(_vkb_device, _frames[i]._commandPool, nullptr);

			//destroy sync objects
			vkDestroyFence(_vkb_device, _frames[i]._renderFence, nullptr);
			vkDestroySemaphore(_vkb_device, _frames[i]._renderSemaphore, nullptr);
			vkDestroySemaphore(_vkb_device, _frames[i]._swapchainSemaphore, nullptr);
		}
	}

	// clear engine pointer
	loadedEngine = nullptr;
}

void VulkanEngine::Draw()
{
	VK_CHECK(vkWaitForFences(_vkb_device, 1, &GetCurrentFrame()._renderFence, true, UINT64_MAX));
	VK_CHECK(vkResetFences(_vkb_device, 1, &GetCurrentFrame()._renderFence));

	uint32_t swapchainImageIndex;
	VK_CHECK(vkAcquireNextImageKHR(_vkb_device, _vkb_swapchain, UINT64_MAX, GetCurrentFrame()._swapchainSemaphore, VK_NULL_HANDLE, &swapchainImageIndex));

	//naming it cmd for shorter writing
	VkCommandBuffer cmd = GetCurrentFrame()._mainCommandBuffer;

	// now that we are sure that the commands finished executing, we can safely
	// reset the command buffer to begin recording again.
	VK_CHECK(vkResetCommandBuffer(cmd, 0));

	//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
	VkCommandBufferBeginInfo cmdBeginInfo = CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	//start the command buffer recording
	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

	//make the swapchain image into writeable mode before rendering
	vkutil::transition_image(cmd, _swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

	//make a clear-color from frame number. This will flash with a 120 frame period.
	VkClearColorValue clearValue;
	float flash = std::abs(std::sin(_frameNumber / 120.f));
	clearValue = { { 0.0f, 0.0f, flash, 1.0f } };

	VkImageSubresourceRange clearRange = ImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);
	vkCmdClearColorImage(cmd, _swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);

	//make the swapchain image into presentable mode
	vkutil::transition_image(cmd, _swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	//finalize the command buffer (we can no longer add commands, but it can now be executed)
	VK_CHECK(vkEndCommandBuffer(cmd));

	VkCommandBufferSubmitInfo cmdinfo = CommandBufferSubmitInfo(cmd);

	VkSemaphoreSubmitInfo waitInfo   = SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, GetCurrentFrame()._swapchainSemaphore);
	VkSemaphoreSubmitInfo signalInfo = SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, GetCurrentFrame()._renderSemaphore);

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	VkSubmitInfo2 submit = SubmitInfo(&cmdinfo, &signalInfo, &waitInfo);

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	VK_CHECK(vkQueueSubmit2(_graphicsQueue, 1, &submit, GetCurrentFrame()._renderFence));

	VkPresentInfoKHR presentInfo = { .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.pSwapchains = &_vkb_swapchain.swapchain;
	presentInfo.swapchainCount = 1;
	presentInfo.pWaitSemaphores = &GetCurrentFrame()._renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pImageIndices = &swapchainImageIndex;

	VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

	//increase the number of frames drawn
	_frameNumber++;
}

void VulkanEngine::Run()
{
	while (true)
	{
		Draw();
	}
}

void VulkanEngine::InitVulkan()
{
	vkb::InstanceBuilder builder;
	auto inst_ret =
		builder
			.set_app_name("Example Vulkan Application")
			.require_api_version(1, 3, 0)
			.request_validation_layers()
			.use_default_debug_messenger()
			.build();
	if (!inst_ret) {
		std::cerr << "Failed to create Vulkan instance. Error: " << inst_ret.error().message() << "\n";
		return;
	}
	_vkb_inst = inst_ret.value();

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(1024, 1024, "Vulkan Triangle", NULL, NULL);

	_surface = VK_NULL_HANDLE;
	VkResult glfw_result = glfwCreateWindowSurface(_vkb_inst, window, nullptr, &_surface);
	if (glfw_result != VK_SUCCESS) {
		std::cerr << "Failed to select create window surface. Error: " << std::to_string(glfw_result) << "\n";
		return;
	}

	VkPhysicalDeviceVulkan13Features features13{};
	features13.synchronization2 = true;

	vkb::PhysicalDeviceSelector selector{ _vkb_inst };
	auto phys_ret = selector
			.set_surface(_surface)
			.set_minimum_version(1, 3)
			.set_required_features_13(features13)
			.select();
	if (!phys_ret) {
		std::cerr << "Failed to select Vulkan Physical Device. Error: " << phys_ret.error().message() << "\n";
		return;
	}
	_vkb_physicalDevice = phys_ret.value();

	vkb::DeviceBuilder device_builder{ _vkb_physicalDevice };
	// automatically propagate needed data from instance & physical device
	auto dev_ret = device_builder.build();
	if (!dev_ret) {
		std::cerr << "Failed to create Vulkan device. Error: " << dev_ret.error().message() << "\n";
		return;
	}
	_vkb_device = dev_ret.value();

	// Get the graphics queue with a helper function
	_graphicsQueue = _vkb_device.get_queue(vkb::QueueType::graphics).value();
	_graphicsQueueFamily = _vkb_device.get_queue_index(vkb::QueueType::graphics).value();
}

void VulkanEngine::InitCommand()
{
	VkCommandPoolCreateInfo commandPoolInfo = CommandPoolCreateInfo(_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	for (int i = 0; i < FRAME_OVERLAP; i++) {

		VK_CHECK(vkCreateCommandPool(_vkb_device, &commandPoolInfo, nullptr, &_frames[i]._commandPool));

		// allocate the default command buffer that we will use for rendering
		VkCommandBufferAllocateInfo cmdAllocInfo = CommandBufferAllocateInfo(_frames[i]._commandPool, 1);

		VK_CHECK(vkAllocateCommandBuffers(_vkb_device, &cmdAllocInfo, &_frames[i]._mainCommandBuffer));
	}
}

void VulkanEngine::InitSwapchain()
{
	CreateSwapchain(_windowExtent.width, _windowExtent.height);
}

void VulkanEngine::InitSyncStructures()
{
	VkFenceCreateInfo fenceCreateInfo = FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
	VkSemaphoreCreateInfo semaphoreCreateInfo = SemaphoreCreateInfo(0);

	for (int i = 0; i < FRAME_OVERLAP; i++) {
		VK_CHECK(vkCreateFence(_vkb_device, &fenceCreateInfo, nullptr, &_frames[i]._renderFence));

		VK_CHECK(vkCreateSemaphore(_vkb_device, &semaphoreCreateInfo, nullptr, &_frames[i]._swapchainSemaphore));
		VK_CHECK(vkCreateSemaphore(_vkb_device, &semaphoreCreateInfo, nullptr, &_frames[i]._renderSemaphore));
	}
}

void VulkanEngine::CreateSwapchain(uint32_t width, uint32_t height)
{
	vkb::SwapchainBuilder swapchainBuilder{ _vkb_physicalDevice, _vkb_device, _surface };

	_swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	auto vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = _swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build();

	_vkb_swapchain = vkbSwapchain.value();
	_swapchainExtent = _vkb_swapchain.extent;
	_swapchainImages = _vkb_swapchain.get_images().value();
	_swapchainImageViews = _vkb_swapchain.get_image_views().value();
}

void VulkanEngine::DestroySwapchain()
{
	vkDestroySwapchainKHR(_vkb_device, _vkb_swapchain, nullptr);
	vkb::destroy_swapchain(_vkb_swapchain);

	for (auto& imageView : _swapchainImageViews)
	{
		vkDestroyImageView(_vkb_device, imageView, nullptr);
	}
}

int main(int argc, char* argv[])
{

	VulkanEngine engine;

	engine.Init();

	engine.Run();

	engine.Cleanup();

	return 0;
}