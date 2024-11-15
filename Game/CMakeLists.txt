add_executable(Game main.cpp)

# Inclure glfw3.dll
#add_custom_command(TARGET Game POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    "${VCPKG_ROOT}/installed/x64-windows/bin/glfw3.dll"
#    $<TARGET_FILE_DIR:Game>
#)

# Inclure le dossier resources
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_custom_command(TARGET Game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${RESOURCE_DIR}"
	$<TARGET_FILE_DIR:Game>/resources
)

target_link_libraries(Game PRIVATE Engine)