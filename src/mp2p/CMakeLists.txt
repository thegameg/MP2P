file (GLOB libmp2p_src files/*.cc)
file (GLOB libmp2p_src misc/*.cc)
file (GLOB libmp2p_src network/*.cc)

add_library(mp2p ${libmp2p_src})
