include(GNUInstallDirs)

set(MECAB_KO_DIC_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/mecab-ko/dic/mecab-ko-dic")

find_package(Python COMPONENTS Interpreter REQUIRED)

set(VERSION 2.1.1-20180720)
set(MECAB_KO_DIC_PKG https://bitbucket.org/eunjeon/mecab-ko-dic/downloads/mecab-ko-dic-${VERSION}.tar.gz)
set(MECAB_KO_DIC_TGTTAR "${CMAKE_CURRENT_BINARY_DIR}/mecab-ko-dic-${VERSION}.tar.gz")
set(MECAB_KO_DIC_TGTDIR "${CMAKE_CURRENT_BINARY_DIR}/mecab-ko-dic-${VERSION}")

if (EXISTS "${MECAB_KO_DIC_TGTTAR}")
  message(STATUS "${MECAB_KO_DIC_PKG} already downloaded")
else()
  message(STATUS "Downloading ${MECAB_KO_DIC_PKG}")
  file(DOWNLOAD ${MECAB_KO_DIC_PKG} ${MECAB_KO_DIC_TGTTAR})
endif()

file(ARCHIVE_EXTRACT
  INPUT ${MECAB_KO_DIC_TGTTAR}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  VERBOSE
  )

set(MECAB_KO_DICT_INDEX "${CMAKE_CURRENT_BINARY_DIR}/../mecab-ko-dict-index")
set(MECAB_KO_DIC_CHARSET "utf-8")  # (euc-jp/sjis/utf-8)

set(MECAB_KO_DIC_BYPRODUCTS
  "${MECAB_KO_DIC_TGTDIR}/char.bin"
  "${MECAB_KO_DIC_TGTDIR}/matrix.bin"
  "${MECAB_KO_DIC_TGTDIR}/model.bin"
  "${MECAB_KO_DIC_TGTDIR}/sys.dic"
  "${MECAB_KO_DIC_TGTDIR}/unk.dic"
  )

add_custom_target(
  mecab-ko-dic ALL
  COMMAND ${MECAB_KO_DICT_INDEX}${CMAKE_EXECUTABLE_SUFFIX} -d ${MECAB_KO_DIC_TGTDIR} -o ${MECAB_KO_DIC_TGTDIR} -f utf-8 -t ${MECAB_KO_DIC_CHARSET}
  DEPENDS ${MECAB_KO_DICT_INDEX} ${MECAB_KO_DIC_TGTDIR}
  BYPRODUCTS ${MECAB_KO_DIC_BYPRODUCTS}
  )

set(MECAB_KO_DIC_DATA
  ${MECAB_KO_DIC_BYPRODUCTS}
  "${MECAB_KO_DIC_TGTDIR}/char.def"
  "${MECAB_KO_DIC_TGTDIR}/feature.def"
  "${MECAB_KO_DIC_TGTDIR}/left-id.def"
  "${MECAB_KO_DIC_TGTDIR}/matrix.def"
  "${MECAB_KO_DIC_TGTDIR}/model.def"
  "${MECAB_KO_DIC_TGTDIR}/pos-id.def"
  "${MECAB_KO_DIC_TGTDIR}/rewrite.def"
  "${MECAB_KO_DIC_TGTDIR}/right-id.def"
  "${MECAB_KO_DIC_TGTDIR}/unk.def"
  "${MECAB_KO_DIC_TGTDIR}/dicrc"
  )

install(FILES
  ${MECAB_KO_DIC_DATA}
  DESTINATION "${MECAB_KO_DIC_DIR}"
  )
