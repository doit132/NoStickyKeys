-- 设置项目信息
set_project("MyCppProject")
set_version("1.0.0")

-- 设置C++标准
set_languages("c++17")

-- 添加自动更新compile_commands.json的规则
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode/"})

-- 设置编译器
set_toolchains("gcc")

-- 添加包含目录
add_includedirs("include")

-- 定义目标
target("NoStickyKeys")
    set_kind("binary")
    add_files("src/*.cpp")
    -- 添加库目录
    add_linkdirs("lib")
-- 链接DLL
    add_links("interception")
