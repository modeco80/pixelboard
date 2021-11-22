add_library(pixelboard_ws
		# These files go first because they *will* take the longest time to compile.
		# And it's best to get them done and over with.
		AsioImplementation.cpp
		BeastImplementation.cpp

		Server.cpp
		Listener.cpp
		NetworkingTSCompatibility.h
		HttpSession.cpp
		HttpUtils.h
		${PROJECT_SOURCE_DIR}/include/websocket/Message.h

)

target_include_directories(pixelboard_ws PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pixelboard_ws PRIVATE fmt::fmt spdlog::spdlog Boost::system)



# Tell beast some stuff
target_compile_definitions(pixelboard_ws PUBLIC
		# No more io_service! No more io_context::work!
		-DBOOST_ASIO_NO_DEPRECATED=1

		-DBOOST_ASIO_DISABLE_BOOST_ARRAY=1
		-DBOOST_ASIO_DISABLE_BOOST_BIND=1
		-DBOOST_ASIO_DISABLE_BOOST_DATE_TIME=1
		-DBOOST_ASIO_DISABLE_BOOST_REGEX=1
		-DBOOST_ASIO_SEPARATE_COMPILATION=1

		# By default, Boost.Beast uses boost::string_view,
		# which has no conversion to the standard library std::string_view type.
		# This is annoying, but thankfully there's an option to make it use
		# the standard library type.
		#
		# Thanks Vinnie, even though you're a chud,
		# and I only use your library because Websocket++ is miles worse (well that, and ASIO is decent).
		-DBOOST_BEAST_USE_STD_STRING_VIEW
		-DBOOST_BEAST_SEPARATE_COMPILATION=1
)

add_library(pixelboard::websocket ALIAS pixelboard_ws)