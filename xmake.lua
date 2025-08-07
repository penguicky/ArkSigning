-- add rules: debug/release
add_rules("mode.debug", "mode.release")

-- add requires
add_requires("openssl", "zlib")

-- define target
target("arksigning")
    set_kind("binary")
    add_files("src/core/*.cpp", "src/crypto/*.cpp", "src/utils/*.cpp")
    add_includedirs("include", "include/arksigning")
    add_packages("openssl", "zlib")
    set_languages("c99", "c++11")
    set_warnings("all")
    add_cxflags("-Wall", "-Wextra")

--
-- ArkSigning XMake Build Configuration
-- Updated for modern project structure with organized source directories
--
-- Project Structure:
--   src/core/    - Main application logic and processing
--   src/crypto/  - Cryptographic operations (OpenSSL wrapper)
--   src/utils/   - Utility functions (base64, JSON, common helpers)
--   include/     - Header files and public API
--
-- FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd ArkSigning
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run arksigning
--   $ xmake run -d arksigning
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add dependencies
--    add_requires("openssl", "zlib")
--
--    -- add macro definitions
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("include", "include/arksigning")
--
--    -- add packages (modern way to link libraries)
--    add_packages("openssl", "zlib")
--
--    -- add system link libraries if needed
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-Wall", "-Wextra", "-std=c++11")
--    add_ldflags("-L/usr/local/lib", {force = true})
--
-- @endcode
--
-- 7. If you want to known more usage about xmake, please see https://xmake.io
--

