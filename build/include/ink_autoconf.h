/* include/ink_autoconf.h.  Generated from ink_autoconf.h.in by configure.  */
/* include/ink_autoconf.h.in.  Generated from configure.ac by autoheader.  */


#pragma once


/* Added in OpenSSL 1.1 */
/* #undef ASN1_STRING_get0_data */

/* Added in OpenSSL 1.1 */
/* #undef BIO_get_data */

/* Added in OpenSSL 1.1 */
/* #undef BIO_get_shutdown */

/* Added in OpenSSL 1.1 */
/* #undef BIO_meth_get_create */

/* Added in OpenSSL 1.1 */
/* #undef BIO_meth_get_ctrl */

/* Added in OpenSSL 1.1 */
/* #undef BIO_meth_get_destroy */

/* Added in OpenSSL 1.1 */
/* #undef BIO_set_data */

/* Renamed in OpenSSL 1.1 */
/* #undef EVP_MD_CTX_free */

/* Renamed in OpenSSL 1.1 */
/* #undef EVP_MD_CTX_new */

/* Renamed in OpenSSL 1.1 */
/* #undef EVP_MD_CTX_reset */

/* Define if libcurl >= 7.19. */
#define HAS_CURL 1

/* Define to 1 if you have the `accept4' function. */
#define HAVE_ACCEPT4 1

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the <arpa/nameser_compat.h> header file. */
#define HAVE_ARPA_NAMESER_COMPAT_H 1

/* Define to 1 if you have the <arpa/nameser.h> header file. */
#define HAVE_ARPA_NAMESER_H 1

/* Define to 1 if you have the `ASYNC_init_thread' function. */
#define HAVE_ASYNC_INIT_THREAD 1

/* Define to 1 if you have '__atomic' functions. */
#define HAVE_ATOMIC 1

/* Define to 1 if you have the `BIO_meth_new' function. */
#define HAVE_BIO_METH_NEW 1

/* Define to 1 if you have the `BIO_sock_non_fatal_error' function. */
#define HAVE_BIO_SOCK_NON_FATAL_ERROR 1

/* Define to 1 if you have the <brotli/encode.h> header file. */
/* #undef HAVE_BROTLI_ENCODE_H */

/* Define to 1 if you have the <cjose/cjose.h> header file. */
/* #undef HAVE_CJOSE_CJOSE_H */

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have the <cpio.h> header file. */
#define HAVE_CPIO_H 1

/* Define to 1 if you have the `CRYPTO_set_mem_functions' function. */
#define HAVE_CRYPTO_SET_MEM_FUNCTIONS 1

/* Define to 1 if you have the <ctype.h> header file. */
#define HAVE_CTYPE_H 1

/* Define to 1 if a SysV or X/Open compatible Curses library is present */
#define HAVE_CURSES 1

/* Define to 1 if library supports color (enhanced functions) */
#define HAVE_CURSES_COLOR 1

/* Define to 1 if library supports X/Open Enhanced functions */
#define HAVE_CURSES_ENHANCED 1

/* Define to 1 if <curses.h> is present */
/* #undef HAVE_CURSES_H */

/* Define to 1 if library supports certain obsolete features */
#define HAVE_CURSES_OBSOLETE 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <editline/readline.h> header file. */
/* #undef HAVE_EDITLINE_READLINE_H */

/* Define to 1 if you have the <endian.h> header file. */
#define HAVE_ENDIAN_H 1

/* Define to 1 if you have the `epoll_ctl' function. */
#define HAVE_EPOLL_CTL 1

/* Define to 1 if you have the `ERR_get_error_all' function. */
#define HAVE_ERR_GET_ERROR_ALL 1

/* Define to 1 if you have the `eventfd' function. */
#define HAVE_EVENTFD 1

/* Define to 1 if you have the <execinfo.h> header file. */
#define HAVE_EXECINFO_H 1

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the <GeoIP.h> header file. */
/* #undef HAVE_GEOIP_H */

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `getpeereid' function. */
/* #undef HAVE_GETPEEREID */

/* Define to 1 if you have the `getpeerucred' function. */
/* #undef HAVE_GETPEERUCRED */

/* Define to 1 if you have the `getresgid' function. */
#define HAVE_GETRESGID 1

/* Define to 1 if you have the `getresuid' function. */
#define HAVE_GETRESUID 1

/* Define to 1 if you have the `getreuid' function. */
/* #undef HAVE_GETREUID */

/* Define to 1 if you have the <gperftools/profiler.h> header file. */
/* #undef HAVE_GPERFTOOLS_PROFILER_H */

