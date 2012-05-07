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
 * \file factory.hpp
 * \brief The design pattern of the factory.
 * \author Julien Jorge
 */
#ifndef __CLAW_FACTORY_HPP__
#define __CLAW_FACTORY_HPP__

#ifdef CLAW_FACTORY_IS_SINGLETON
#include <claw/basic_singleton.hpp>
#endif

#include <claw/exception.hpp>
#include <map>

namespace claw
{
  namespace pattern
  {
    /**
     * \brief Exception thrown when an incorrect identifier is given to a type.
     * \author Julien Jorge
     */
    class bad_type_identifier:
      public exception
    {
    public:
      /**
       * \brief Constructor.
       */
      bad_type_identifier() throw()
        : exception("No type has this identifier.")
      { }
    }; // class bad_type_identifier

    /**
     * \brief The design pattern of the factory allow to dynamically instanciate
     *        classes of various types given an identifier of this type.
     *
     * \b Template \b parameters:
     * - \a BaseClass A base class for all classes instanciated by this factory.
     * - \a IdentifierType The type of the identifiers of the classes
     *   instanciated by this factory.
     *
     * \b Type \b requirements:
     * - IdentifierType must be "less than" comparable.
     */
    template<typename BaseClass, typename IdentifierType>
#ifdef CLAW_FACTORY_IS_SINGLETON
    class factory:
      public basic_singleton< factory<BaseClass, IdentifierType> >
#else
    class factory
#endif
    {
    private:
      /**
       * \brief A class creator is an object that allocate a given type. This is
       *        the base class for all class creators.
       */
      class class_creator_base
      {
      public:
        virtual ~class_creator_base();
        virtual BaseClass* create() const = 0;

      }; // class class_creator_base

      /**
       * \brief A class creator is an object that allocate a given type.
       *
       * \b Template \b parameters:
       * - \a Derived The type of the class to instanciate.
       *
       * \b Type \b requirements:
       * - Derived must be default constructible.
       */
      template<typename Derived>
      class class_creator:
        public class_creator_base
      {
      public:
        virtual Derived* create() const;

      }; // class class_creator

      /** \brief The type of the data used to identify a class. */
      typedef IdentifierType identifier_type;

      /** \brief A base class for all instances created by this factory. */
      typedef BaseClass base_class;

      /** \brief The association identifier <-> type. */
      typedef std::map<identifier_type, class_creator_base*> class_map;

    public:
      ~factory();

      template<typename T>
      bool register_type( const identifier_type& id );

      base_class* create( const identifier_type& id ) const;

      bool is_known_type( const identifier_type& id ) const;

    private:
      /** \brief The association identifier <-> type. */
      class_map m_classes;

    }; // class factory

  } // namespace pattern
} // namespace claw

#include <claw/impl/factory.tpp>

#endif // __CLAW_FACTORY_HPP__
