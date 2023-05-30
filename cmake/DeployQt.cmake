# Deploys Qt libraries besides the target executable.
function(target_deploy_qt TARGET_NAME)
  if(WIN32)
    set(DEPLOYQT_NAME "windeployqt")
  elseif(APPLE)
    set(DEPLOYQT_NAME "macdeployqt")
  else()
    message(WARNING "Deployement of dependecies not implemented yet for this platform.")
    return()
  endif()

  if (DEPLOYQT_NAME)
    get_target_property(QMAKE_LOCATION Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BINARY_DIR ${QMAKE_LOCATION} DIRECTORY)
    find_program(DEPLOYQT_EXE "${DEPLOYQT_NAME}" HINTS "${QT_BINARY_DIR}" REQUIRED)
    set(QTDEPLOY_TARGET_NAME Qt5::deploy)
    add_executable(${QTDEPLOY_TARGET_NAME} IMPORTED)
    set_property(TARGET ${QTDEPLOY_TARGET_NAME} PROPERTY IMPORTED_LOCATION ${DEPLOYQT_EXE})

    # Deploy Qt dependencies to help launch individual instances.
    if(TARGET ${QTDEPLOY_TARGET_NAME})
      if(WIN32)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E echo "Deploying Qt..."
          COMMAND ${QTDEPLOY_TARGET_NAME} --verbose 0 --no-patchqt --no-compiler-runtime --no-webkit2 --no-system-d3d-compiler --no-translations --no-angle --no-opengl-sw --dir "$<TARGET_FILE_DIR:${TARGET_NAME}>" "$<TARGET_FILE:${TARGET_NAME}>"
        )
      elseif(APPLE)
       add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E echo "Deploying Qt..."
          COMMAND ${QTDEPLOY_TARGET_NAME} "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>" -verbose=0
        )
      endif()
    endif()
  endif()
endfunction()
