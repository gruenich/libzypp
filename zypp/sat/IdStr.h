/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/sat/IdStr.h
 *
*/
#ifndef ZYPP_SAT_IDSTR_H
#define ZYPP_SAT_IDSTR_H

#include <iosfwd>
#include <string>

#include "zypp/base/SafeBool.h"

#include "zypp/sat/detail/PoolMember.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  namespace sat
  { /////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : IdStr
    //
    /** Access to the sat-pools string space.
     * Construction from string will place a copy of the string in the
     * string space, if it is not already present.
    */
    class IdStr: protected detail::PoolMember,
                 private base::SafeBool<IdStr>
    {
      public:
        /** Default ctor, empty string. */
        IdStr() : _id( Empty.id() ) {}
        /** Ctor from id. */
        explicit IdStr( detail::IdType id_r ) : _id( id_r ) {}
        /** Ctor from string. */
        explicit IdStr( const char * str_r );
        /** Ctor from string. */
        explicit IdStr( const std::string & str_r );
        /** Evaluate in a boolean context (\c != \c Null). */
        using base::SafeBool<IdStr>::operator bool_type;
      public:
        /** No or Null string. */
        static const IdStr Null;
        /** Empty string. */
        static const IdStr Empty;
      public:
        bool empty() const
        { return( _id == Empty.id() ); }
        unsigned size() const;
      public:
        /** Conversion to <tt>const char *</tt> */
        const char * c_str() const;
        /** Conversion to <tt>std::string</tt> */
        std::string string() const;
        /** \overload */
        std::string asString() const
        { return string(); }

      public:
        /** Expert backdoor. */
        detail::IdType id() const
        { return _id; }
      private:
        friend base::SafeBool<IdStr>::operator bool_type() const;
        bool boolTest() const { return _id; }
      private:
        detail::IdType _id;
    };
    ///////////////////////////////////////////////////////////////////

    /** \relates IdStr Stream output */
    std::ostream & operator<<( std::ostream & str, const IdStr & obj );

    /** \relates IdStr */
    inline bool operator==( const IdStr & lhs, const IdStr & rhs )
    { return lhs.id() == rhs.id(); }

    /** \relates IdStr */
    inline bool operator!=( const IdStr & lhs, const IdStr & rhs )
    { return lhs.id() != rhs.id(); }


    /////////////////////////////////////////////////////////////////
  } // namespace sat
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_SAT_IDSTR_H
