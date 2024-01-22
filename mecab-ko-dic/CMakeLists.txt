include(GNUInstallDirs)

set(MECAB_DICDIR "${CMAKE_INSTALL_FULL_LIBDIR}/mecab-ko/dic/mecab-ko-dic")

find_package(Python COMPONENTS Interpreter REQUIRED)

set(VERSION 2.1.1-20180720)
set(PKG https://bitbucket.org/eunjeon/mecab-ko-dic/downloads/mecab-ko-dic-${VERSION}.tar.gz)
set(TGTTAR "${CMAKE_CURRENT_BINARY_DIR}/mecab-ko-dic-${VERSION}.tar.gz")
set(TGTDIR "${CMAKE_CURRENT_BINARY_DIR}/mecab-ko-dic-${VERSION}")

# message(STATUS "PKG: \"${PKG}\"")
# message(STATUS "TGTTAR: \"${TGTTAR}\"")
# message(STATUS "TGTDIR: \"${TGTDIR}\"")

if (EXISTS "${TGTTAR}")
  message(STATUS "${PKG} already downloaded")
else()
  message(STATUS "Downloading ${PKG}")
  file(DOWNLOAD ${PKG} ${TGTTAR})
endif()

file(ARCHIVE_EXTRACT
  INPUT ${TGTTAR}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  VERBOSE
  )

set(MECAB_DICT_INDEX "${CMAKE_CURRENT_BINARY_DIR}/../mecab-ko-dict-index")
set(CHARSET "utf-8")  # (euc-jp/sjis/utf-8)

# message(STATUS "MECAB_DICT_INDEX: \"${MECAB_DICT_INDEX}\"")
# message(STATUS "CHARSET: \"${CHARSET}\"")
# message(STATUS "MECAB_DICDIR: \"${MECAB_DICDIR}\"")

set(BYPRODUCTS
  "${TGTDIR}/char.bin"
  "${TGTDIR}/matrix.bin"
  "${TGTDIR}/model.bin"
  "${TGTDIR}/sys.dic"
  "${TGTDIR}/unk.dic"
  )

add_custom_target(
  mecab-ko-dic ALL
  COMMAND ${MECAB_DICT_INDEX} -d ${TGTDIR} -o ${TGTDIR} -f utf-8 -t ${CHARSET}
  DEPENDS ${MECAB_DICT_INDEX} ${TGTDIR}
  BYPRODUCTS ${BYPRODUCTS}
  )

file(GLOB DIC_DATA
  ${BYPRODUCTS}
  "${TGTDIR}/*.def"
  "${TGTDIR}/dicrc"
  )

# message(STATUS "DIC_DATA: \"${DIC_DATA}\"")

install(FILES
  ${DIC_DATA}
  DESTINATION "${MECAB_DICDIR}"
  )