/* Define to 1 if you have the `hcreate_r' function. */
/* #undef HAVE_HCREATE_R */

/* Define to 1 if you have the <hiredis/hiredis.h> header file. */
/* #undef HAVE_HIREDIS_HIREDIS_H */

/* Define to 1 if you have the <history.h> header file. */
/* #undef HAVE_HISTORY_H */

/* Define to 1 if you have the `HMAC_CTX_new' function. */
#define HAVE_HMAC_CTX_NEW 1

/* Define to 1 if you have the `hsearch_r' function. */
/* #undef HAVE_HSEARCH_R */

/* Whether HWLOC_OBJ_PU is available */
#define HAVE_HWLOC_OBJ_PU 1

/* Define to 1 if you have the <ifaddrs.h> header file. */
#define HAVE_IFADDRS_H 1

/* Define to 1 if you have the `inotify_init' function. */
#define HAVE_INOTIFY_INIT 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <jansson.h> header file. */
/* #undef HAVE_JANSSON_H */

/* Define to 1 if you have the <jemalloc/jemalloc.h> header file. */
/* #undef HAVE_JEMALLOC_JEMALLOC_H */

/* Define to 1 if you have the <kclangc.h> header file. */
/* #undef HAVE_KCLANGC_H */

/* Define to 1 if you have the `kqueue' function. */
/* #undef HAVE_KQUEUE */

/* Define to 1 if you have the <libaio.h> header file. */
/* #undef HAVE_LIBAIO_H */

/* Define to 1 if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Compiling with pcre support */
#define HAVE_LIBPCRE 1

/* Define if you have a readline compatible library */
/* #undef HAVE_LIBREADLINE */

/* Define to 1 if you have the <linux/hdreg.h> header file. */
#define HAVE_LINUX_HDREG_H 1

/* Define to 1 if you have the <linux/major.h> header file. */
#define HAVE_LINUX_MAJOR_H 1

/* Define to 1 if you have the <luajit.h> header file. */
/* #undef HAVE_LUAJIT_H */

/* Define to 1 if you have the <lzma.h> header file. */
#define HAVE_LZMA_H 1

/* Define to 1 if you have the <machine/endian.h> header file. */
/* #undef HAVE_MACHINE_ENDIAN_H */

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the `malloc_usable_size' function. */
#define HAVE_MALLOC_USABLE_SIZE 1

/* Define to 1 if you have the <maxminddb.h> header file. */
/* #undef HAVE_MAXMINDDB_H */

/* Define to 1 if you have the <mcheck.h> header file. */
#define HAVE_MCHECK_H 1

/* Define to 1 if you have the `mcheck_pedantic' function. */
#define HAVE_MCHECK_PEDANTIC 1

/* Define to 1 if you have the `MD5_Init' function. */
#define HAVE_MD5_INIT 1

/* Define to 1 if you have the <mimalloc.h> header file. */
/* #undef HAVE_MIMALLOC_H */

/* Define to 1 if you have the <mysql/mysql.h> header file. */
/* #undef HAVE_MYSQL_MYSQL_H */

/* Define to 1 if the Ncurses library is present */
/* #undef HAVE_NCURSES */

/* Define to 1 if the NcursesW library is present */
#define HAVE_NCURSESW 1

/* Define to 1 if <ncursesw/curses.h> is present */
#define HAVE_NCURSESW_CURSES_H 1

/* Define to 1 if <ncursesw.h> is present */
/* #undef HAVE_NCURSESW_H */

/* Define to 1 if <ncurses/curses.h> is present */
/* #undef HAVE_NCURSES_CURSES_H */

/* Define to 1 if <ncurses.h> is present */
#define HAVE_NCURSES_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/in_systm.h> header file. */
#define HAVE_NETINET_IN_SYSTM_H 1

/* Define to 1 if you have the <netinet/ip.h> header file. */
#define HAVE_NETINET_IP_H 1

/* Define to 1 if you have the <netinet/ip_icmp.h> header file. */
#define HAVE_NETINET_IP_ICMP_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <net/ppp_defs.h> header file. */
#define HAVE_NET_PPP_DEFS_H 1

/* Define to 1 if you have the <openssl/ssl.h> header file. */
#define HAVE_OPENSSL_SSL_H 1

/* Define to 1 if you have the <openssl/tls1.h> header file. */
#define HAVE_OPENSSL_TLS1_H 1

