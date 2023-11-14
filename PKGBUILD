pkgname=swayipc-cpp
_name="${pkgname%-git}"
pkgver=0.1.0
pkgrel=1
pkgdesc='A simple C++17 library for controlling sway window manager.'
url='https://github.com/aokellermann/swayipc-cpp'
arch=(x86_64)
makedepends=(cmake extra-cmake-modules git conan1)
provides=("$_name")
conflicts=("$_name")
source=("git+$url")
sha256sums=(SKIP)

_cmake_flags=(
	-DCMAKE_BUILD_TYPE=Release
	-DCMAKE_INSTALL_LIBDIR=lib
	-DCMAKE_INSTALL_PREFIX=/usr
)

build() {
	mkdir build
	cd build
	cmake ../$pkgname "${_cmake_flags[@]}" -DBUILD_SHARED_LIBS=ON
	make
}

check() {
	cd build
	cmake ../$pkgname "${_cmake_flags[@]}" -DBUILD_SHARED_LIBS=ON
	make
}

package() {
	cd build
	make "DESTDIR=$pkgdir" install
}
