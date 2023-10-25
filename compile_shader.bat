@echo off

SETLOCAL EnableDelayedExpansion
for /F %%a in ('echo prompt $E ^| cmd') do (
  set "ESC=%%a"
)

echo %ESC%[31m%ESC%[1mCompiling shader...%ESC%[0m

:: Compile GLSL shaders
C:\VulkanSDK\1.3.250.1\Bin\glslc.exe -fshader-stage=fragment shaders/glsl/scene.fragment.glsl -o shaders/scene_frag.spv
C:\VulkanSDK\1.3.250.1\Bin\glslc.exe -fshader-stage=vertex shaders/glsl/scene.vertex.glsl -o shaders/scene_vert.spv
C:\VulkanSDK\1.3.250.1\Bin\glslc.exe -fshader-stage=vertex shaders/glsl/shadow.vertex.glsl -o shaders/shadow_vert.spv

:: Compile HLSL shaders
:: C:\VulkanSDK\1.3.250.1\Bin\dxc.exe -spirv -T vs_6_0 -E main "shaders\hlsl\scene_vert.hlsl" -Fo "shaders\scene_vert.spv"
:: C:\VulkanSDK\1.3.250.1\Bin\dxc.exe -spirv -T ps_6_0 -E main "shaders\hlsl\scene_frag.hlsl" -Fo "shaders\scene_frag.spv"
:: C:\VulkanSDK\1.3.250.1\Bin\dxc.exe -spirv -T vs_6_0 -E main "shaders\hlsl\shadow_vert.hlsl" -Fo "shaders\shadow_vert.spv"

if %errorlevel%==0 (
	echo %ESC%[32m%ESC%[1mShaders compiled.%ESC%[0m
    exit /b 0
) else (
	echo %ESC%[31m%ESC%[1mShaders failed to compile.%ESC%[0m
    exit /b 1
)