cmake_minimum_required(VERSION 2.8)
project(SendaMvlink)

set(src_dir ${SRC_ROOT})
set(module_name SendaMvlink)

set(src_list
	${src_dir}/main.c
	${src_dir}/mavlink_handle.c
	${src_dir}/udp.c
)
set(SENDMAV_BINARY_DIR )

include_directories(
	${src_dir}/include/
	${src_dir}/include/mavlink/include/mavlink/v2.0/common/
)
add_executable(${module_name}
	${src_list}
)
target_link_libraries(${module_name}
	-lpthread
)
