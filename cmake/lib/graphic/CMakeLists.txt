set(module_root "${lib_root}/graphic")
set(source_root "${module_root}/src/claw/graphic")

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

claw_library(
  TARGET claw_graphic
  MODULE_ROOT ${module_root}
  SOURCES
    "${source_root}/bitmap.cpp"
    "${source_root}/bitmap_reader.cpp"
    "${source_root}/bitmap_writer.cpp"
    "${source_root}/gif.cpp"
    "${source_root}/gif_frame.cpp"
    "${source_root}/gif_reader.cpp"
    "${source_root}/image.cpp"
    "${source_root}/jpeg.cpp"
    "${source_root}/jpeg_error_manager.cpp"
    "${source_root}/jpeg_reader.cpp"
    "${source_root}/jpeg_writer.cpp"
    "${source_root}/pcx.cpp"
    "${source_root}/pcx_reader.cpp"
    "${source_root}/pcx_writer.cpp"
    "${source_root}/pixel.cpp"
    "${source_root}/png.cpp"
    "${source_root}/png_reader.cpp"
    "${source_root}/png_writer.cpp"
    "${source_root}/targa.cpp"
    "${source_root}/targa_file_structure.cpp"
    "${source_root}/targa_reader.cpp"
    "${source_root}/targa_writer.cpp"
    "${source_root}/xbm.cpp"
    "${source_root}/xbm_reader.cpp"
    "${source_root}/xbm_writer.cpp"
  LINK
    claw_core
    PNG::PNG
    JPEG::JPEG
  )
