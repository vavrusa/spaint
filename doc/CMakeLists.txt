# Files
set(tex_src documentation.tex)

# pdflatex
find_package(LATEX)
if(EXISTS ${PDFLATEX_COMPILER})
  message(STATUS "PDF compiler found, pdflatex.")
  message(STATUS "PDF compiler output: ${CMAKE_BINARY_DIR}")
  add_custom_target(doc
                    "${PDFLATEX_COMPILER}" -output-directory "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${tex_src}"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    COMMENT "Building documentation ..."
                    SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${tex_src}"
                    )
endif(EXISTS ${PDFLATEX_COMPILER})