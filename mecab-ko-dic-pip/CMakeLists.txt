include(GNUInstallDirs)

set(MECAB_KO_DIC_PIP_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/mecab-ko/dic/mecab-ko-dic")

find_package(Python COMPONENTS Interpreter REQUIRED)

set(MECAB_KO_DIC_PIP_PYPKG mecab-ko-dic==1.0.0)
set(MECAB_KO_DIC_PIP_TARGETDIR "${CMAKE_CURRENT_BINARY_DIR}/mecab-ko-dic-1.0.0")

set(MECAB_KO_DIC_PIP_DATA
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/matrix.bin"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/char.bin"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/sys.dic"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/unk.dic"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/left-id.def"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/rewrite.def"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/right-id.def"
  "${MECAB_KO_DIC_PIP_TARGETDIR}/mecab_ko_dic/dicdir/dicrc"
)

add_custom_target(
  mecab-ko-dic-pip ALL
  COMMAND ${Python_EXECUTABLE} -m pip install ${MECAB_KO_DIC_PIP_PYPKG} -t ${MECAB_KO_DIC_PIP_TARGETDIR} --upgrade
  BYPRODUCTS "${MECAB_KO_DIC_PIP_DATA}"
)

install(FILES
  ${MECAB_KO_DIC_PIP_DATA}
  DESTINATION "${MECAB_KO_DIC_PIP_DIR}"
  )

