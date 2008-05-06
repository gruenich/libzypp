/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/target/modalias/Modalias.cc
 *
*/
extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <fnmatch.h>
}
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <iostream>

#undef ZYPP_BASE_LOGGER_LOGGROUP
#define ZYPP_BASE_LOGGER_LOGGROUP "MODALIAS"
#include "zypp/base/Logger.h"

#include "zypp/target/modalias/Modalias.h"


using std::endl;
using std::string;


///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  namespace target
  { /////////////////////////////////////////////////////////////////

struct modalias_list {
	char *modalias;
	struct modalias_list *next;
};

    ///////////////////////////////////////////////////////////////////
    namespace
    { /////////////////////////////////////////////////////////////////

/*
 * For each file in the directory PATH other than . and .., call
 * FUNC with the arguments PATH, the file's name, and ARG.
 *
 * If FUNC returns a non-zero return value, stop reading the directory
 * and return that value. Returns -1 if an error occurs.
 */
int
foreach_file(const char *path, int (*func)(const char *, const char *, void *),
	     void *arg)
{
	DIR *dir;
	struct dirent *dirent;
	int ret = 0;

	if (!(dir = opendir(path)))
		return -1;
	while ((dirent = readdir(dir)) != NULL) {

		if (strcmp(dirent->d_name, ".") == 0 ||
		    strcmp(dirent->d_name, "..") == 0)
			continue;
		if ((ret = func(path, dirent->d_name, arg)) != 0)
			break;
	}
	if (closedir(dir) != 0)
		return -1;
	return ret;
}

/*
 * If DIR/FILE/modalias exists, remember this modalias on the linked modalias list
 * passed in in ARG. Never returns an error.
 */
int
read_modalias(const char *dir, const char *file, void *arg)
{
	char path[PATH_MAX];
	int fd;
	ssize_t len;
	char modalias[PATH_MAX];
	struct modalias_list **list = (struct modalias_list **)arg, *entry;

	snprintf(path, sizeof(path), "%s/%s/modalias", dir, file);
	if ((fd = open(path, O_RDONLY)) == -1)
		return 0;
	len = read(fd, modalias, sizeof(modalias) - 1);
	if (len < 0)
		goto out;
	while (len > 0 && modalias[len - 1] == '\n')
		len--;
	modalias[len] = 0;

	if ((entry = (struct modalias_list *)malloc(sizeof(*entry))) == NULL)
		goto out;
	if ((entry->modalias = strdup(modalias)) == NULL) {
	        free(entry);
		goto out;
	}
	entry->next = *list;
	*list = entry;
	XXX << "system modalias: " << entry->modalias << endl;

out:
	(void) close(fd);
	return 0;
}

/*
 * Iterate over all devices on a bus (/sys/bus/BUS/devices/<*>)
 * and remembers all module aliases for those devices on
 * the linked modalias list passed in in ARG.
 */
int
iterate_bus(const char *dir, const char *file, void *arg)
{
	char path[PATH_MAX];

	snprintf(path, sizeof(path), "%s/%s/devices", dir, file);
	(void) foreach_file(path, read_modalias, arg);

	return 0;
}

/*
 * Iterate over all devices in a class (/sys/class/CLASS/<*>)
 * and remembers all module aliases for those devices on
 * the linked modalias list passed in in ARG.
 */
int
iterate_class(const char *dir, const char *file, void *arg)
{
	char path[PATH_MAX];

	snprintf(path, sizeof(path), "%s/%s", dir, file);
	(void) foreach_file(path, read_modalias, arg);

	return 0;
}

      /////////////////////////////////////////////////////////////////
    } // namespace
    ///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Modalias::Impl
//
/** Modalias implementation. */
struct Modalias::Impl
{
    struct modalias_list *_modaliases;

    /** Ctor. */
    Impl()
	: _modaliases(0)
    {
	const char *dir;
	char path[PATH_MAX];

	dir = getenv("ZYPP_MODALIAS_SYSFS");
	if (!dir)
		dir = "/sys";
	DBG << "Using /sys directory : " << dir << endl;

	snprintf(path, sizeof(path), "%s/bus", dir);
	foreach_file( path, iterate_bus, &_modaliases );

	snprintf(path, sizeof(path), "%s/class", dir);
	foreach_file( path, iterate_class, &_modaliases );
    }

    /** Dtor. */
    ~Impl()
    {
	while (_modaliases != NULL) {
	    struct modalias_list *l = _modaliases;
	    _modaliases = _modaliases->next;
	    free(l->modalias);
	    free(l);
	}
    }

    /*
     * Check if a device on the system matches a modalias PATTERN.
     *
     * Returns NULL if no matching device is found, and the modalias
     * of the first matching device otherwise. (More than one device
     * may match a given pattern.)
     *
     * On a system that has the following device,
     *
     *   pci:v00008086d0000265Asv00008086sd00004556bc0Csc03i00
     *
     * modalias_matches("pci:v00008086d0000265Asv*sd*bc*sc*i*") will
     * return a non-NULL value.
     */
    bool query( const char * cap_r ) const
    {
        if ( cap_r )
        {
          struct modalias_list *l;
          for (l = _modaliases; l; l = l->next) {
            if ( fnmatch( cap_r, l->modalias, 0 ) == 0 )
              return true;
          }
        }
        return false;
    }

  public:
     /** Offer default Impl. */
    static shared_ptr<Impl> nullimpl()
    {
	static shared_ptr<Impl> _nullimpl( new Impl );
	return _nullimpl;
    }

};  // struct Modalias::Impl

///////////////////////////////////////////////////////////////////

/** \relates Modalias::Impl Stream output
     * And maybe std::ostream & operator<< Modalias::Impl below too.
     * return libhal version or something like that.
 */
inline std::ostream & operator<<( std::ostream & str, const Modalias::Impl & obj )
{
  return str << "Modalias::Impl";
}

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : Modalias
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Modalias::Modalias
//	METHOD TYPE : Ctor
//
Modalias::Modalias()
: _pimpl( Impl::nullimpl() )
{}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Modalias::~Modalias
//	METHOD TYPE : Dtor
//
Modalias::~Modalias()
{}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : Modalias::instance
//	METHOD TYPE : Modalias &
//
Modalias & Modalias::instance()
{
  static Modalias _singleton;
  return _singleton;
}

///////////////////////////////////////////////////////////////////
// Foreward to implenemtation
///////////////////////////////////////////////////////////////////

bool Modalias::query( const char * cap_r ) const
{ return _pimpl->query( cap_r ); }

/******************************************************************
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : std::ostream &
*/
std::ostream & operator<<( std::ostream & str, const Modalias & obj )
{
  return str << *obj._pimpl;
}

    /////////////////////////////////////////////////////////////////
  } // namespace target
  ///////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////

