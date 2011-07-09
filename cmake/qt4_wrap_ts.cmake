# create function to wrap qt *.ts files
# qt4_wrap_ts(qmfiles tsfile1 tsfile2 ... [SOURCES cppfile1 cppfile2 dir ...] [OPTIONS arguments...])
# outfiles receives .qm generated files from
# .ts files in arguments
# files_or_dirs - directory or path to source files
# arguments - additional arguments for lupdate
# a target lupdate is created for you
# update/generate your translations files
# example: QT4_WRAP_TS(foo_QM ${foo_TS} SOURCES . OPTIONS -recursive -extensions js,qs)
macro(qt4_wrap_ts outfiles)
if(QT_LUPDATE_EXECUTABLE)
set(CURRENT_VAR TS_FILES)
set(TS_FILES)
set(SOURCES)
set(LUPDATE_ARGS)
foreach(ARGUMENT ${ARGN})
if(ARGUMENT STREQUAL "SOURCES")
set(CURRENT_VAR SOURCES)
elseif(ARGUMENT STREQUAL "OPTIONS")
set(CURRENT_VAR LUPDATE_ARGS)
else(ARGUMENT)
list(APPEND ${CURRENT_VAR} ${ARGUMENT})
endif(ARGUMENT STREQUAL "SOURCES")
endforeach(ARGUMENT)

# если исходники для ts файлов не заданы
if(NOT SOURCES)
# устанавливаем текущую директорию в качестве исходной
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR})
# добавляем рекурсивный просмотр
list(APPEND LUPDATE_ARGS -recursive)
# удаляем повторяющиеся аргументы
list(REMOVE_DUPLICATES LUPDATE_ARGS)
endif(NOT SOURCES)

# добавляем цель для создания 
add_custom_target(lupdate
COMMAND ${QT_LUPDATE_EXECUTABLE}
${SOURCES}
${LUPDATE_ARGS}
-ts ${TS_FILES}
)

add_custom_command (OUTPUT ${TS_FILES}
COMMAND ${QT_LUPDATE_EXECUTABLE}
${SOURCES}
${LUPDATE_ARGS}
-ts ${TS_FILES}
COMMENT "Creating qt translations files"
)

if(QT_LRELEASE_EXECUTABLE)
foreach (it ${TS_FILES})
get_filename_component(outfile ${it} NAME_WE)
get_filename_component(outpath ${it} PATH)

set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}.qm)


add_custom_command(OUTPUT ${outfile}
COMMAND ${QT_LRELEASE_EXECUTABLE}
ARGS ${it} -qm ${outfile}
DEPENDS ${it}
COMMENT "Compilation qt translations files"
)
#list(APPEND ${qmfiles} ${outfile})
set(${outfiles} ${${outfiles}} ${outfile})
endforeach (it)
else(QT_LRELEASE_EXECUTABLE)
message(WARNING "lrelease not found. Command for compilation of translations files not created.")
endif(QT_LRELEASE_EXECUTABLE)
else(QT_LUPDATE_EXECUTABLE)
message(WARNING "lupdate not found. Translations files not created.")
endif(QT_LUPDATE_EXECUTABLE)
endmacro(qt4_wrap_ts)