/* Define to 1 if you have the <openssl/ts.h> header file. */
#define HAVE_OPENSSL_TS_H 1

/* Define to 1 if you have the <opentelemetry/trace/provider.h> header file.
   */
/* #undef HAVE_OPENTELEMETRY_TRACE_PROVIDER_H */

/* Define to 1 if you have the <pcre.h> header file. */
#define HAVE_PCRE_H 1

/* Define to 1 if you have the <pcre/pcre.h> header file. */
/* #undef HAVE_PCRE_PCRE_H */

/* Define to 1 if you have the `port_create' function. */
/* #undef HAVE_PORT_CREATE */

/* Define to 1 if you have the `posix_fadvise' function. */
#define HAVE_POSIX_FADVISE 1

/* Define to 1 if you have the `posix_fallocate' function. */
#define HAVE_POSIX_FALLOCATE 1

/* Define to 1 if you have the `posix_madvise' function. */
#define HAVE_POSIX_MADVISE 1

/* Define to 1 if you have the `psiginfo' function. */
#define HAVE_PSIGINFO 1

/* Define to 1 if you have the `psignal' function. */
#define HAVE_PSIGNAL 1

/* Whether pthread_getname_np() is available */
#define HAVE_PTHREAD_GETNAME_NP 1

/* Whether pthread_get_name_np() is available */
/* #undef HAVE_PTHREAD_GET_NAME_NP */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the `pthread_mutexattr_settype' function. */
#define HAVE_PTHREAD_MUTEXATTR_SETTYPE 1

/* Define to 1 if you have the <pthread_np.h> header file. */
/* #undef HAVE_PTHREAD_NP_H */

/* Whether the 1 parameter version of pthread_setname_np() is available */
/* #undef HAVE_PTHREAD_SETNAME_NP_1 */

/* Whether the 2 parameter version of pthread_setname_np() is available */
#define HAVE_PTHREAD_SETNAME_NP_2 1

/* Whether the 2 parameter version of pthread_set_name_np() is available */
/* #undef HAVE_PTHREAD_SET_NAME_NP_2 */

/* Define to 1 if you have the <readline.h> header file. */
/* #undef HAVE_READLINE_H */

/* Define if your readline library has \`add_history' */
/* #undef HAVE_READLINE_HISTORY */

/* Define to 1 if you have the <readline/history.h> header file. */
/* #undef HAVE_READLINE_HISTORY_H */

/* Define to 1 if you have the <readline/readline.h> header file. */
/* #undef HAVE_READLINE_READLINE_H */

/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H 1

/* Define to 1 if you have the <search.h> header file. */
/* #undef HAVE_SEARCH_H */

/* Define to 1 if you have the `setresuid' function. */
#define HAVE_SETRESUID 1

/* Define to 1 if you have the `setreuid' function. */
#define HAVE_SETREUID 1

/* Define to 1 if you have the `SHA1' function. */
#define HAVE_SHA1 1

/* Define to 1 if you have the `SHA256_Init' function. */
#define HAVE_SHA256_INIT 1

/* Define to 1 if you have the <siginfo.h> header file. */
/* #undef HAVE_SIGINFO_H */

/* Define to 1 if you have the `SSL_CTX_set_ciphersuites' function. */
#define HAVE_SSL_CTX_SET_CIPHERSUITES 1

/* Define to 1 if you have the `SSL_CTX_set_keylog_callback' function. */
#define HAVE_SSL_CTX_SET_KEYLOG_CALLBACK 1

/* Define to 1 if you have the `SSL_CTX_set_tlsext_ticket_key_cb' function. */
#define HAVE_SSL_CTX_SET_TLSEXT_TICKET_KEY_CB 1

/* Define to 1 if you have the `SSL_CTX_set_tlsext_ticket_key_evp_cb'
   function. */
#define HAVE_SSL_CTX_SET_TLSEXT_TICKET_KEY_EVP_CB 1

/* Define to 1 if you have the `SSL_get_all_async_fds' function. */
#define HAVE_SSL_GET_ALL_ASYNC_FDS 1

/* Define to 1 if you have the `SSL_SESSION_dup' function. */
#define HAVE_SSL_SESSION_DUP 1

/* Define to 1 if you have the `SSL_set1_verify_cert_store' function. */
/* #undef HAVE_SSL_SET1_VERIFY_CERT_STORE */

/* Define to 1 if you have the `SSL_set_max_early_data' function. */
#define HAVE_SSL_SET_MAX_EARLY_DATA 1

