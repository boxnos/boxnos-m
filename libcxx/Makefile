BASEDIR=$(PWD)
PREFIX=$(PWD)/lib
COMMON_CFLAGS=-nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS
CC=clang
CXX=clang++
TARGET_TRIPLE=x86_64-elf

make-all: build_libcxx

newlib-cygwin:
	git clone --depth 1 --branch fix-build https://github.com/uchan-nos/newlib-cygwin.git

llvm-project:
	git clone --depth 1 --branch llvmorg-9.0.0 https://github.com/llvm/llvm-project.git

.ONESHELL:
build_newlib: newlib-cygwin
	mkdir $@
	cd $@
	../newlib-cygwin/newlib/configure CC=$(CC) CC_FOR_BUILD=$(CC) \
		CFLAGS="-fPIC $(COMMON_CFLAGS)" \
		-target=$(TARGET_TRIPLE) --prefix=$(PREFIX) --disable-multilib --disable-newlib-multithread
	make -j 4
	make install

.ONESHELL:
build_libcxxabi: build_newlib llvm-project
	mkdir $@
	cd $@
	cmake -G "Unix Makefiles" \
	  -DCMAKE_INSTALL_PREFIX=$(PREFIX) \
	  -DCMAKE_CXX_COMPILER=$(CXX) \
	  -DCMAKE_CXX_FLAGS="-I$(PREFIX)/include $(COMMON_CFLAGS) -D_LIBCPP_HAS_NO_THREADS" \
	  -DCMAKE_C_COMPILER=$(CC) \
	  -DCMAKE_C_FLAGS="-I$(PREFIX)/include $(COMMON_CFLAGS)" \
	  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	  -DCMAKE_BUILD_TYPE=Release \
	  -DLIBCXXABI_LIBCXX_INCLUDES="$(BASEDIR)/llvm-project/libcxx/include" \
	  -DLIBCXXABI_ENABLE_EXCEPTIONS=False \
	  -DLIBCXXABI_ENABLE_THREADS=False \
	  -DLIBCXXABI_TARGET_TRIPLE=$(TARGET_TRIPLE) \
	  -DLIBCXXABI_ENABLE_SHARED=False \
	  -DLIBCXXABI_ENABLE_STATIC=True \
	  $(BASEDIR)/llvm-project/libcxxabi
	make -j4
	make install

.ONESHELL:
build_libcxx: build_libcxxabi
	mkdir $@
	cd $@
	cmake -G "Unix Makefiles" \
	  -DCMAKE_INSTALL_PREFIX=$(PREFIX) \
	  -DCMAKE_CXX_COMPILER=$(CXX) \
	  -DCMAKE_CXX_FLAGS="-I$(PREFIX)/include $(COMMON_CFLAGS)" \
	  -DCMAKE_CXX_COMPILER_TARGET=$(TARGET_TRIPLE) \
	  -DCMAKE_C_COMPILER=$(CC) \
	  -DCMAKE_C_FLAGS="-I$(PREFIX)/include $(COMMON_CFLAGS)" \
	  -DCMAKE_C_COMPILER_TARGET=$(TARGET_TRIPLE) \
	  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	  -DCMAKE_BUILD_TYPE=Release \
	  -DLIBCXX_CXX_ABI=libcxxabi \
	  -DLIBCXX_CXX_ABI_INCLUDE_PATHS="$(BASEDIR)/llvm-project/libcxxabi/include" \
	  -DLIBCXX_CXX_ABI_LIBRARY_PATH="$(PREFIX)/lib" \
	  -DLIBCXX_ENABLE_EXCEPTIONS=False \
	  -DLIBCXX_ENABLE_FILESYSTEM=False \
	  -DLIBCXX_ENABLE_MONOTONIC_CLOCK=False \
	  -DLIBCXX_ENABLE_RTTI=False \
	  -DLIBCXX_ENABLE_THREADS=False \
	  -DLIBCXX_ENABLE_SHARED=False \
	  -DLIBCXX_ENABLE_STATIC=True \
	  $(BASEDIR)/llvm-project/libcxx
	make -j4
	make install

clean:
	rm -rf newlib-cygwin
	rm -rf llvm-project
	rm -rf build_libcxx
	rm -rf build_libcxxabi
	rm -rf build_newlib
	rm -rf lib
