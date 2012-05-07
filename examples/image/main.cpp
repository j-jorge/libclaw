#include <iostream>
#include <fstream>

#include <claw/image.hpp>
#include <claw/targa.hpp>
#include <claw/bitmap.hpp>
#include <claw/pcx.hpp>
#include <claw/png.hpp>
#include <claw/jpeg.hpp>
#include <claw/xbm.hpp>

/*----------------------------------------------------------------------------*/
void save_bitmap
( const claw::graphic::image& img, const std::string& filename )
{
  std::ofstream f( (filename + ".bmp").c_str(), std::ios::binary );
  claw::graphic::bitmap::writer( img, f );
  f.close();
}

/*----------------------------------------------------------------------------*/
void save_pcx
( const claw::graphic::image& img, const std::string& filename )
{
  std::ofstream f( (filename + ".pcx").c_str(), std::ios::binary );
  claw::graphic::pcx::writer( img, f );
  f.close();
}

/*----------------------------------------------------------------------------*/
void save_xbm
( const claw::graphic::image& img, const std::string& filename,
  const std::string& name, const claw::math::coordinate_2d<int>* hot )
{
  claw::graphic::xbm::writer::options opt(name, hot);

  std::ostringstream oss;

  oss << filename << '-' << name << "-";

  if ( hot==NULL )
    oss << ".";
  else
    oss << hot->x << ":" << hot->y;

  oss << ".xbm";

  std::ofstream f( oss.str().c_str() );
  claw::graphic::xbm::writer( img, f, opt );
  f.close();
}

/*----------------------------------------------------------------------------*/
void save_xbm( const claw::graphic::image& img, const std::string& filename )
{
  claw::math::coordinate_2d<int> hot(10, 10);

  std::cout << "no hot" << std::endl;
  save_xbm( img, filename, "cold", NULL );

  std::cout << "hot" << std::endl;
  save_xbm( img, filename, "hot", &hot );
}

/*----------------------------------------------------------------------------*/
void save_targa
( const claw::graphic::image& img, const std::string& filename, bool rle )
{
  std::ostringstream oss;

  oss << filename << '-' << (rle ? "rle" : "raw") << ".tga";

  std::ofstream f( oss.str().c_str(), std::ios::binary );
  claw::graphic::targa::writer( img, f, rle );
  f.close();
}

/*----------------------------------------------------------------------------*/
void save_targa( const claw::graphic::image& img, const std::string& filename )
{
  std::cout << "compressed" << std::endl;
  save_targa( img, filename, true );
  std::cout << "not compressed" << std::endl;
  save_targa( img, filename, false );
}

/*----------------------------------------------------------------------------*/
void save_png
( const claw::graphic::image& img, const std::string& filename,
  claw::graphic::png::writer::options::compression_level comp,
  claw::graphic::png::writer::options::interlace_type inter )
{
  claw::graphic::png::writer::options opt(comp, inter);

  std::ostringstream oss;

  oss << filename << '-';

  switch (comp)
    {
    case claw::graphic::png::writer::options::no_compression:
      oss << "none";
      break;
    case claw::graphic::png::writer::options::best_compression:
      oss << "best";
      break;
    case claw::graphic::png::writer::options::best_speed:
      oss << "speed";
      break;
    case claw::graphic::png::writer::options::default_compression:
      oss << "default";
      break;
    }

  oss << '-';

  if (inter == claw::graphic::png::writer::options::none )
    oss << "none";
  else
    oss << "adam";

  oss << ".png";
  std::ofstream f( oss.str().c_str(), std::ios::binary );
  claw::graphic::png::writer( img, f, opt );
  f.close();
}

