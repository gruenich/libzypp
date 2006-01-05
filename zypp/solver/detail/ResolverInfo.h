#/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* ResolverInfo.h
 *
 * Copyright (C) 2000-2002 Ximian, Inc.
 * Copyright (C) 2005 SUSE Linux Products GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef ZYPP_SOLVER_DETAIL_RESOLVER_INFO_H
#define ZYPP_SOLVER_DETAIL_RESOLVER_INFO_H

#include <iosfwd>
#include <list>
#include <map>
#include <string>

#include "zypp/base/ReferenceCounted.h"
#include "zypp/base/NonCopyable.h"
#include "zypp/base/PtrTypes.h"
#include "zypp/solver/detail/ResolverInfoPtr.h"
#include "zypp/solver/detail/ResItem.h"
#include "zypp/solver/detail/Channel.h"

/////////////////////////////////////////////////////////////////////////
namespace zypp
{ ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  namespace solver
  { /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    namespace detail
    { ///////////////////////////////////////////////////////////////////

typedef enum {
    RESOLVER_INFO_TYPE_INVALID = 0,
    RESOLVER_INFO_TYPE_NEEDED_BY,
    RESOLVER_INFO_TYPE_CONFLICTS_WITH,
    RESOLVER_INFO_TYPE_OBSOLETES,
    RESOLVER_INFO_TYPE_DEPENDS_ON,
    RESOLVER_INFO_TYPE_CHILD_OF,
    RESOLVER_INFO_TYPE_MISSING_REQ,
    RESOLVER_INFO_TYPE_MISC
} ResolverInfoType;

#define RESOLVER_INFO_PRIORITY_USER      500
#define RESOLVER_INFO_PRIORITY_VERBOSE   100
#define RESOLVER_INFO_PRIORITY_DEBUGGING   0

typedef void (*ResolverInfoFn) (ResolverInfo_Ptr info, void *data);

typedef std::list <ResolverInfo_Ptr> ResolverInfoList;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ResolverInfo

class ResolverInfo : public base::ReferenceCounted, private base::NonCopyable {

  private:

    ResolverInfoType _type;
    ResItem_constPtr _resItem;
    int _priority;

    bool _error;
    bool _important;

  protected:

    ResolverInfo (ResolverInfoType type, ResItem_constPtr resItem, int priority);

  public:

    virtual ~ResolverInfo();

    void copy (ResolverInfo_constPtr from);

    // ---------------------------------- I/O

    static std::string toString (const ResolverInfo & context, bool full = false);
    virtual std::ostream & dumpOn(std::ostream & str ) const;
    friend std::ostream& operator<<(std::ostream&, const ResolverInfo & context);
    virtual std::string asString (void ) const;

    // ---------------------------------- accessors

    ResolverInfoType type (void) const { return _type; }
    ResItem_constPtr resItem (void) const { return _resItem; }
    int priority (void) const { return _priority; }

    int error (void) const { return _error; }
    void flagAsError (void) { _error = true; }
    int important (void) const { return _important; }
    void flagAsImportant (void) { _important = true; }

    // ---------------------------------- methods

    bool merge (ResolverInfo_Ptr to_be_merged);
    virtual ResolverInfo_Ptr copy (void) const;

    bool isAbout (ResItem_constPtr resItem) const;
};

///////////////////////////////////////////////////////////////////
    };// namespace detail
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
  };// namespace solver
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
};// namespace zypp
/////////////////////////////////////////////////////////////////////////
#endif // ZYPP_SOLVER_DETAIL_RESOLVER_INFO_H

