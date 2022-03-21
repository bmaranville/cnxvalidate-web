LIBXML_CMAKE_DIR="libxml2"
LIBXML_BUILD_DIR="libxml2_build"
LIBXML_INSTALL_DIR="libxml2/dist"
libxml = $(LIBXML_INSTALL_DIR)/lib/libxml2.a

BUILD_DIR="build"
DIST_DIR="dist"

libxml:
	mkdir -p $(LIBXML_BUILD_DIR);
	cd $(LIBXML_BUILD_DIR) && emcmake cmake ../$(LIBXML_CMAKE_DIR);
	cd $(LIBXML_BUILD_DIR) && emmake make install;

cnxvalidate: libxml
	mkdir -p $(BUILD_DIR);
	cp -r application_defs $(BUILD_DIR);
	cd $(BUILD_DIR) && emcmake cmake ../;
	cd $(BUILD_DIR) && emmake make;

all: cnxvalidate

clean:
	rm -rf $(LIBXML_BUILD_DIR);
	rm -rf $(LIBXML_INSTALL_DIR);
	rm -rf $(BUILD_DIR);
	rm -rf $(DIST_DIR);