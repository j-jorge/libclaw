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
  contact: julien.jorge@stuff-o-matic.com
*/
#include "claw/tween/single_tweener.hpp"

#include "claw/tween/easing/easing_back.hpp"
#include "claw/tween/easing/easing_bounce.hpp"
#include "claw/tween/easing/easing_circ.hpp"
#include "claw/tween/easing/easing_cubic.hpp"
#include "claw/tween/easing/easing_elastic.hpp"
#include "claw/tween/easing/easing_expo.hpp"
#include "claw/tween/easing/easing_linear.hpp"
#include "claw/tween/easing/easing_none.hpp"
#include "claw/tween/easing/easing_quad.hpp"
#include "claw/tween/easing/easing_quart.hpp"
#include "claw/tween/easing/easing_quint.hpp"
#include "claw/tween/easing/easing_sine.hpp"

#include <fstream>
#include <iostream>

#include <boost/bind.hpp>

class tester
{
public:
  void run(std::ostream& output, double init, double end, double duration,
           claw::tween::single_tweener::easing_function e);
  void run_ref(std::ostream& output, double init, double end, double duration,
               claw::tween::single_tweener::easing_function e);

private:
  void print_val(double d) const;

private:
  double m_date;

  std::ostream* m_output;

}; // class tester

void tester::print_val(double d) const
{
  *m_output << m_date << '\t' << d << std::endl;
}

void tester::run(std::ostream& output, double init, double end,
                 double duration,
                 claw::tween::single_tweener::easing_function e)
{
  claw::tween::single_tweener t(init, end, duration,
                                boost::bind(&tester::print_val, this, _1), e);

  m_output = &output;
  m_date = 0;

  t.update(0);

  while(!t.is_finished())
    {
      m_date += 0.1;
      t.update(0.1);
    }
}

void tester::run_ref(std::ostream& output, double init, double end,
                     double duration,
                     claw::tween::single_tweener::easing_function e)
{
  double val(0);
  claw::tween::single_tweener t(val, end, duration, e);

  m_output = NULL;
  m_date = 0;

  t.update(0);

  while(!t.is_finished())
    {
      m_date += 0.1;
      t.update(0.1);
      output << m_date << '\t' << val << std::endl;
    }
}

template <typename Easing>
void test()
{
  tester t;
  std::ofstream f("in.plot");

  t.run(f, 0, 1, 10, Easing::ease_in);

  f.close();
  f.open("out.plot");
  t.run(f, 0, 1, 10, Easing::ease_out);

  f.close();
  f.open("inout.plot");
  t.run(f, 0, 1, 10, Easing::ease_in_out);

  f.close();
  f.open("ref-inout.plot");
  t.run(f, 0, 1, 10, Easing::ease_in_out);
}

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      std::cerr << "usage: " << argv[0] << " easing" << std::endl;
      std::cerr << "where easing is one of: none, back, bounce, circ, cubic, "
                   "elastic, expo, linear, quad, quart, quint, "
                << "sine";
      std::cerr << std::endl;
      return 1;
    }

  const std::string easing(argv[1]);

  if(easing == "none")
    test<claw::tween::easing_none>();
  else if(easing == "back")
    test<claw::tween::easing_back>();
  else if(easing == "bounce")
    test<claw::tween::easing_bounce>();
  else if(easing == "circ")
    test<claw::tween::easing_circ>();
  else if(easing == "cubic")
    test<claw::tween::easing_cubic>();
  else if(easing == "elastic")
    test<claw::tween::easing_elastic>();
  else if(easing == "expo")
    test<claw::tween::easing_expo>();
  else if(easing == "linear")
    test<claw::tween::easing_linear>();
  else if(easing == "quad")
    test<claw::tween::easing_quad>();
  else if(easing == "quart")
    test<claw::tween::easing_quart>();
  else if(easing == "quint")
    test<claw::tween::easing_quint>();
  else if(easing == "sine")
    test<claw::tween::easing_sine>();
  else
    std::cerr << "Unknown easing type: '" << easing << "'." << std::endl;

  return 0;
}
