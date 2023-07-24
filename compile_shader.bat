:: Compile GLSL shaders
:: C:\VulkanSDK\1.3.250.1\Bin\glslc.exe shaders/shader.frag -o shaders/frag.spv
:: C:\VulkanSDK\1.3.250.1\Bin\glslc.exe shaders/shader.vert -o shaders/vert.spv

:: Compile HLSL shaders
echo "Compiling shader..."
C:\VulkanSDK\1.3.250.1\Bin\dxc.exe -spirv -T vs_6_0 -E main "shaders\vertex.hlsl" -Fo "shaders\vert.spv"
C:\VulkanSDK\1.3.250.1\Bin\dxc.exe -spirv -T ps_6_0 -E main "shaders\fragment.hlsl" -Fo "shaders\frag.spv"