/* Define to 1 if you have the `SSL_set_quic_early_data_enabled' function. */
/* #undef HAVE_SSL_SET_QUIC_EARLY_DATA_ENABLED */

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
#define HAVE_STRLCAT 1

/* Define to 1 if you have the `strlcpy' function. */
#define HAVE_STRLCPY 1

/* Define to 1 if you have the <stropts.h> header file. */
/* #undef HAVE_STROPTS_H */

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Whether struct tcp_info have the tcpi_data_segs_{in,out} member */
#define HAVE_STRUCT_LINUX_TCP_INFO 1

/* Whether struct sockaddr_in6 has the sin6_len member */
/* #undef HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN */

/* Whether struct sockaddr_in has the sin_len member */
/* #undef HAVE_STRUCT_SOCKADDR_IN_SIN_LEN */

/* Whether struct sockaddr_in has the sa_len member */
/* #undef HAVE_STRUCT_SOCKADDR_SA_LEN */

/* Define to 1 if `st_mtimespec.tv_nsec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC */

/* Define to 1 if `st_mtim.tv_nsec' is a member of `struct stat'. */
#define HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC 1

/* whether struct tcp_info is available */
#define HAVE_STRUCT_TCP_INFO 1

/* Define to 1 if you have the `sysconf' function. */
#define HAVE_SYSCONF 1

/* Define to 1 if you have the `sysctlbyname' function. */
/* #undef HAVE_SYSCTLBYNAME */

/* Define to 1 if you have the <sysexits.h> header file. */
#define HAVE_SYSEXITS_H 1

/* Define to 1 if using probes. */
/* #undef HAVE_SYSTEMTAP */

/* Define to 1 if you have the <sys/byteorder.h> header file. */
/* #undef HAVE_SYS_BYTEORDER_H */

/* Define to 1 if you have the <sys/capability.h> header file. */
#define HAVE_SYS_CAPABILITY_H 1

/* Define to 1 if you have the <sys/cpuset.h> header file. */
/* #undef HAVE_SYS_CPUSET_H */

/* Define to 1 if you have the <sys/disklabel.h> header file. */
/* #undef HAVE_SYS_DISKLABEL_H */

/* Define to 1 if you have the <sys/disk.h> header file. */
/* #undef HAVE_SYS_DISK_H */

/* Define to 1 if you have the <sys/endian.h> header file. */
/* #undef HAVE_SYS_ENDIAN_H */

/* Define to 1 if you have the <sys/epoll.h> header file. */
#define HAVE_SYS_EPOLL_H 1

/* Define to 1 if you have the <sys/eventfd.h> header file. */
#define HAVE_SYS_EVENTFD_H 1

/* Define to 1 if you have the <sys/event.h> header file. */
/* #undef HAVE_SYS_EVENT_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/mount.h> header file. */
#define HAVE_SYS_MOUNT_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/prctl.h> header file. */
#define HAVE_SYS_PRCTL_H 1

/* Define to 1 if you have the <sys/pset.h> header file. */
/* #undef HAVE_SYS_PSET_H */

/* Define to 1 if you have the <sys/sockio.h> header file. */
/* #undef HAVE_SYS_SOCKIO_H */

/* Define to 1 if you have the <sys/statfs.h> header file. */
#define HAVE_SYS_STATFS_H 1

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#define HAVE_SYS_STATVFS_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysctl.h> header file. */
/* #undef HAVE_SYS_SYSCTL_H */

/* Define to 1 if you have the <sys/sysinfo.h> header file. */
#define HAVE_SYS_SYSINFO_H 1

/* Define to 1 if you have the <sys/sysmacros.h> header file. */
#define HAVE_SYS_SYSMACROS_H 1

/* Define to 1 if you have the <sys/systeminfo.h> header file. */
/* #undef HAVE_SYS_SYSTEMINFO_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <ucred.h> header file. */
/* #undef HAVE_UCRED_H */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <values.h> header file. */
#define HAVE_VALUES_H 1

/* Define to 1 if you have the `X509_get0_signature' function. */
#define HAVE_X509_GET0_SIGNATURE 1

/* Define to 1 if you have the <zlib.h> header file. */
#define HAVE_ZLIB_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "trafficserver"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "dev@trafficserver.apache.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Apache Traffic Server"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Apache Traffic Server 9.2.4"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "trafficserver"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://trafficserver.apache.org"

/* Define to the version of this package. */
#define PACKAGE_VERSION "9.2.4"

/* The size of `void*', as computed by sizeof. */
#define SIZEOF_VOIDP 8

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "9.2.4"

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1
