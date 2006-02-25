/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/detail/AtomImplIf.h
 *
*/
#ifndef ZYPP_DETAIL_ATOMIMPLIF_H
#define ZYPP_DETAIL_ATOMIMPLIF_H

#include "zypp/detail/ResObjectImplIf.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  class Atom;

  ///////////////////////////////////////////////////////////////////
  namespace detail
  { /////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //
    //	CLASS NAME : AtomImplIf
    //
    /** Abstact Atom implementation interface.
    */
    class AtomImplIf : public ResObjectImplIf
    {
    public:
      typedef Atom ResType;

    public:

    };
    ///////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
  } // namespace detail
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_DETAIL_ATOMIMPLIF_H
