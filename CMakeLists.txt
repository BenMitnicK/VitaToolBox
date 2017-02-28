cmake_minimum_required(VERSION 2.8)

# VitaSDK defines
if( NOT DEFINED CMAKE_TOOLCHAIN_FILE )
  if( DEFINED ENV{VITASDK} )
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

# Project start
project(VitaToolBox)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "VitaToolBox")
set(VITA_TITLEID  "VITATOOLS")
set(VITA_VERSION  "01.21")

# Flags and includes
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -Wno-unused-variable -Wno-unused-but-set-variable -fno-lto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2800000000000001")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Builds
FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(in_f ${ARGN})
    SET(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    GET_FILENAME_COMPONENT(out_dir ${out_f} DIRECTORY)
    ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
      COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Building resource ${out_f}"
      VERBATIM
      )
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

file(GLOB res_files RELATIVE ${CMAKE_SOURCE_DIR} resources/*.png resources/*.txt resources/*.bin resources/*.ini resources/*.skprx)
add_resources(vitatoolbox_res ${res_files})

add_executable(VitaToolBox
  ${vitatoolbox_res}
  src/main.c
  src/init.c
  src/io_process.c
  src/makezip.c
  src/package_installer.c
  src/network_update.c
  src/context_menu.c
  src/archive.c
  src/photo.c
  src/audioplayer.c
  src/file.c
  src/text.c
  src/hex.c
  src/sfo.c
  src/coredump.c
  src/settings.c
  src/property_dialog.c
  src/uncommon_dialog.c
  src/message_dialog.c
  src/ime_dialog.c
  src/config.c
  src/theme.c
  src/language.c
  src/utils.c
  src/elf.c
  src/sha1.c
  src/list_dialog.c
  src/minizip/zip.c
  src/minizip/unzip.c
  src/minizip/ioapi.c
  src/bm.c
  src/audio/vita_audio.c
  src/audio/player.c
  src/audio/id3.c
  src/audio/oggplayer.c
  src/audio/mp3player.c
  src/audio/mp3xing.c
  src/audio/lrcparse.c
  src/libmad/bit.c
  src/libmad/decoder.c
  src/libmad/fixed.c
  src/libmad/frame.c
  src/libmad/huffman.c
  src/libmad/layer12.c
  src/libmad/layer3.c
  src/libmad/stream.c
  src/libmad/synth.c
  src/libmad/timer.c
  src/unrarlib/unrarlibutils.c
  src/archiveRAR.c
  src/database.c
  src/plugins_manager.c
  src/pluginsmanager/configs.c
  src/pluginsmanager/plugins.c
  src/vitaident/app.c
  src/vitaident/fs.c
  src/vitaident/kernel.c
  src/vitaident/net.c
  src/vitaident/power.c
  src/vitaident/utils.c
  src/vitaident.c
)

target_link_libraries(VitaToolBox
  ftpvita
  vita2d
  vorbisfile
  vorbis
  ogg
  png
  jpeg
  z
  m
  c
  onigmo
  unrar
  stdc++
  taihen_stub
  HENkaku_stub
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceAudiodec_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceLibKernel_stub
  SceDisplay_stub
  SceGxm_stub
  SceIme_stub
  SceHttp_stub
  SceMtpIfDriver_stub
  SceMusicExport_stub
  SceNet_stub
  SceNetCtl_stub
  SceShellSvc_stub
  SceSsl_stub
  SceSysmodule_stub
  ScePgf_stub
  ScePhotoExport_stub
  ScePower_stub
  ScePromoterUtil_stub
  SceTouch_stub
  SceUdcd_stub
  SceUsbstorVStorDriver_stub
  SceVshBridge_stub
)

# Create Vita artifacts
vita_create_self(eboot.bin VitaToolBox UNSAFE)
vita_create_vpk(VitaToolBox.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE pkg/sce_sys/icon0.png sce_sys/icon0.png
       pkg/sce_sys/pic0.png sce_sys/pic0.png
       pkg/sce_sys/livearea/contents/bg0.png sce_sys/livearea/contents/bg0.png
       pkg/sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
       pkg/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
       pkg/sce_sys/livearea/contents/item_2.png sce_sys/livearea/contents/item_2.png
       pkg/sce_sys/livearea/contents/item_3.png sce_sys/livearea/contents/item_3.png
       pkg/sce_sys/livearea/contents/item_4.png sce_sys/livearea/contents/item_4.png
       pkg/sce_sys/livearea/contents/item_5.png sce_sys/livearea/contents/item_5.png
	   pkg/sce_sys/manual/001.png sce_sys/manual/001.png
	   pkg/sce_sys/manual/002.png sce_sys/manual/002.png
	   pkg/sce_sys/manual/003.png sce_sys/manual/003.png
	   pkg/sce_sys/manual/004.png sce_sys/manual/004.png
	   pkg/sce_sys/manual/005.png sce_sys/manual/005.png
	   pkg/sce_sys/manual/006.png sce_sys/manual/006.png
	   pkg/sce_sys/manual/007.png sce_sys/manual/007.png
	   pkg/sce_sys/manual/008.png sce_sys/manual/008.png
)

add_custom_target(send
	COMMAND curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/${VITA_TITLEID}/
	DEPENDS eboot.bin
)

add_custom_target(copy
	COMMAND cp eboot.bin F:/app/${VITA_TITLEID}/eboot.bin
	DEPENDS eboot.bin
)