/*----------------------------------------------------------------------------*/
void save_png( const claw::graphic::image& img, const std::string& filename )
{
  std::cout << "not compressed, not interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::no_compression,
            claw::graphic::png::writer::options::none );
  std::cout << "best compression, not interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::best_compression,
            claw::graphic::png::writer::options::none );
  std::cout << "best speed compression, not interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::best_speed,
            claw::graphic::png::writer::options::none );
  std::cout << "default compression, not interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::default_compression,
            claw::graphic::png::writer::options::none );

  std::cout << "not compressed, interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::no_compression,
            claw::graphic::png::writer::options::adam7 );
  std::cout << "best compression, interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::best_compression,
            claw::graphic::png::writer::options::adam7 );
  std::cout << "best speed compression, interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::best_speed,
            claw::graphic::png::writer::options::adam7 );
  std::cout << "default compression, interlaced" << std::endl;
  save_png( img, filename,
            claw::graphic::png::writer::options::default_compression,
            claw::graphic::png::writer::options::adam7 );
}

/*----------------------------------------------------------------------------*/
void save_jpeg( const claw::graphic::image& img, const std::string& filename,
                bool progressive, unsigned int quality )
{
  claw::graphic::jpeg::writer::options opt;
  opt.progressive = progressive;
  opt.quality = quality;

  std::ostringstream oss;

  oss << filename << '-' << (progressive ? 'p' : '.') << '-' << quality
      << ".jpg";

  std::ofstream f( oss.str().c_str(), std::ios::binary );
  claw::graphic::jpeg::writer( img, f, opt );
  f.close();
}

/*----------------------------------------------------------------------------*/
void save_jpeg( const claw::graphic::image& img, const std::string& filename )
{
  std::cout << "non progressive, quality=1" << std::endl;
  save_jpeg( img, filename, false, 1 );
  std::cout << "non progressive, quality=50" << std::endl;
  save_jpeg( img, filename, false, 50 );
  std::cout << "non progressive, quality=80" << std::endl;
  save_jpeg( img, filename, false, 80 );
  std::cout << "non progressive, quality=100" << std::endl;
  save_jpeg( img, filename, false, 100 );

  std::cout << "progressive, quality=1" << std::endl;
  save_jpeg( img, filename, true, 1 );
  std::cout << "progressive, quality=50" << std::endl;
  save_jpeg( img, filename, true, 50 );
  std::cout << "progressive, quality=80" << std::endl;
  save_jpeg( img, filename, true, 80 );
  std::cout << "progressive, quality=100" << std::endl;
  save_jpeg( img, filename, true, 100 );
}

/*----------------------------------------------------------------------------*/
void save( const claw::graphic::image& img, const std::string& filename )
{
  std::cout << "== Saving pcx files ==" << std::endl;
  save_pcx( img, filename );

  std::cout << "== Saving jpg files ==" << std::endl;
  save_jpeg( img, filename );

  std::cout << "== Saving png files ==" << std::endl;
  save_png( img, filename );

  std::cout << "== Saving bitmap files ==" << std::endl;
  save_bitmap( img, filename );

  std::cout << "== Saving targa files ==" << std::endl;
  save_targa( img, filename );

  std::cout << "== Saving xbm files ==" << std::endl;
  save_xbm( img, filename );
}

/*----------------------------------------------------------------------------*/
std::string get_base_name( const std::string& filename )
{
  std::string::size_type end = filename.find_last_of('.');
  std::string::size_type begin = filename.find_last_of('/');
  std::string result;

  if ( begin != std::string::npos )
    {
      if ( end != std::string::npos )
        result = filename.substr(begin+1, end - begin - 1);
      else
        result = filename.substr(begin+1);
    }
  else if ( end != std::string::npos )
    result = filename.substr(0, end);
  else
    result = filename;

  return result;
}

/*----------------------------------------------------------------------------*/
int main( int argc, char* argv[] )
{
  if (argc == 1)
    std::cerr << argv[0] << " image_file..." << std::endl;
  else
    for (int i=1; i!=argc; ++i)
      {
        std::ifstream f(argv[i], std::ios::binary);

        if (f)
          {
            try
              {
                claw::graphic::image img( f );
                save( img, get_base_name(argv[i]) );
              }
            catch(std::exception& e)
              {
                std::cerr << "Exception: " << e.what() << std::endl;
              }
          }
        else
          std::cerr << "can't open '" << argv[i] << "'" << std::endl;
      }

  return 0;
}
