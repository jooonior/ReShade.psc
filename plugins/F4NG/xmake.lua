package("commonlibf4")
    set_urls("https://github.com/libxse/commonlibf4.git")

    -- set_kind("static")
    add_deps("spdlog", {
        public = true,
        configs = { header_only = false, wchar = true, std_format = true },
    })
    add_syslinks("advapi32", "bcrypt", "d3d11", "d3dcompiler", "dbghelp", "dxgi", "ole32", "shell32", "user32", "version", "ws2_32")

    on_install(function (package)
        import("package.tools.xmake").install(package)
    end)
package_end()
