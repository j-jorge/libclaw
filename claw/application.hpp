/*
  CLAW - a C++ Library Absolutely Wonderful

  CLAW is a free library without any particular aim but being useful to 
  anyone.

  Copyright (C) 2005-2011 Julien Jorge

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: julien.jorge@gamned.org
*/
/**
 * \file application.hpp
 * \brief A class to represent the application.
 * \author Julien Jorge
 */
#ifndef __CLAW_APPLICATION_HPP__
#define __CLAW_APPLICATION_HPP__

#include <claw/arguments_table.hpp>

/**
 * \brief Use this macro to implement your application's class. If you want to
 * implement your application class named my_app, write somewhere in your
 * code (no semi column at the end of the line) :
 * <tt> CLAW_APPLICATION_IMPLEMENT(my_app) </tt>
 * \pre App must inerhit from claw::application.
 */
#define CLAW_APPLICATION_IMPLEMENT(App)         \
  int main(int argc, char** argv)               \
  {                                             \
    App app(argc, argv);                        \
    return app.run();                           \
  }                                             \

namespace claw
{
  /**
   * \brief A class to represent the application.
   *
   * The claw::application understand the following command line arguments :
   * - --log-file=string Sets the file in which the logs are written,
   * - --log-level=integer Sets the log level.
   * 
   * \author Julien Jorge
   */
  class application
  {
  public:
    application( int& argc, char** &argv );
    virtual ~application();

    virtual int run() = 0;

  protected:
    /** \brief The arguments passed by the system. */
    arguments_table m_arguments;
  }; // class application
} // namespace claw

#endif // __CLAW_APPLICATION_HPP__
