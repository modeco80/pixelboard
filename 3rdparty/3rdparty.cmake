# Third party submodules
# we build as a part of the PixelBoard board server

add_subdirectory(3rdparty/fmt)

# We provide the Fmt library ourselves,
# so we don't wanna use the spdlog built-in version.
# why? well I don't wanna duplicate code :V
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(3rdparty/spdlog)