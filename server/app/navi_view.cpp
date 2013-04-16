#include "reply_view.hpp"
#include "navi_view.hpp"
#include "last_position.hpp"
#include "redirect_view.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>


// #define DEBUG_CONTENT



NaviView::NaviView(const string &username,const string &user_uid)
:user_(username)
,user_uid_(user_uid)
,category_()
,last_(new LastPostion(user_uid_))
{
}

NaviView::NaviView(const string &username,const string &user_uid,const string &category)
:user_(username)
,user_uid_(user_uid)
,category_(category)
,last_(new LastPostion(user_uid_))
{
}
NaviView::~NaviView()
{
}

bool NaviView::getContent(const string &doc_root,string &contents)
{
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewNavi.html";
#ifdef DEBUG_CONTENT
        std::cout << "full_path=" << full_path << std::endl;
#endif
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is)
        {
            return false;
        }
        // Fill out the reply to be sent to the client.
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0) {
            contents.append(buf, is.gcount());
        }
    }
    // replace users
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_USER$",user_);
    }
    // replace navi path
    {
        this->replace_source_path(contents);
    }
    // search all navi items and create a table.
    {
        std::string table_navi;
        unsigned int counter = iConstColNum_;
        for(auto it = navi_items_.begin();it != navi_items_.end();it++ )
        {
#ifdef DEBUG_CONTENT
            std::cout << "it->first=<" << it->first << ">" << std::endl;
#endif
            // if no category is speciled,list all categories
            if(true == category_.empty())
            {
                string tr("");
                if(iConstColNum_ == counter)
                {
                    tr += "<tr>\n";
                }
                tr +=  "<td class=\"content\">";
                tr +=  "<a href=\"/users";
                tr +=  "/" + user_;
                tr +=  "/" + it->first;
                tr +=  "\">";
                tr +=  it->first;
                tr += "</a></td>\n";
                if(0 == --counter )
                {
                    tr += "</tr>\n";
                    counter = iConstColNum_;
                }
                table_navi += tr;
                continue;
            }
            // category is speciled,list all in the category
            if(category_ == it->first)
            {
                unsigned int counter = iConstColNum_;
                for(auto subit = it->second.begin();subit != it->second.end();subit++)
                {
                    string tr("");
                    if(iConstColNum_ == counter)
                    {
                        tr += "<tr>\n";
                    }
                    tr +=  "<td class=\"content\">";
                    tr +=  "<a href=\"/users";
                    tr +=  "/" + user_;
                    tr +=  "/" + it->first;
                    tr +=  "/" + *subit;
                    tr +=  "\">";
                    tr +=  *subit;
                    tr += "</a></td>\n";
                    if(0 == --counter)
                    {
                        tr += "</tr>\n";
                        counter = iConstColNum_;
                    }
                    table_navi += tr;
                }
                if(iConstColNum_ != counter)
                {
                    table_navi += "</tr>\n";
                }
                break;
            }
        }
        if(true == category_.empty() && iConstColNum_ != counter)
        {
            table_navi += "</tr>\n";
        }
        boost::algorithm::replace_all(contents,"$BOW_TMPL_NAVI_TABLE$",table_navi);
    }
    this->replace_loginout(contents);
    if(category_.empty())
    {
        last_->set("");
    }
    else
    {
        last_->set(category_);
    }
    return true;
}

void NaviView::replace_source_path(string &contents)
{
    string href("/users");
    std::string path;
    // user_
    href += "/" + user_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += user_;
    path += "</a>";
    // category_
    if(false == category_.empty())
    {
        path += "<span>/</span>";
        href += "/" + category_;
        path += "<a href=\"";
        path += href;
        path += "\">";
        path += category_;
        path += "</a>";
    }
    boost::algorithm::replace_all(contents,"$BOW_TMPL_NAVI_PATH$",path);
}


static string strConstLogin =
"<a href=\"/login.php\" data-method=\"post\" id=\"login\">login</a>";
static string strConstLogout =
"<a href=\"/logout.php\" data-method=\"post\" id=\"logout\">logout</a>";

void NaviView::replace_loginout(string &contents)
{
    if( "guest" == user_ || user_.empty())
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_USER_LOGINOUT$",strConstLogin);
    }
    else
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_USER_LOGINOUT$",strConstLogout);
    }
}




