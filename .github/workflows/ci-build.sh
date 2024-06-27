# Versions used
CAIRO_VERSION=1.17.4
PIXMAN_VERSION=0.42.2
LIBPNG_VERSION=1.6.40
ZLIB_VERSION=1.2.13
FREETYPE_VERSION=2.13.1
DEVIL_VERSION=1.8.0
TIFF_VERSION=4.5.1
JPEG_VERSION=v9e
JASPER_VERSION=4.0.0
LCMS2_VERSION=2.15
LIBSQUISH_VERSION=1.15
LIBMNG_VERSION=2.0.3

# Download packages if not already
wget -nc https://www.cairographics.org/snapshots/cairo-$CAIRO_VERSION.tar.xz
wget -nc https://www.cairographics.org/releases/pixman-$PIXMAN_VERSION.tar.gz
wget -nc https://download.sourceforge.net/libpng/libpng-$LIBPNG_VERSION.tar.gz
wget -nc http://www.zlib.net/zlib-$ZLIB_VERSION.tar.gz
wget -nc http://download.osgeo.org/libtiff/tiff-$TIFF_VERSION.tar.gz
wget -nc http://www.ijg.org/files/jpegsrc.$JPEG_VERSION.tar.gz
wget -nc https://sourceforge.net/projects/openil/files/DevIL/$DEVIL_VERSION/DevIL-$DEVIL_VERSION.tar.gz
wget -nc https://github.com/jasper-software/jasper/releases/download/version-$JASPER_VERSION/jasper-$JASPER_VERSION.tar.gz
wget -nc https://github.com/mm2/Little-CMS/releases/download/lcms$LCMS2_VERSION/lcms2-$LCMS2_VERSION.tar.gz
wget -nc https://sourceforge.net/projects/libsquish/files/libsquish-$LIBSQUISH_VERSION.tgz
wget -nc http://sourceforge.net/projects/libmng/files/libmng-devel/$LIBMNG_VERSION/libmng-$LIBMNG_VERSION.tar.gz
wget -nc http://download.savannah.gnu.org/releases/freetype/freetype-$FREETYPE_VERSION.tar.gz

# Extract packages if not already
if [ ! -d cairo ]; then
    echo "Extracting cairo-$CAIRO_VERSION..."
    tar -xJf cairo-$CAIRO_VERSION.tar.xz
    mv cairo-$CAIRO_VERSION cairo
fi
if [ ! -d pixman ]; then
    echo "Extracting pixman-$PIXMAN_VERSION..."
    tar -xzf pixman-$PIXMAN_VERSION.tar.gz
    mv pixman-$PIXMAN_VERSION pixman
fi
if [ ! -d libpng ]; then
    echo "Extracting libpng-$LIBPNG_VERSION..."
    tar -xzf libpng-$LIBPNG_VERSION.tar.gz
    mv libpng-$LIBPNG_VERSION libpng
fi
if [ ! -d zlib ]; then
    echo "Extracting zlib-$ZLIB_VERSION..."
    tar -xzf zlib-$ZLIB_VERSION.tar.gz
    mv zlib-$ZLIB_VERSION zlib
fi
if [ ! -d freetype ]; then
    echo "Extracting freetype-$FREETYPE_VERSION..."
    tar -xzf freetype-$FREETYPE_VERSION.tar.gz
    mv freetype-$FREETYPE_VERSION freetype
fi
if [ ! -d tiff ]; then
    echo "Extracting tiff-$TIFF_VERSION..."
    tar -xzf tiff-$TIFF_VERSION.tar.gz
    mv tiff-$TIFF_VERSION tiff
fi
if [ ! -d jpeg ]; then
    echo "Extracting jpegsrc.$JPEG_VERSION..."
    tar -xzf jpegsrc.$JPEG_VERSION.tar.gz
    mv jpegsrc.$JPEG_RENAME jpeg
fi
if [ ! -d DevIL ]; then
    echo "Extracting DevIL-$DEVIL_VERSION..."
    tar -xzf DevIL-$DEVIL_VERSION.tar.gz
    mv DevIL-$DEVIL_VERSION DevIL
fi
if [ ! -d jasper ]; then
    echo "Extracting jasper-$JASPER_VERSION..."
    tar -xzf jasper-$JASPER_VERSION.tar.gz
    mv jasper-$JASPER_VERSION jasper
fi
if [ ! -d lcms2 ]; then
    echo "Extracting $LCMS2_VERSION..."
    tar -xzf lcms2-$LCMS2_VERSION.tar.gz
    mv lcms2-$LCMS2_VERSION lcms2
fi
if [ ! -d libsquish ]; then
    echo "Extracting libsquish-$LIBSQUISH_VERSION..."
    mkdir libsquish-$LIBSQUISH_VERSION
    tar -xzf libsquish-$LIBSQUISH_VERSION.tgz -C libsquish-$LIBSQUISH_VERSION
    mv libsquish-$LIBSQUISH_VERSION libsquish
fi
