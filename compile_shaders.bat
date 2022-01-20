for /r %%v in (shaders\*.vert) do C:\VulkanSDK\1.2.198.1\Bin\glslc "%%v" -o "%%v.spv"
for /r %%v in (shaders\*.frag) do C:\VulkanSDK\1.2.198.1\Bin\glslc "%%v" -o "%%v.spv"