const map<string,vector<string>> NaviView::navi_items_ =
{
    { "ruby",
        {"helloworld"}
    },
    { "c_cxx",
        {
            "helloworld","helloworldcxx",
            "assert","printf",
            "__setfpucw",
            "a64l",
            "abort",
            "abs",
            "acos",
            "acosh",
            "addseverity",
            "adjtime",
            "aio_cancel",
            "aio_error",
            "aio_fsync",
            "aio_init",
            "aio_read",
            "aio_return",
            "aio_suspend",
            "aio_write",
            "alloca",
            "argz_add",
            "asin",
            "asinh",
            "asprintf",
            "assert_perror",
            "atan",
            "atan2",
            "atanh",
            "atexit",
            "atof",
            "atoi",
            "backtrace",
            "basename",
            "bcmp",
            "bcopy",
            "bind_textdomain_codeset",
            "bindresvport",
            "bindtextdomain",
            "bsd_signal",
            "bsearch",
            "bstring",
            "btowc",
            "btree",
            "byteorder",
            "bzero",
            "cabs",
            "cacos",
            "cacosh",
            "canonicalize_file_name",
            "carg",
            "casin",
            "casinh",
            "catan",
            "catanh",
            "catgets",
            "catopen",
            "cbrt",
            "ccos",
            "ccosh",
            "ceil",
            "cerf",
            "cexp",
            "cexp2",
            "cfree",
            "cimag",
            "clearenv",
            "clock",
            "clock_getcpuclockid",
            "clog",
            "clog10",
            "clog2",
            "closedir",
            "cmsg",
            "confstr",
            "conj",
            "copysign",
            "cos",
            "cosh",
            "cp",
            "cpow",
            "cproj",
            "CPU_SET",
            "creal",
            "crypt",
            "csin",
            "csinh",
            "csqrt",
            "ctan",
            "ctanh",
            "ctermid",
            "ctime",
            "daemon",
            "dbopen",
            "des_crypt",
            "dialog",
            "difftime",
            "dirfd",
            "div",
            "dl_iterate_phdr",
            "dlopen",
            "Dpkg::BuildEnv",
            "Dpkg::BuildFlags",
            "Dpkg::BuildOptions",
            "Dpkg::Changelog",
            "Dpkg::Changelog::Debian",
            "Dpkg::Changelog::Entry",
            "Dpkg::Changelog::Entry::Debian",
            "Dpkg::Changelog::Parse",
            "Dpkg::Checksums",
            "Dpkg::Compression",
            "Dpkg::Compression::FileHandle",
            "Dpkg::Compression::Process",
            "Dpkg::Conf",
            "Dpkg::Control",
            "Dpkg::Control::Changelog",
            "Dpkg::Control::Fields",
            "Dpkg::Control::Hash",
            "Dpkg::Control::Info",
            "Dpkg::Control::Types",
            "Dpkg::Deps",
            "Dpkg::Index",
            "Dpkg::Interface::Storable",
            "Dpkg::IPC",
            "Dpkg::Path",
            "Dpkg::Source::Package",
            "Dpkg::Substvars",
            "Dpkg::Vendor",
            "Dpkg::Vendor::Debian",
            "Dpkg::Vendor::Default",
            "Dpkg::Vendor::Ubuntu",
            "Dpkg::Version",
            "dprintf",
            "drand48",
            "drand48_r",
            "dysize",
            "ecvt",
            "ecvt_r",
            "encrypt",
            "end",
            "endian",
            "envz_add",
            "erf",
            "erfc",
            "err",
            "errno",
            "error",
            "ether_aton",
            "euidaccess",
            "exec",
            "exit",
            "exp",
            "exp10",
            "exp2",
            "expm1",
            "fabs",
            "fclose",
            "fcloseall",
            "fdim",
            "fenv",
            "ferror",
            "fexecve",
            "fflush",
            "ffs",
            "fgetgrent",
            "fgetpwent",
            "fgetwc",
            "fgetws",
            "finite",
            "flockfile",
            "floor",
            "fma",
            "fmax",
            "fmemopen",
            "fmin",
            "fmod",
            "fmtmsg",
            "fnmatch",
            "fopen",
            "fopencookie",
            "fpathconf",
            "fpclassify",
            "fpurge",
            "fputwc",
            "fputws",
            "fread",
            "frexp",
            "fseek",
            "fseeko",
            "ftime",
            "ftok",
            "fts",
            "ftw",
            "futimes",
            "fwide",
            "gamma",
            "gcvt",
            "gdbm",
            "get_nprocs_conf",
            "getaddrinfo",
            "getaddrinfo_a",
            "getauxval",
            "getcwd",
            "getdate",
            "getdirentries",
            "getenv",
            "getfsent",
            "getgrent",
            "getgrent_r",
            "getgrnam",
            "getgrouplist",
            "gethostbyname",
            "gethostid",
            "getifaddrs",
            "getipnodebyname",
            "getline",
            "getloadavg",
            "getlogin",
            "getmntent",
            "getnameinfo",
            "getnetent",
            "getnetent_r",
            "getopt",
            "getpass",
            "getprotoent",
            "getprotoent_r",
            "getpt",
            "getpw",
            "getpwent",
            "getpwent_r",
            "getpwnam",
            "getrpcent",
            "getrpcent_r",
            "getrpcport",
            "gets",
            "getservent",
            "getservent_r",
            "getspnam",
            "getsubopt",
            "gettext",
            "getttyent",
            "getumask",
            "getusershell",
            "getutent",
            "getutmp",
            "getw",
            "getwchar",
            "glob",
            "gnu_get_libc_version",
            "grantpt",
            "gsignal",
            "hash",
            "hsearch",
            "hypot",
            "iconv",
            "iconv_close",
            "iconv_open",
            "ilogb",
            "index",
            "inet",
            "inet_ntop",
            "inet_pton",
            "INFINITY",
            "initgroups",
            "insque",
            "intro",
            "isalpha",
            "isatty",
            "isgreater",
            "iswalnum",
            "iswalpha",
            "iswblank",
            "iswcntrl",
            "iswctype",
            "iswdigit",
            "iswgraph",
            "iswlower",
            "iswprint",
            "iswpunct",
            "iswspace",
            "iswupper",
            "iswxdigit",
            "j0",
            "key_setsecret",
            "ldexp",
            "lgamma",
            "libnetlink",
            "librsync",
            "lio_listio",
            "localeconv",
            "lockf",
            "log",
            "log10",
            "log1p",
            "log2",
            "logb",
            "login",
            "longjmp",
            "lrint",
            "lround",
            "lsearch",
            "lseek64",
            "makecontext",
            "makedev",
            "mallinfo",
            "malloc",
            "malloc_get_state",
            "malloc_hook",
            "malloc_info",
            "malloc_stats",
            "malloc_trim",
            "malloc_usable_size",
            "mallopt",
            "matherr",
            "MB_CUR_MAX",
            "MB_LEN_MAX",
            "mblen",
            "mbrlen",
            "mbrtowc",
            "mbsinit",
            "mbsnrtowcs",
            "mbsrtowcs",
            "mbstowcs",
            "mbtowc",
            "mcheck",
            "memccpy",
            "memchr",
            "memcmp",
            "memcpy",
            "memfrob",
            "memmem",
            "memmove",
            "mempcpy",
            "memset",
            "mkdtemp",
            "mkfifo",
            "mkfifoat",
            "mkstemp",
            "mktemp",
            "modf",
            "mpool",
            "mq_close",
            "mq_getattr",
            "mq_notify",
            "mq_open",
            "mq_receive",
            "mq_send",
            "mq_unlink",
            "mtrace",
            "nan",
            "netlink",
            "nextafter",
            "ngettext",
            "nl_langinfo",
            "offsetof",
            "on_exit",
            "opendir",
            "openpty",
            "pcrepattern",
            "perror",
            "popen",
            "posix_fallocate",
            "posix_memalign",
            "posix_openpt",
            "pow",
            "pow10",
            "profil",
            "program_invocation_name",
            "psignal",
            "pthread_attr_init",
            "pthread_attr_setaffinity_np",
            "pthread_attr_setdetachstate",
            "pthread_attr_setguardsize",
            "pthread_attr_setinheritsched",
            "pthread_attr_setschedparam",
            "pthread_attr_setschedpolicy",
            "pthread_attr_setscope",
            "pthread_attr_setstack",
            "pthread_attr_setstackaddr",
            "pthread_attr_setstacksize",
            "pthread_cancel",
            "pthread_cleanup_push",
            "pthread_cleanup_push_defer_np",
            "pthread_create",
            "pthread_detach",
            "pthread_equal",
            "pthread_exit",
            "pthread_getattr_np",
            "pthread_getcpuclockid",
            "pthread_join",
            "pthread_kill",
            "pthread_kill_other_threads_np",
            "pthread_self",
            "pthread_setaffinity_np",
            "pthread_setcancelstate",
            "pthread_setconcurrency",
            "pthread_setschedparam",
            "pthread_setschedprio",
            "pthread_sigmask",
            "pthread_sigqueue",
            "pthread_testcancel",
            "pthread_tryjoin_np",
            "pthread_yield",
            "ptsname",
            "putenv",
            "putgrent",
            "putpwent",
            "puts",
            "putwchar",
            "qecvt",
            "qsort",
            "queue",
            "raise",
            "rand",
            "random",
            "random_r",
            "rcmd",
            "re_comp",
            "readdir",
            "realpath",
            "recno",
            "redland",
            "regex",
            "remainder",
            "remove",
            "remquo",
            "resolver",
            "rewinddir",
            "rexec",
            "rint",
            "round",
            "rpc",
            "rpmatch",
            "rtime",
            "rtnetlink",
            "scalb",
            "scalbln",
            "scandir",
            "scandirat",
            "scanf",
            "sched_getcpu",
            "seekdir",
            "sem_close",
            "sem_destroy",
            "sem_getvalue",
            "sem_init",
            "sem_open",
            "sem_post",
            "sem_unlink",
            "sem_wait",
            "setaliasent",
            "setbuf",
            "setenv",
            "setjmp",
            "setlocale",
            "setlogmask",
            "setnetgrent",
            "shm_open",
            "siginterrupt",
            "signbit",
            "significand",
            "sigpause",
            "sigqueue",
            "sigset",
            "sigsetops",
            "sigvec",
            "sigwait",
            "sin",
            "sincos",
            "sinh",
            "sleep",
            "sockatmark",
            "sqrt",
            "statvfs",
            "stdarg",
            "stdin",
            "stdio",
            "stdio_ext",
            "stpcpy",
            "stpncpy",
            "strcasecmp",
            "strcat",
            "strchr",
            "strcmp",
            "strcoll",
            "strcpy",
            "strdup",
            "strerror",
            "strfmon",
            "strfry",
            "strftime",
            "string",
            "strlen",
            "strnlen",
            "strpbrk",
            "strptime",
            "strsep",
            "strsignal",
            "strspn",
            "strstr",
            "strtod",
            "strtoimax",
            "strtok",
            "strtol",
            "strtoul",
            "strverscmp",
            "strxfrm",
            "swab",
            "sysconf",
            "syslog",
            "system",
            "sysv_signal",
            "tan",
            "tanh",
            "tcgetpgrp",
            "tcgetsid",
            "telldir",
            "tempnam",
            "termios",
            "textdomain",
            "tgamma",
            "timegm",
            "timeradd",
            "tmpfile",
            "tmpnam",
            "toascii",
            "toupper",
            "towctrans",
            "towlower",
            "towupper",
            "trunc",
            "tsearch",
            "ttyname",
            "ttyslot",
            "tzset",
            "ualarm",
            "ulimit",
            "undocumented",
            "ungetwc",
            "unlocked_stdio",
            "unlockpt",
            "updwtmp",
            "usleep",
            "wcpcpy",
            "wcpncpy",
            "wcrtomb",
            "wcscasecmp",
            "wcscat",
            "wcschr",
            "wcscmp",
            "wcscpy",
            "wcscspn",
            "wcsdup",
            "wcslen",
            "wcsncasecmp",
            "wcsncat",
            "wcsncmp",
            "wcsncpy",
            "wcsnlen",
            "wcsnrtombs",
            "wcspbrk",
            "wcsrchr",
            "wcsrtombs",
            "wcsspn",
            "wcsstr",
            "wcstoimax",
            "wcstok",
            "wcstombs",
            "wcswidth",
            "wctob",
            "wctomb",
            "wctrans",
            "wctype",
            "wcwidth",
            "wmemchr",
            "wmemcmp",
            "wmemcpy",
            "wmemmove",
            "wmemset",
            "wordexp",
            "wprintf",
            "xcrypt",
            "xdr",
            "y0",
            "zlib",
        }
    },
}
;
