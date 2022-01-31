for /r %%f in (shaders\*.vert shaders\*.tesc shaders\*.tese shaders\*.geom shaders\*.frag shaders\*.comp) do C:\VulkanSDK\1.2.198.1\Bin\glslc.exe -o %%f.spv %%f

