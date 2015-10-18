file(GLOB libmp2p_src_files files/*.cc)
file(GLOB libmp2p_src_misc misc/*.cc)
file(GLOB libmp2p_src_network network/*.cc)

set(libmp2p_src
  ${libmp2p_src_files}
  ${libmp2p_src_misc}
  ${libmp2p_src_network}
)

add_library(mp2p ${libmp2p_src})
target_link_libraries(mp2p
  ${Boost_LIBRARIES}
  Threads::Threads
  ${OPENSSL_LIBRARY}
  ${LIBCRYPTO_LIBRARY}
)
