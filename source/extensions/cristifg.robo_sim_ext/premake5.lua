local ext = get_current_extension_info()

local targetDepsDir = "%{root}/_build/target-deps"

project_ext(ext)

-- Define the C++ Plugin
project_ext_plugin(ext, "cristifg.robo_sim_ext.plugin")

dependson { "prebuild", "omni.physx.plugin", "carb.physics-usd.plugin" }
add_files("impl", "plugins")
add_files("include", "plugins/cristifg.robo_sim_ext")
add_files("source", "plugins/cristifg.robo_sim_ext")
includedirs{
    "%{root}/source/extensions/isaacsim.core.includes/include",
    extsbuild_dir .. "/usdrt.scenegraph/include",
    target_deps .. "/omni_physics/%{config}/include",
    "include",
    "plugins/cristifg.robo_sim_ext",
    "%{root}/source/extensions/isaacsim.sensors.physics/include",
    "%{root}/_build/target-deps/omni-isaacsim-schema/%{platform}/%{config}/include",
    target_deps .. "/usd/%{cfg.buildcfg}/include",
    target_deps .. "/python/include",
    target_deps .. "/usd_ext_physics/%{cfg.buildcfg}/include",
    target_deps .. "/omni_client_library/include",
    target_deps .. "/python/include",
    "%{root}/source/extensions/isaacsim.core.nodes/include",
    "%{kit_sdk_bin_dir}/dev/fabric/include"
}

targetdir(ext.bin_dir)
language("C++")
cppdialect("C++20")

include_physx()
add_cuda_dependencies()

libdirs {
    target_deps .. "/python/lib",
    target_deps .. "/usd/%{cfg.buildcfg}/lib",
    target_deps .. "/usd_ext_physics/%{cfg.buildcfg}/lib",
    "%{root}/_build/target-deps/omni-isaacsim-schema/%{platform}/%{config}/lib",
    extsbuild_dir .. "omni.usd.core/bin"
}

-- Link against Kit/Isaac Sim libraries if needed
links { "carb", "physxSchema", "isaacSensorSchema" }

extra_usd_libs = { "usdGeom", "usdPhysics", "usdUtils" }

add_usd(extra_usd_libs)

filter { "system:linux" }
includedirs {
    target_deps .. "/usd/%{cfg.buildcfg}/include/boost",
    target_deps .. "/python/include/python3.11"
}
filter {}

filter { "configurations:debug" }
defines { "_DEBUG" }
filter { "configurations:release" }
defines { "NDEBUG" }
filter {}




defines { "CARB_EXPORTS", "NOMINMAX" }



