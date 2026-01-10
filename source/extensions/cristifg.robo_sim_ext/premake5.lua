local ext = get_current_extension_info()
project_ext(ext)

-- Define the C++ Plugin
project_ext_plugin(ext, "cristifg.robo_sim_ext.plugin")
add_files("include", "plugins/cristifg.robo_sim_ext")
add_files("source", "plugins/cristifg.robo_sim_ext")
includedirs{"include", "plugins/cristifg.robo_sim_ext" }

targetdir(ext.bin_dir)
language("C++")

include_physx()
add_cuda_dependencies()
    -- Link against Kit/Isaac Sim libraries if needed
links { "carb" }
defines { "CARB_EXPORTS", "NOMINMAX" }

