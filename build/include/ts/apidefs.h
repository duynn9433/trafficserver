/** @file

  This header file defines common types that can be shared
  between internal code and TS APIs.

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  @section developers Developers

  Developers, when adding a new element to an enum, append it. DO NOT
  insert it.  Otherwise, binary compatibility of plugins will be broken!

 */

#pragma once

/*
 * ATS Plugin just needs to include <ts/ts.h>,
 * should not include this file directly!
 *
 */

/* GENERATED FILE WARNING!  DO NOT EDIT apidefs.h
 *
 * You must modify apidefs.h.in instead.
 *
 */

#include <stdbool.h>
#include <stdint.h> // NOLINT(modernize-deprecated-headers)
#include <sys/types.h>
#include <sys/socket.h>

#ifndef tsapi
#define tsapi
#endif

/** Apply printf format string compile-time argument checking to a function.
 *
 *
 * For example, given the following function from DiagsTypes.h:
 *
 * @code
 * class Diags {
 *
 * ...
 *
 * void
 * print(
 *     const char *tag,
 *     DiagsLevel level,
 *     const SourceLocation *loc,
 *     const char *fmt,
 *     ...) const;
 *
 * ...
 *
 * };
 * @endcode
 *
 * This macro can be used to apply compiler checking for ... against the fmt
 * argument like so:
 *
 *
 * @code
 * class Diags {
 *
 * ...
 *
 * void
 * print(
 *     const char *tag,
 *     DiagsLevel level,
 *     const SourceLocation *loc,
 *     const char *fmt,
 *     ...) const TS_PRINTFLIKE(5, 6);
 *
 * ...
 *
 * };
 * @endcode
 *
 * Note in this case, (5, 6) rather than (4, 5) is passed because `this`
 * counts as the implicit first parameter of this member function.
 *
 * @param[in] fmt_index The index of the format string argument, with argument
 * indexing starting at 1.
 *
 * @param[in] arg_index The index of the first format argument string, with
 * argument indexing starting at 1.
 */
#if !defined(TS_PRINTFLIKE)
#if defined(__GNUC__) || defined(__clang__)
#define TS_PRINTFLIKE(fmt_index, arg_index) __attribute__((format(printf, fmt_index, arg_index)))
#else
#define TS_PRINTFLIKE(fmt_index, arg_index)
#endif
#endif

#if !defined(TS_NORETURN)
#if defined(__GNUC__) || defined(__clang__)
#define TS_NORETURN __attribute__((noreturn))
#else
#define TS_NORETURN
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Version info
 */
/* clang-format off */
#define TS_VERSION_STRING "9.2.4"
#define TS_VERSION_NUMBER 9002004
#define TS_VERSION_MAJOR 9
#define TS_VERSION_MINOR 2
#define TS_VERSION_MICRO 4
/* clang-format on */

#define TS_HTTP_VERSION(a, b) ((((a)&0xFFFF) << 16) | ((b)&0xFFFF))
#define TS_HTTP_MINOR(v) ((v)&0xFFFF)
#define TS_HTTP_MAJOR(v) (((v) >> 16) & 0xFFFF)

#ifndef TS_RES_MEM_PATH
#define __TS_RES_PATH(x) #x
#define _TS_RES_PATH(x) __TS_RES_PATH(x)
#define TS_RES_PATH(x) x __FILE__ ":" _TS_RES_PATH(__LINE__)
#define TS_RES_MEM_PATH TS_RES_PATH("memory/")
#endif

#ifndef TS_DEPRECATED
#define TS_DEPRECATED __attribute__((deprecated))
#endif

/**
    The following struct is used by TSPluginRegister(). It stores
    registration information about the plugin.

 */
typedef struct {
  const char *plugin_name;
  const char *vendor_name;
  const char *support_email;
} TSPluginRegistrationInfo;

typedef struct {
  const char *tag;  ///< Message tag (null terminated).
  void const *data; ///< Message data (payload)
  size_t data_size; ///< Amount of message data.
} TSPluginMsg;

/**
    This set of enums are possible values returned by
    TSHttpHdrParseReq() and TSHttpHdrParseResp().

 */
typedef enum {
  TS_PARSE_ERROR = -1,
  TS_PARSE_DONE  = 0,
  TS_PARSE_CONT  = 1,
} TSParseResult;

/**
    This set of enums represents the possible HTTP types that
    can be assigned to an HTTP header. When a header is created
    with TSHttpHdrCreate(), it is automatically assigned a type of
    TS_HTTP_TYPE_UNKNOWN. You can modify the HTTP type ONCE after it
    the header is created, using TSHttpHdrTypeSet(). After setting the
    HTTP type once, you cannot set it again. Use TSHttpHdrTypeGet()
    to obtain the TSHttpType of an HTTP header.

 */
typedef enum {
  TS_HTTP_TYPE_UNKNOWN,
  TS_HTTP_TYPE_REQUEST,
  TS_HTTP_TYPE_RESPONSE,
} TSHttpType;

/**
    This set of enums represents possible return values from
    TSHttpHdrStatusGet(), which retrieves the status code from an
    HTTP response header (TSHttpHdrStatusGet() retrieves status codes
    only from headers of type TS_HTTP_TYPE_RESPONSE). You can also set
    the TSHttpStatus of a response header using TSHttpHdrStatusSet().

 */
typedef enum {
  TS_HTTP_STATUS_NONE                            = 0,
  TS_HTTP_STATUS_CONTINUE                        = 100,
  TS_HTTP_STATUS_SWITCHING_PROTOCOL              = 101,
  TS_HTTP_STATUS_EARLY_HINTS                     = 103,
  TS_HTTP_STATUS_OK                              = 200,
  TS_HTTP_STATUS_CREATED                         = 201,
  TS_HTTP_STATUS_ACCEPTED                        = 202,
  TS_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION   = 203,
  TS_HTTP_STATUS_NO_CONTENT                      = 204,
  TS_HTTP_STATUS_RESET_CONTENT                   = 205,
  TS_HTTP_STATUS_PARTIAL_CONTENT                 = 206,
  TS_HTTP_STATUS_MULTI_STATUS                    = 207,
  TS_HTTP_STATUS_ALREADY_REPORTED                = 208,
  TS_HTTP_STATUS_IM_USED                         = 211,
  TS_HTTP_STATUS_MULTIPLE_CHOICES                = 300,
  TS_HTTP_STATUS_MOVED_PERMANENTLY               = 301,
  TS_HTTP_STATUS_MOVED_TEMPORARILY               = 302,
  TS_HTTP_STATUS_SEE_OTHER                       = 303,
  TS_HTTP_STATUS_NOT_MODIFIED                    = 304,
  TS_HTTP_STATUS_USE_PROXY                       = 305,
  TS_HTTP_STATUS_TEMPORARY_REDIRECT              = 307,
  TS_HTTP_STATUS_PERMANENT_REDIRECT              = 308,
  TS_HTTP_STATUS_BAD_REQUEST                     = 400,
  TS_HTTP_STATUS_UNAUTHORIZED                    = 401,
  TS_HTTP_STATUS_PAYMENT_REQUIRED                = 402,
  TS_HTTP_STATUS_FORBIDDEN                       = 403,
  TS_HTTP_STATUS_NOT_FOUND                       = 404,
  TS_HTTP_STATUS_METHOD_NOT_ALLOWED              = 405,
  TS_HTTP_STATUS_NOT_ACCEPTABLE                  = 406,
  TS_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED   = 407,
  TS_HTTP_STATUS_REQUEST_TIMEOUT                 = 408,
  TS_HTTP_STATUS_CONFLICT                        = 409,
  TS_HTTP_STATUS_GONE                            = 410,
  TS_HTTP_STATUS_LENGTH_REQUIRED                 = 411,
  TS_HTTP_STATUS_PRECONDITION_FAILED             = 412,
  TS_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE        = 413,
  TS_HTTP_STATUS_REQUEST_URI_TOO_LONG            = 414,
  TS_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE          = 415,
  TS_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
  TS_HTTP_STATUS_EXPECTATION_FAILED              = 417,
  TS_HTTP_STATUS_UNPROCESSABLE_ENTITY            = 422,
  TS_HTTP_STATUS_LOCKED                          = 423,
  TS_HTTP_STATUS_FAILED_DEPENDENCY               = 424,
  TS_HTTP_STATUS_UPGRADE_REQUIRED                = 426,
  TS_HTTP_STATUS_PRECONDITION_REQUIRED           = 428,
  TS_HTTP_STATUS_TOO_MANY_REQUESTS               = 429,
  TS_HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
  TS_HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS   = 451,
  TS_HTTP_STATUS_INTERNAL_SERVER_ERROR           = 500,
  TS_HTTP_STATUS_NOT_IMPLEMENTED                 = 501,
  TS_HTTP_STATUS_BAD_GATEWAY                     = 502,
  TS_HTTP_STATUS_SERVICE_UNAVAILABLE             = 503,
  TS_HTTP_STATUS_GATEWAY_TIMEOUT                 = 504,
  TS_HTTP_STATUS_HTTPVER_NOT_SUPPORTED           = 505,
  TS_HTTP_STATUS_VARIANT_ALSO_NEGOTIATES         = 506,
  TS_HTTP_STATUS_INSUFFICIENT_STORAGE            = 507,
  TS_HTTP_STATUS_LOOP_DETECTED                   = 508,
  TS_HTTP_STATUS_NOT_EXTENDED                    = 510,
  TS_HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED = 511
} TSHttpStatus;

/**
    This set of enums represents the possible hooks where you can
    set up continuation callbacks. The functions used to register a
    continuation for a particular hook are:

    TSHttpHookAdd: adds a global hook. You can globally add
    any hook except for
     - TS_HTTP_REQUEST_TRANSFORM_HOOK
     - TS_HTTP_RESPONSE_TRANSFORM_HOOK
     - TS_HTTP_RESPONSE_CLIENT_HOOK

    The following hooks can ONLY be added globally:
     - TS_HTTP_SELECT_ALT_HOOK
     - TS_HTTP_SSN_START_HOOK

    TSHttpSsnHookAdd: adds a transaction hook to each transaction
    within a session. You can only use transaction hooks with this call:
     - TS_HTTP_READ_REQUEST_HDR_HOOK
     - TS_HTTP_OS_DNS_HOOK
     - TS_HTTP_SEND_REQUEST_HDR_HOOK
     - TS_HTTP_READ_CACHE_HDR_HOOK
     - TS_HTTP_READ_RESPONSE_HDR_HOOK
     - TS_HTTP_SEND_RESPONSE_HDR_HOOK
     - TS_HTTP_REQUEST_TRANSFORM_HOOK
     - TS_HTTP_RESPONSE_TRANSFORM_HOOK
     - TS_HTTP_RESPONSE_CLIENT_HOOK
     - TS_HTTP_TXN_START_HOOK
     - TS_HTTP_TXN_CLOSE_HOOK
     - TS_HTTP_SSN_CLOSE_HOOK

    TSHttpTxnHookAdd: adds a callback at a specific point within
    an HTTP transaction. The following hooks can be used with this
    function:
     - TS_HTTP_READ_REQUEST_HDR_HOOK
     - TS_HTTP_OS_DNS_HOOK
     - TS_HTTP_SEND_REQUEST_HDR_HOOK
     - TS_HTTP_READ_CACHE_HDR_HOOK
     - TS_HTTP_READ_RESPONSE_HDR_HOOK
     - TS_HTTP_SEND_RESPONSE_HDR_HOOK
     - TS_HTTP_REQUEST_TRANSFORM_HOOK
     - TS_HTTP_RESPONSE_TRANSFORM_HOOK
     - TS_HTTP_TXN_CLOSE_HOOK

    The two transform hooks can ONLY be added as transaction hooks.

    TS_VCONN_START_HOOK - called just after the connection is created,
    before other activity such as I/O or TLS handshakes  No handshake
    data has been read or sent (from the proxy) at this point

    TS_HTTP_LAST_HOOK _must_ be the last element. Only right place
    to insert a new element is just before TS_HTTP_LAST_HOOK.

 */
typedef enum {
  TS_HTTP_READ_REQUEST_HDR_HOOK,
  TS_HTTP_OS_DNS_HOOK,
  TS_HTTP_SEND_REQUEST_HDR_HOOK,
  TS_HTTP_READ_CACHE_HDR_HOOK,
  TS_HTTP_READ_RESPONSE_HDR_HOOK,
  TS_HTTP_SEND_RESPONSE_HDR_HOOK,
  TS_HTTP_REQUEST_TRANSFORM_HOOK,
  TS_HTTP_RESPONSE_TRANSFORM_HOOK,
  TS_HTTP_SELECT_ALT_HOOK,
  TS_HTTP_TXN_START_HOOK,
  TS_HTTP_TXN_CLOSE_HOOK,
  TS_HTTP_SSN_START_HOOK,
  TS_HTTP_SSN_CLOSE_HOOK,
  TS_HTTP_CACHE_LOOKUP_COMPLETE_HOOK,
  TS_HTTP_PRE_REMAP_HOOK,
  TS_HTTP_POST_REMAP_HOOK,
  TS_HTTP_RESPONSE_CLIENT_HOOK,
  // Putting the SSL hooks in the same enum space
  // So both sets of hooks can be set by the same Hook function
  TS_SSL_FIRST_HOOK,
  TS_VCONN_START_HOOK      = TS_SSL_FIRST_HOOK,
  TS_VCONN_PRE_ACCEPT_HOOK = TS_VCONN_START_HOOK, // Deprecated but compatible for now.
  TS_VCONN_CLOSE_HOOK,
  TS_SSL_CLIENT_HELLO_HOOK,
  TS_SSL_SNI_HOOK,
  TS_SSL_CERT_HOOK = TS_SSL_SNI_HOOK,
  TS_SSL_SERVERNAME_HOOK,
  TS_SSL_SERVER_VERIFY_HOOK,
  TS_SSL_VERIFY_SERVER_HOOK = TS_SSL_SERVER_VERIFY_HOOK,
  TS_SSL_VERIFY_CLIENT_HOOK,
  TS_SSL_SESSION_HOOK,
  TS_VCONN_OUTBOUND_START_HOOK,
  TS_VCONN_OUTBOUND_CLOSE_HOOK,
  TS_SSL_LAST_HOOK = TS_VCONN_OUTBOUND_CLOSE_HOOK,
  TS_HTTP_REQUEST_BUFFER_READ_COMPLETE_HOOK,
  TS_HTTP_LAST_HOOK
} TSHttpHookID;

/** Plugin lifecycle hooks.

    These are called during lifecycle events of a plugin. They
    should be set in the plugin initialization function. The
    continuation is invoked with an event ID specified for each hook
    and @c NULL for the event data.

    TS_LIFECYCLE_PORTS_INITIALIZED_HOOK

      called once, after the HTTP proxy port data structures have
      been initialized. In particular, SSL related calls that depend
      on accept endpoints may be invoked. After this hook is
      finished, the proxy port sockets are opened and connections
      are accepted.

      Event: TS_EVENT_LIFECYCLE_PORTS_INITIALIZED

    TS_LIFECYCLE_PORTS_READY_HOOK

      called once, after the sockets have been opened and the accept
      threads have been started. That is, the ports are ready to
      accept connections. This is *not* guaranteed to be called
      before the first connection is accepted.

      Event: TS_EVENT_LIFECYCLE_PORTS_READY

    TS_LIFECYCLE_CACHE_READY_HOOK

      called once, after the cache has finished its
      initialization. It is either online or has failed when this
      hook is called.

      Event: TS_EVENT_LIFECYCLE_CACHE_READY

    TS_LIFECYCLE_SERVER_SSL_CTX_INITIALIZED_HOOK

      called every time after a server SSL_CTX has finished the initialization.
      It exposes the initialized SSL_CTX pointer.

      Event: TS_EVENT_LIFECYCLE_SERVER_SSL_CTX_INITIALIZED

    TS_LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED_HOOK

      called once, after the client SSL_CTX has finished the initialization.
      It exposes the initialized SSL_CTX pointer.

      Event: TS_EVENT_LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED

    TS_LIFECYCLE_MSG_HOOK

      Called in response to an external agent. The data is a pointer to  an instance of TSPluginMsg.

      Event: TS_EVENT_LIFECYCLE_MSG

    TS_LIFECYCLE_TASK_THREADS_READY_HOOK

      called once, after the task threads have been started.

      Event: TS_EVENT_LIFECYCLE_TASK_THREADS_READY

    TS_LIFECYCLE_SHUTDOWN_HOOK

      called once, after receiving a shutdown signal, such as SIGTERM.

      Event: TS_EVENT_LIFECYCLE_SHUTDOWN

    Ordering guarantees:

    - TS_LIFECYCLE_PORTS_INITIALIZED_HOOK before TS_LIFECYCLE_PORTS_READY_HOOK.

    NOTE! ONLY the orderings EXPLICITLY mentioned above are guaranteed.

 */
typedef enum {
  TS_LIFECYCLE_PORTS_INITIALIZED_HOOK,
  TS_LIFECYCLE_PORTS_READY_HOOK,
  TS_LIFECYCLE_CACHE_READY_HOOK,
  TS_LIFECYCLE_SERVER_SSL_CTX_INITIALIZED_HOOK,
  TS_LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED_HOOK,
  TS_LIFECYCLE_MSG_HOOK,
  TS_LIFECYCLE_TASK_THREADS_READY_HOOK,
  TS_LIFECYCLE_SHUTDOWN_HOOK,
  // TS_LIFECYCLE_SSL_SECRET_HOOK, future release
  TS_LIFECYCLE_LAST_HOOK = TS_LIFECYCLE_SHUTDOWN_HOOK + 2
} TSLifecycleHookID;

/**
    TSEvents are sent to continuations when they are called back.
    The TSEvent provides the continuation's handler function with
    information about the callback. Based on the event it receives,
    the handler function can decide what to do.

 */
typedef enum {
  TS_EVENT_NONE      = 0,
  TS_EVENT_IMMEDIATE = 1,
  TS_EVENT_TIMEOUT   = 2,
  TS_EVENT_ERROR     = 3,
  TS_EVENT_CONTINUE  = 4,

  TS_EVENT_VCONN_READ_READY         = 100,
  TS_EVENT_VCONN_WRITE_READY        = 101,
  TS_EVENT_VCONN_READ_COMPLETE      = 102,
  TS_EVENT_VCONN_WRITE_COMPLETE     = 103,
  TS_EVENT_VCONN_EOS                = 104,
  TS_EVENT_VCONN_INACTIVITY_TIMEOUT = 105,
  TS_EVENT_VCONN_ACTIVE_TIMEOUT     = 106,
  TS_EVENT_VCONN_START              = 107,
  TS_EVENT_VCONN_CLOSE              = 108,
  TS_EVENT_VCONN_OUTBOUND_START     = 109,
  TS_EVENT_VCONN_OUTBOUND_CLOSE     = 110,
  TS_EVENT_VCONN_PRE_ACCEPT         = TS_EVENT_VCONN_START, // Deprecated but still compatible

  TS_EVENT_NET_CONNECT        = 200,
  TS_EVENT_NET_CONNECT_FAILED = 201,
  TS_EVENT_NET_ACCEPT         = 202,
  TS_EVENT_NET_ACCEPT_FAILED  = 204,

  TS_EVENT_INTERNAL_206 = 206,
  TS_EVENT_INTERNAL_207 = 207,
  TS_EVENT_INTERNAL_208 = 208,
  TS_EVENT_INTERNAL_209 = 209,
  TS_EVENT_INTERNAL_210 = 210,
  TS_EVENT_INTERNAL_211 = 211,
  TS_EVENT_INTERNAL_212 = 212,

  TS_EVENT_HOST_LOOKUP = 500,

  TS_EVENT_CACHE_OPEN_READ              = 1102,
  TS_EVENT_CACHE_OPEN_READ_FAILED       = 1103,
  TS_EVENT_CACHE_OPEN_WRITE             = 1108,
  TS_EVENT_CACHE_OPEN_WRITE_FAILED      = 1109,
  TS_EVENT_CACHE_REMOVE                 = 1112,
  TS_EVENT_CACHE_REMOVE_FAILED          = 1113,
  TS_EVENT_CACHE_SCAN                   = 1120,
  TS_EVENT_CACHE_SCAN_FAILED            = 1121,
  TS_EVENT_CACHE_SCAN_OBJECT            = 1122,
  TS_EVENT_CACHE_SCAN_OPERATION_BLOCKED = 1123,
  TS_EVENT_CACHE_SCAN_OPERATION_FAILED  = 1124,
  TS_EVENT_CACHE_SCAN_DONE              = 1125,
  TS_EVENT_CACHE_LOOKUP                 = 1126,
  TS_EVENT_CACHE_READ                   = 1127,
  TS_EVENT_CACHE_DELETE                 = 1128,
  TS_EVENT_CACHE_WRITE                  = 1129,
  TS_EVENT_CACHE_WRITE_HEADER           = 1130,
  TS_EVENT_CACHE_CLOSE                  = 1131,
  TS_EVENT_CACHE_LOOKUP_READY           = 1132,
  TS_EVENT_CACHE_LOOKUP_COMPLETE        = 1133,
  TS_EVENT_CACHE_READ_READY             = 1134,
  TS_EVENT_CACHE_READ_COMPLETE          = 1135,

  TS_EVENT_INTERNAL_1200 = 1200,

  TS_EVENT_SSL_SESSION_GET    = 2000,
  TS_EVENT_SSL_SESSION_NEW    = 2001,
  TS_EVENT_SSL_SESSION_REMOVE = 2002,

  TS_EVENT_AIO_DONE = 3900,

  TS_EVENT_HTTP_CONTINUE                = 60000,
  TS_EVENT_HTTP_ERROR                   = 60001,
  TS_EVENT_HTTP_READ_REQUEST_HDR        = 60002,
  TS_EVENT_HTTP_OS_DNS                  = 60003,
  TS_EVENT_HTTP_SEND_REQUEST_HDR        = 60004,
  TS_EVENT_HTTP_READ_CACHE_HDR          = 60005,
  TS_EVENT_HTTP_READ_RESPONSE_HDR       = 60006,
  TS_EVENT_HTTP_SEND_RESPONSE_HDR       = 60007,
  TS_EVENT_HTTP_REQUEST_TRANSFORM       = 60008,
  TS_EVENT_HTTP_RESPONSE_TRANSFORM      = 60009,
  TS_EVENT_HTTP_SELECT_ALT              = 60010,
  TS_EVENT_HTTP_TXN_START               = 60011,
  TS_EVENT_HTTP_TXN_CLOSE               = 60012,
  TS_EVENT_HTTP_SSN_START               = 60013,
  TS_EVENT_HTTP_SSN_CLOSE               = 60014,
  TS_EVENT_HTTP_CACHE_LOOKUP_COMPLETE   = 60015,
  TS_EVENT_HTTP_PRE_REMAP               = 60016,
  TS_EVENT_HTTP_POST_REMAP              = 60017,
  TS_EVENT_HTTP_REQUEST_BUFFER_COMPLETE = 60018,

  TS_EVENT_LIFECYCLE_PORTS_INITIALIZED          = 60100,
  TS_EVENT_LIFECYCLE_PORTS_READY                = 60101,
  TS_EVENT_LIFECYCLE_CACHE_READY                = 60102,
  TS_EVENT_LIFECYCLE_SERVER_SSL_CTX_INITIALIZED = 60103,
  TS_EVENT_LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED = 60104,
  TS_EVENT_LIFECYCLE_MSG                        = 60105,
  TS_EVENT_LIFECYCLE_TASK_THREADS_READY         = 60106,
  TS_EVENT_LIFECYCLE_SHUTDOWN                   = 60107,

  TS_EVENT_INTERNAL_60200    = 60200,
  TS_EVENT_INTERNAL_60201    = 60201,
  TS_EVENT_INTERNAL_60202    = 60202,
  TS_EVENT_SSL_CERT          = 60203,
  TS_EVENT_SSL_SERVERNAME    = 60204,
  TS_EVENT_SSL_VERIFY_SERVER = 60205,
  TS_EVENT_SSL_VERIFY_CLIENT = 60206,
  TS_EVENT_SSL_CLIENT_HELLO  = 60207,
  TS_EVENT_SSL_SECRET        = 60208,

  TS_EVENT_MGMT_UPDATE = 60300
} TSEvent;
#define TS_EVENT_HTTP_READ_REQUEST_PRE_REMAP TS_EVENT_HTTP_PRE_REMAP /* backwards compat */

typedef enum {
  TS_SRVSTATE_STATE_UNDEFINED = 0,
  TS_SRVSTATE_ACTIVE_TIMEOUT,
  TS_SRVSTATE_BAD_INCOMING_RESPONSE,
  TS_SRVSTATE_CONNECTION_ALIVE,
  TS_SRVSTATE_CONNECTION_CLOSED,
  TS_SRVSTATE_CONNECTION_ERROR,
  TS_SRVSTATE_INACTIVE_TIMEOUT,
  TS_SRVSTATE_OPEN_RAW_ERROR,
  TS_SRVSTATE_PARSE_ERROR,
  TS_SRVSTATE_TRANSACTION_COMPLETE,
  TS_SRVSTATE_PARENT_RETRY,
  TS_SRVSTATE_OUTBOUND_CONGESTION
} TSServerState;

typedef enum {
  TS_LOOKUP_UNDEFINED_LOOKUP,
  TS_LOOKUP_PARENT_PROXY,
  TS_LOOKUP_ORIGIN_SERVER,
  TS_LOOKUP_INCOMING_ROUTER,
  TS_LOOKUP_HOST_NONE
} TSLookingUpType;

typedef enum {
  TS_CACHE_LOOKUP_MISS,
  TS_CACHE_LOOKUP_HIT_STALE,
  TS_CACHE_LOOKUP_HIT_FRESH,
  TS_CACHE_LOOKUP_SKIPPED
} TSCacheLookupResult;

typedef enum {
  TS_CACHE_DATA_TYPE_NONE,
  TS_CACHE_DATA_TYPE_HTTP,
  TS_CACHE_DATA_TYPE_OTHER,
} TSCacheDataType;

typedef enum {
  TS_CACHE_ERROR_NO_DOC    = -20400,
  TS_CACHE_ERROR_DOC_BUSY  = -20401,
  TS_CACHE_ERROR_NOT_READY = -20407,
} TSCacheError;

typedef enum {
  TS_CACHE_SCAN_RESULT_DONE     = 0,
  TS_CACHE_SCAN_RESULT_CONTINUE = 1,
  TS_CACHE_SCAN_RESULT_DELETE   = 10,
  TS_CACHE_SCAN_RESULT_DELETE_ALL_ALTERNATES,
  TS_CACHE_SCAN_RESULT_UPDATE,
  TS_CACHE_SCAN_RESULT_RETRY
} TSCacheScanResult;

typedef enum {
  TS_TXN_INFO_NONE = -1,
  TS_TXN_INFO_CACHE_HIT_RAM,
  TS_TXN_INFO_CACHE_COMPRESSED_IN_RAM,
  TS_TXN_INFO_CACHE_HIT_RWW, // READ_WHILE_WRITE
  TS_TXN_INFO_CACHE_OPEN_READ_TRIES,
  TS_TXN_INFO_CACHE_OPEN_WRITE_TRIES,
  TS_TXN_INFO_CACHE_VOLUME,
  TS_TXN_INFO_LAST_ENTRY
} TSHttpTxnInfoKey;

typedef enum {
  TS_VC_CLOSE_ABORT  = -1,
  TS_VC_CLOSE_NORMAL = 1,
} TSVConnCloseFlags;

typedef enum {
  TS_IOBUFFER_SIZE_INDEX_128  = 0,
  TS_IOBUFFER_SIZE_INDEX_256  = 1,
  TS_IOBUFFER_SIZE_INDEX_512  = 2,
  TS_IOBUFFER_SIZE_INDEX_1K   = 3,
  TS_IOBUFFER_SIZE_INDEX_2K   = 4,
  TS_IOBUFFER_SIZE_INDEX_4K   = 5,
  TS_IOBUFFER_SIZE_INDEX_8K   = 6,
  TS_IOBUFFER_SIZE_INDEX_16K  = 7,
  TS_IOBUFFER_SIZE_INDEX_32K  = 8,
  TS_IOBUFFER_SIZE_INDEX_64K  = 9,
  TS_IOBUFFER_SIZE_INDEX_128K = 10,
  TS_IOBUFFER_SIZE_INDEX_256K = 11,
  TS_IOBUFFER_SIZE_INDEX_512K = 12,
  TS_IOBUFFER_SIZE_INDEX_1M   = 13,
  TS_IOBUFFER_SIZE_INDEX_2M   = 14
} TSIOBufferSizeIndex;

typedef enum {
  TS_IOBUFFER_WATER_MARK_UNDEFINED         = -1,
  TS_IOBUFFER_WATER_MARK_PLUGIN_VC_DEFAULT = 0,     // mirror of DEFAULT_PLUGIN_VC_BUFFER_WATER_MARK
  TS_IOBUFFER_WATER_MARK_HTTP_DEFAULT      = 32768, // mirror of default_buffer_water_mark
} TSIOBufferWaterMark;

typedef enum {
  TS_ERROR   = -1,
  TS_SUCCESS = 0,
} TSReturnCode;

typedef enum {
  NO_CALLBACK = 0,
  AFTER_HEADER,
  AFTER_BODY,
} TSFetchWakeUpOptions;

/// Values for per server outbound connection tracking group definition.
/// See proxy.config.http.per_server.match
typedef enum {
  TS_SERVER_OUTBOUND_MATCH_IP,
  TS_SERVER_OUTBOUND_MATCH_PORT,
  TS_SERVER_OUTBOUND_MATCH_HOST,
  TS_SERVER_OUTBOUND_MATCH_BOTH
} TSOutboundConnectionMatchType;

/* librecords types */

/* The values of this enum must match enum RecT in I_RecDefs.h */
typedef enum {
  TS_RECORDTYPE_NULL    = 0x00,
  TS_RECORDTYPE_CONFIG  = 0x01,
  TS_RECORDTYPE_PROCESS = 0x02,
  TS_RECORDTYPE_NODE    = 0x04,
  TS_RECORDTYPE_LOCAL   = 0x10,
  TS_RECORDTYPE_PLUGIN  = 0x20,
  TS_RECORDTYPE_ALL     = 0x3F
} TSRecordType;

/* The values of this enum must match enum RecDataT in I_RecDefs.h */
typedef enum {
  TS_RECORDDATATYPE_NULL = 0,
  TS_RECORDDATATYPE_INT,
  TS_RECORDDATATYPE_FLOAT,
  TS_RECORDDATATYPE_STRING,
  TS_RECORDDATATYPE_COUNTER,
  TS_RECORDDATATYPE_STAT_CONST,
  TS_RECORDDATATYPE_STAT_FX,
  TS_RECORDDATATYPE_MAX
} TSRecordDataType;

typedef union {
  int64_t rec_int;
  float rec_float;
  char *rec_string;
  int64_t rec_counter;
} TSRecordData;

/* The values of this enum must match enum RecPersistT in I_RecDefs.h */
typedef enum {
  TS_RECORDP_NULL,
  TS_RECORDP_PERSISTENT,
  TS_RECORDP_NON_PERSISTENT,
} TSRecordPersistType;

/* The values of this enum must match enum RecUpdateT in I_RecDefs.h */
typedef enum {
  TS_RECORDUPDATE_NULL,
  TS_RECORDUPDATE_DYNAMIC,
  TS_RECORDUPDATE_RESTART_TS,
  TS_RECORDUPDATE_RESTART_TM,
} TSRecordUpdateType;

/* The values of this enum must match enum RecCheckT in I_RecDefs.h */
typedef enum {
  TS_RECORDCHECK_NULL,
  TS_RECORDCHECK_STR,
  TS_RECORDCHECK_INT,
  TS_RECORDCHECK_IP,
} TSRecordCheckType;

/* The values of this enum must match enum RecModeT in I_RecDefs.h */
typedef enum {
  TS_RECORDMODE_NULL,
  TS_RECORDMODE_CLIENT,
  TS_RECORDMODE_SERVER,
  TS_RECORDMODE_STAND_ALONE,
} TSRecordModeType;

/* The values of this enum must match enum RecAccessT in I_RecDefs.h */
typedef enum {
  TS_RECORDACCESS_NULL,
  TS_RECORDACCESS_NO_ACCESS,
  TS_RECORDACCESS_READ_ONLY,
} TSRecordAccessType;

typedef enum {
  TS_CONFIG_NULL = -1,
  TS_CONFIG_URL_REMAP_PRISTINE_HOST_HDR,
  TS_CONFIG_HTTP_CHUNKING_ENABLED,
  TS_CONFIG_HTTP_NEGATIVE_CACHING_ENABLED,
  TS_CONFIG_HTTP_NEGATIVE_CACHING_LIFETIME,
  TS_CONFIG_HTTP_CACHE_WHEN_TO_REVALIDATE,
  TS_CONFIG_HTTP_KEEP_ALIVE_ENABLED_IN,
  TS_CONFIG_HTTP_KEEP_ALIVE_ENABLED_OUT,
  TS_CONFIG_HTTP_KEEP_ALIVE_POST_OUT,
  TS_CONFIG_HTTP_SERVER_SESSION_SHARING_MATCH,
  TS_CONFIG_NET_SOCK_RECV_BUFFER_SIZE_OUT,
  TS_CONFIG_NET_SOCK_SEND_BUFFER_SIZE_OUT,
  TS_CONFIG_NET_SOCK_OPTION_FLAG_OUT,
  TS_CONFIG_HTTP_FORWARD_PROXY_AUTH_TO_PARENT,
  TS_CONFIG_HTTP_ANONYMIZE_REMOVE_FROM,
  TS_CONFIG_HTTP_ANONYMIZE_REMOVE_REFERER,
  TS_CONFIG_HTTP_ANONYMIZE_REMOVE_USER_AGENT,
  TS_CONFIG_HTTP_ANONYMIZE_REMOVE_COOKIE,
  TS_CONFIG_HTTP_ANONYMIZE_REMOVE_CLIENT_IP,
  TS_CONFIG_HTTP_ANONYMIZE_INSERT_CLIENT_IP,
  TS_CONFIG_HTTP_RESPONSE_SERVER_ENABLED,
  TS_CONFIG_HTTP_INSERT_SQUID_X_FORWARDED_FOR,
  TS_CONFIG_HTTP_SEND_HTTP11_REQUESTS,
  TS_CONFIG_HTTP_CACHE_HTTP,
  TS_CONFIG_HTTP_CACHE_IGNORE_CLIENT_NO_CACHE,
  TS_CONFIG_HTTP_CACHE_IGNORE_CLIENT_CC_MAX_AGE,
  TS_CONFIG_HTTP_CACHE_IMS_ON_CLIENT_NO_CACHE,
  TS_CONFIG_HTTP_CACHE_IGNORE_SERVER_NO_CACHE,
  TS_CONFIG_HTTP_CACHE_CACHE_RESPONSES_TO_COOKIES,
  TS_CONFIG_HTTP_CACHE_IGNORE_AUTHENTICATION,
  TS_CONFIG_HTTP_CACHE_CACHE_URLS_THAT_LOOK_DYNAMIC,
  TS_CONFIG_HTTP_CACHE_REQUIRED_HEADERS,
  TS_CONFIG_HTTP_INSERT_REQUEST_VIA_STR,
  TS_CONFIG_HTTP_INSERT_RESPONSE_VIA_STR,
  TS_CONFIG_HTTP_CACHE_HEURISTIC_MIN_LIFETIME,
  TS_CONFIG_HTTP_CACHE_HEURISTIC_MAX_LIFETIME,
  TS_CONFIG_HTTP_CACHE_GUARANTEED_MIN_LIFETIME,
  TS_CONFIG_HTTP_CACHE_GUARANTEED_MAX_LIFETIME,
  TS_CONFIG_HTTP_CACHE_MAX_STALE_AGE,
  TS_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_IN,
  TS_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_OUT,
  TS_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_IN,
  TS_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_OUT,
  TS_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_OUT,
  TS_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES,
  TS_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES_DEAD_SERVER,
  TS_CONFIG_HTTP_CONNECT_ATTEMPTS_RR_RETRIES,
  TS_CONFIG_HTTP_CONNECT_ATTEMPTS_TIMEOUT,
  TS_CONFIG_HTTP_POST_CONNECT_ATTEMPTS_TIMEOUT,
  TS_CONFIG_HTTP_DOWN_SERVER_CACHE_TIME,
  // Should be removed for 10.0
  TS_CONFIG_HTTP_DOWN_SERVER_ABORT_THRESHOLD,
  TS_CONFIG_HTTP_DOC_IN_CACHE_SKIP_DNS,
  TS_CONFIG_HTTP_BACKGROUND_FILL_ACTIVE_TIMEOUT,
  TS_CONFIG_HTTP_RESPONSE_SERVER_STR,
  TS_CONFIG_HTTP_CACHE_HEURISTIC_LM_FACTOR,
  TS_CONFIG_HTTP_BACKGROUND_FILL_COMPLETED_THRESHOLD,
  TS_CONFIG_NET_SOCK_PACKET_MARK_OUT,
  TS_CONFIG_NET_SOCK_PACKET_TOS_OUT,
  TS_CONFIG_HTTP_INSERT_AGE_IN_RESPONSE,
  TS_CONFIG_HTTP_CHUNKING_SIZE,
  TS_CONFIG_HTTP_FLOW_CONTROL_ENABLED,
  TS_CONFIG_HTTP_FLOW_CONTROL_LOW_WATER_MARK,
  TS_CONFIG_HTTP_FLOW_CONTROL_HIGH_WATER_MARK,
  TS_CONFIG_HTTP_CACHE_RANGE_LOOKUP,
  TS_CONFIG_HTTP_DEFAULT_BUFFER_SIZE,
  TS_CONFIG_HTTP_DEFAULT_BUFFER_WATER_MARK,
  TS_CONFIG_HTTP_REQUEST_HEADER_MAX_SIZE,
  TS_CONFIG_HTTP_RESPONSE_HEADER_MAX_SIZE,
  TS_CONFIG_HTTP_NEGATIVE_REVALIDATING_ENABLED,
  TS_CONFIG_HTTP_NEGATIVE_REVALIDATING_LIFETIME,
  TS_CONFIG_SSL_HSTS_MAX_AGE,
  TS_CONFIG_SSL_HSTS_INCLUDE_SUBDOMAINS,
  TS_CONFIG_HTTP_CACHE_OPEN_READ_RETRY_TIME,
  TS_CONFIG_HTTP_CACHE_MAX_OPEN_READ_RETRIES,
  TS_CONFIG_HTTP_CACHE_RANGE_WRITE,
  TS_CONFIG_HTTP_POST_CHECK_CONTENT_LENGTH_ENABLED,
  TS_CONFIG_HTTP_GLOBAL_USER_AGENT_HEADER,
  TS_CONFIG_HTTP_AUTH_SERVER_SESSION_PRIVATE,
  TS_CONFIG_HTTP_SLOW_LOG_THRESHOLD,
  TS_CONFIG_HTTP_CACHE_GENERATION,
  TS_CONFIG_BODY_FACTORY_TEMPLATE_BASE,
  TS_CONFIG_HTTP_CACHE_OPEN_WRITE_FAIL_ACTION,
  TS_CONFIG_HTTP_NUMBER_OF_REDIRECTIONS,
  TS_CONFIG_HTTP_CACHE_MAX_OPEN_WRITE_RETRIES,
  TS_CONFIG_HTTP_REDIRECT_USE_ORIG_CACHE_KEY,
  TS_CONFIG_HTTP_ATTACH_SERVER_SESSION_TO_CLIENT,
  TS_CONFIG_WEBSOCKET_NO_ACTIVITY_TIMEOUT,
  TS_CONFIG_WEBSOCKET_ACTIVE_TIMEOUT,
  TS_CONFIG_HTTP_UNCACHEABLE_REQUESTS_BYPASS_PARENT,
  TS_CONFIG_HTTP_PARENT_PROXY_TOTAL_CONNECT_ATTEMPTS,
  TS_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_IN,
  TS_CONFIG_SRV_ENABLED,
  TS_CONFIG_HTTP_FORWARD_CONNECT_METHOD,
  TS_CONFIG_SSL_CERT_FILENAME,
  TS_CONFIG_SSL_CLIENT_CERT_FILENAME = TS_CONFIG_SSL_CERT_FILENAME,
  TS_CONFIG_SSL_CERT_FILEPATH,
  TS_CONFIG_PARENT_FAILURES_UPDATE_HOSTDB,
  TS_CONFIG_HTTP_CACHE_IGNORE_ACCEPT_MISMATCH,
  TS_CONFIG_HTTP_CACHE_IGNORE_ACCEPT_LANGUAGE_MISMATCH,
  TS_CONFIG_HTTP_CACHE_IGNORE_ACCEPT_ENCODING_MISMATCH,
  TS_CONFIG_HTTP_CACHE_IGNORE_ACCEPT_CHARSET_MISMATCH,
  TS_CONFIG_HTTP_PARENT_PROXY_FAIL_THRESHOLD,
  TS_CONFIG_HTTP_PARENT_PROXY_RETRY_TIME,
  TS_CONFIG_HTTP_PER_PARENT_CONNECT_ATTEMPTS,
  TS_CONFIG_HTTP_PARENT_CONNECT_ATTEMPT_TIMEOUT,
  TS_CONFIG_HTTP_NORMALIZE_AE,
  TS_CONFIG_HTTP_INSERT_FORWARDED,
  TS_CONFIG_HTTP_PROXY_PROTOCOL_OUT,
  TS_CONFIG_HTTP_ALLOW_MULTI_RANGE,
  TS_CONFIG_HTTP_REQUEST_BUFFER_ENABLED,
  TS_CONFIG_HTTP_ALLOW_HALF_OPEN,
  TS_CONFIG_HTTP_SERVER_MIN_KEEP_ALIVE_CONNS,
  TS_CONFIG_HTTP_PER_SERVER_CONNECTION_MAX,
  TS_CONFIG_HTTP_PER_SERVER_CONNECTION_MATCH,
#if TS_VERSION_MAJOR < 10
  /* This is kept in the 9.x releases to preserve the ABI. Remove this in the
   * 10 release. */
  TS_CONFIG_SSL_CLIENT_VERIFY_SERVER,
#endif
  TS_CONFIG_SSL_CLIENT_VERIFY_SERVER_POLICY,
  TS_CONFIG_SSL_CLIENT_VERIFY_SERVER_PROPERTIES,
  TS_CONFIG_SSL_CLIENT_SNI_POLICY,
  TS_CONFIG_SSL_CLIENT_PRIVATE_KEY_FILENAME,
  TS_CONFIG_SSL_CLIENT_CA_CERT_FILENAME,
  TS_CONFIG_HTTP_HOST_RESOLUTION_PREFERENCE,
  TS_CONFIG_HTTP_CONNECT_DEAD_POLICY,
  TS_CONFIG_HTTP_MAX_PROXY_CYCLES,
  TS_CONFIG_PLUGIN_VC_DEFAULT_BUFFER_INDEX,
  TS_CONFIG_PLUGIN_VC_DEFAULT_BUFFER_WATER_MARK,
  TS_CONFIG_NET_SOCK_NOTSENT_LOWAT,
  TS_CONFIG_BODY_FACTORY_RESPONSE_SUPPRESSION_MODE,
  TS_CONFIG_HTTP_ENABLE_PARENT_TIMEOUT_MARKDOWNS,
  TS_CONFIG_HTTP_DISABLE_PARENT_MARKDOWNS,
  TS_CONFIG_LAST_ENTRY
} TSOverridableConfigKey;

/* The TASK pool of threads is the primary method of off-loading continuations from the
   net-threads. Configure this with proxy.config.task_threads in records.config. */
typedef enum {
  TS_THREAD_POOL_NET,
  TS_THREAD_POOL_TASK,
  /* unlikely you should use these */
  TS_THREAD_POOL_DNS,
  TS_THREAD_POOL_UDP
} TSThreadPool;

typedef int64_t TSHRTime;

/* The TSMilestonesType is an enum defining all the various milestones ("timers") that
   we track for a request. */
typedef enum {
  TS_MILESTONE_NULL = -1,
  TS_MILESTONE_UA_BEGIN,
  TS_MILESTONE_UA_FIRST_READ,
  TS_MILESTONE_UA_READ_HEADER_DONE,
  TS_MILESTONE_UA_BEGIN_WRITE,
  TS_MILESTONE_UA_CLOSE,
  TS_MILESTONE_SERVER_FIRST_CONNECT,
  TS_MILESTONE_SERVER_CONNECT,
  TS_MILESTONE_SERVER_CONNECT_END,
  TS_MILESTONE_SERVER_BEGIN_WRITE,
  TS_MILESTONE_SERVER_FIRST_READ,
  TS_MILESTONE_SERVER_READ_HEADER_DONE,
  TS_MILESTONE_SERVER_CLOSE,
  TS_MILESTONE_CACHE_OPEN_READ_BEGIN,
  TS_MILESTONE_CACHE_OPEN_READ_END,
  TS_MILESTONE_CACHE_OPEN_WRITE_BEGIN,
  TS_MILESTONE_CACHE_OPEN_WRITE_END,
  TS_MILESTONE_DNS_LOOKUP_BEGIN,
  TS_MILESTONE_DNS_LOOKUP_END,
  TS_MILESTONE_SM_START,
  TS_MILESTONE_SM_FINISH,
  TS_MILESTONE_PLUGIN_ACTIVE,
  TS_MILESTONE_PLUGIN_TOTAL,
  TS_MILESTONE_TLS_HANDSHAKE_START,
  TS_MILESTONE_TLS_HANDSHAKE_END,
  TS_MILESTONE_LAST_ENTRY
} TSMilestonesType;

/* These typedefs are used with the corresponding TSMgmt*Get functions
   for storing the values retrieved by those functions. For example,
   TSMgmtCounterGet() retrieves an TSMgmtCounter. */
typedef int64_t TSMgmtInt;
typedef int64_t TSMgmtCounter;
typedef float TSMgmtFloat;
typedef char *TSMgmtString;

/// The source of a management value.
typedef enum {
  TS_MGMT_SOURCE_NULL,     ///< No source / value not found.
  TS_MGMT_SOURCE_DEFAULT,  ///< Built in core default.
  TS_MGMT_SOURCE_PLUGIN,   ///< Plugin supplied default.
  TS_MGMT_SOURCE_EXPLICIT, ///< Set by administrator (config file, external API, etc.)
  TS_MGMT_SOURCE_ENV       ///< Process environment variable.
} TSMgmtSource;

/// The User Arg type, used for Txn/Ssn/VConn user argument slots
typedef enum {
  TS_USER_ARGS_TXN,   ///< Transaction based.
  TS_USER_ARGS_SSN,   ///< Session based
  TS_USER_ARGS_VCONN, ///< VConnection based
  TS_USER_ARGS_GLB,   ///< Global based
  TS_USER_ARGS_COUNT  ///< Fake enum, # of valid entries.
} TSUserArgType;

/** An enumeration of HTTP version types for the priority functions that behave
 * differently across HTTP protocols. */
typedef enum {
  HTTP_PRIORITY_TYPE_HTTP_UNSPECIFIED = 1,
  HTTP_PRIORITY_TYPE_HTTP_2,
  HTTP_PRIORITY_TYPE_HTTP_3,
} TSHttpPriorityType;

/** The abstract type of the various HTTP priority implementations. */
typedef struct {
  /** The reference to the concrete HTTP priority implementation. This will be
   * a value from TSHttpPriorityType. */
  uint8_t priority_type;
  /** The space allocated for the concrete priority implementation.
   *
   * Note that this has to take padding into account. There is a static_assert
   * in InkAPI.cc to verify that TSHttpPriority is at least as large as
   * TSHttp2Priority. As other structures are added that are represented by
   * TSHttpPriority add more static_asserts to verify that TSHttpPriority is as
   * large as it needs to be.
   */
  uint8_t data[7];
} TSHttpPriority;

/** A structure for HTTP/2 priority.
 *
 * For an explanation of these terms with respect to HTTP/2, see RFC 7540,
 * section 5.3.
 */
typedef struct {
  uint8_t priority_type; /** HTTP_PROTOCOL_TYPE_HTTP_2 */
  uint8_t weight;
  /** The stream dependency. Per spec, see RFC 7540 section 6.2, this is 31
   * bits. We use a signed 32 bit structure to store either a valid dependency
   * or -1 if the stream has no dependency. */
  int32_t stream_dependency;
} TSHttp2Priority;

typedef struct tsapi_file *TSFile;

typedef struct tsapi_mloc *TSMLoc;
typedef struct tsapi_mbuffer *TSMBuffer;
typedef struct tsapi_httpssn *TSHttpSsn;
typedef struct tsapi_httptxn *TSHttpTxn;
typedef struct tsapi_ssl_obj *TSSslConnection;
typedef struct tsapi_ssl_session *TSSslSession;
typedef struct tsapi_httpaltinfo *TSHttpAltInfo;
typedef struct tsapi_mimeparser *TSMimeParser;
typedef struct tsapi_httpparser *TSHttpParser;
typedef struct tsapi_cachekey *TSCacheKey;
typedef struct tsapi_cachehttpinfo *TSCacheHttpInfo;
typedef struct tsapi_cachetxn *TSCacheTxn;
typedef struct tsapi_x509_store_ctx *TSSslVerifyCTX;

typedef struct tsapi_port *TSPortDescriptor;
typedef struct tsapi_vio *TSVIO;
typedef struct tsapi_thread *TSThread;
typedef struct tsapi_event_thread *TSEventThread;
typedef struct tsapi_x509 *TSSslX509;
typedef struct tsapi_mutex *TSMutex;
typedef struct tsapi_config *TSConfig;
typedef struct tsapi_cont *TSCont;
typedef struct tsapi_cont *TSVConn; /* a VConn is really a specialized TSCont */
typedef struct tsapi_ssl_context *TSSslContext;
typedef struct tsapi_action *TSAction;
typedef struct tsapi_iobuffer *TSIOBuffer;
typedef struct tsapi_iobufferdata *TSIOBufferData;
typedef struct tsapi_bufferblock *TSIOBufferBlock;
typedef struct tsapi_bufferreader *TSIOBufferReader;
typedef struct tsapi_hostlookupresult *TSHostLookupResult;
typedef struct tsapi_aiocallback *TSAIOCallback;
typedef struct tsapi_net_accept *TSAcceptor;

typedef struct tsapi_fetchsm *TSFetchSM;

typedef void *(*TSThreadFunc)(void *data);
typedef int (*TSEventFunc)(TSCont contp, TSEvent event, void *edata);
typedef void (*TSConfigDestroyFunc)(void *data);

typedef struct {
  int success_event_id;
  int failure_event_id;
  int timeout_event_id;
} TSFetchEvent;

typedef struct TSFetchUrlParams {
  const char *request;
  int request_len;
  struct sockaddr_storage ip;
  int port;
  TSCont contp;
  TSFetchEvent events;
  TSFetchWakeUpOptions options;
  struct TSFetchUrlParams *next;
} TSFetchUrlParams_t;

// This is a duplicate of the SSL_MAX_SSL_SESSION_ID_LENGTH constant
// Redefining here so we don't include the openssl/ssl.h file here
#define TS_SSL_MAX_SSL_SESSION_ID_LENGTH 32

// This mirrors the internal data structure SSLSessionID
typedef struct TSSslSessionID_s {
  size_t len;
  char bytes[TS_SSL_MAX_SSL_SESSION_ID_LENGTH];
} TSSslSessionID;

typedef struct TSSecretID_s {
  const char *cert_name;
  size_t cert_name_len;
  const char *key_name;
  size_t key_name_len;
} TSSecretID;

typedef enum { TS_CONNECT_UNDEFINED, TS_CONNECT_PLUGIN, TS_CONNECT_LAST_ENTRY } TSConnectType;

typedef struct TSHttpConnectOptions_s {
  TSConnectType connect_type;
  struct sockaddr const *addr;
  const char *tag;
  int64_t id;
  TSIOBufferSizeIndex buffer_index;
  TSIOBufferWaterMark buffer_water_mark;
} TSHttpConnectOptions;

/* --------------------------------------------------------------------------
   Init */

/**
    This function must be defined by all plugins. Traffic Server
    calls this initialization routine when it loads the plugin (at
    startup), and sets argc and argv appropriately based on the values
    in plugin.config.

    @param argc the number of initial values specified in plugin.config,
      plus one. If only the name of your plugin shared object is
      specified in plugin.config, argc=1.
    @param argv the vector of arguments. The length of argv is argc.
      argv[0] is the name of the plugin shared library. Subsequent
      values of argv are initialization values specified in
      plugin.config.

 */
extern tsapi void TSPluginInit(int argc, const char *argv[]);

/* --------------------------------------------------------------------------
   URL schemes */
extern tsapi const char *TS_URL_SCHEME_FILE;
extern tsapi const char *TS_URL_SCHEME_FTP;
extern tsapi const char *TS_URL_SCHEME_GOPHER;
extern tsapi const char *TS_URL_SCHEME_HTTP;
extern tsapi const char *TS_URL_SCHEME_HTTPS;
extern tsapi const char *TS_URL_SCHEME_MAILTO;
extern tsapi const char *TS_URL_SCHEME_NEWS;
extern tsapi const char *TS_URL_SCHEME_NNTP;
extern tsapi const char *TS_URL_SCHEME_PROSPERO;
extern tsapi const char *TS_URL_SCHEME_TELNET;
extern tsapi const char *TS_URL_SCHEME_TUNNEL;
extern tsapi const char *TS_URL_SCHEME_WAIS;
extern tsapi const char *TS_URL_SCHEME_PNM;
extern tsapi const char *TS_URL_SCHEME_RTSP;
extern tsapi const char *TS_URL_SCHEME_RTSPU;
extern tsapi const char *TS_URL_SCHEME_MMS;
extern tsapi const char *TS_URL_SCHEME_MMSU;
extern tsapi const char *TS_URL_SCHEME_MMST;
extern tsapi const char *TS_URL_SCHEME_WS;
extern tsapi const char *TS_URL_SCHEME_WSS;

/* --------------------------------------------------------------------------
   URL scheme string lengths */
extern tsapi int TS_URL_LEN_FILE;
extern tsapi int TS_URL_LEN_FTP;
extern tsapi int TS_URL_LEN_GOPHER;
extern tsapi int TS_URL_LEN_HTTP;
extern tsapi int TS_URL_LEN_HTTPS;
extern tsapi int TS_URL_LEN_MAILTO;
extern tsapi int TS_URL_LEN_NEWS;
extern tsapi int TS_URL_LEN_NNTP;
extern tsapi int TS_URL_LEN_PROSPERO;
extern tsapi int TS_URL_LEN_TELNET;
extern tsapi int TS_URL_LEN_WAIS;
extern tsapi int TS_URL_LEN_WS;
extern tsapi int TS_URL_LEN_WSS;

/* --------------------------------------------------------------------------
   MIME fields */
extern tsapi const char *TS_MIME_FIELD_ACCEPT;
extern tsapi const char *TS_MIME_FIELD_ACCEPT_CHARSET;
extern tsapi const char *TS_MIME_FIELD_ACCEPT_ENCODING;
extern tsapi const char *TS_MIME_FIELD_ACCEPT_LANGUAGE;
extern tsapi const char *TS_MIME_FIELD_ACCEPT_RANGES;
extern tsapi const char *TS_MIME_FIELD_AGE;
extern tsapi const char *TS_MIME_FIELD_ALLOW;
extern tsapi const char *TS_MIME_FIELD_APPROVED;
extern tsapi const char *TS_MIME_FIELD_AUTHORIZATION;
extern tsapi const char *TS_MIME_FIELD_BYTES;
extern tsapi const char *TS_MIME_FIELD_CACHE_CONTROL;
extern tsapi const char *TS_MIME_FIELD_CLIENT_IP;
extern tsapi const char *TS_MIME_FIELD_CONNECTION;
extern tsapi const char *TS_MIME_FIELD_CONTENT_BASE;
extern tsapi const char *TS_MIME_FIELD_CONTENT_ENCODING;
extern tsapi const char *TS_MIME_FIELD_CONTENT_LANGUAGE;
extern tsapi const char *TS_MIME_FIELD_CONTENT_LENGTH;
extern tsapi const char *TS_MIME_FIELD_CONTENT_LOCATION;
extern tsapi const char *TS_MIME_FIELD_CONTENT_MD5;
extern tsapi const char *TS_MIME_FIELD_CONTENT_RANGE;
extern tsapi const char *TS_MIME_FIELD_CONTENT_TYPE;
extern tsapi const char *TS_MIME_FIELD_CONTROL;
extern tsapi const char *TS_MIME_FIELD_COOKIE;
extern tsapi const char *TS_MIME_FIELD_DATE;
extern tsapi const char *TS_MIME_FIELD_DISTRIBUTION;
extern tsapi const char *TS_MIME_FIELD_ETAG;
extern tsapi const char *TS_MIME_FIELD_EXPECT;
extern tsapi const char *TS_MIME_FIELD_EXPIRES;
extern tsapi const char *TS_MIME_FIELD_FOLLOWUP_TO;
extern tsapi const char *TS_MIME_FIELD_FROM;
extern tsapi const char *TS_MIME_FIELD_HOST;
extern tsapi const char *TS_MIME_FIELD_IF_MATCH;
extern tsapi const char *TS_MIME_FIELD_IF_MODIFIED_SINCE;
extern tsapi const char *TS_MIME_FIELD_IF_NONE_MATCH;
extern tsapi const char *TS_MIME_FIELD_IF_RANGE;
extern tsapi const char *TS_MIME_FIELD_IF_UNMODIFIED_SINCE;
extern tsapi const char *TS_MIME_FIELD_KEEP_ALIVE;
extern tsapi const char *TS_MIME_FIELD_KEYWORDS;
extern tsapi const char *TS_MIME_FIELD_LAST_MODIFIED;
extern tsapi const char *TS_MIME_FIELD_LINES;
extern tsapi const char *TS_MIME_FIELD_LOCATION;
extern tsapi const char *TS_MIME_FIELD_MAX_FORWARDS;
extern tsapi const char *TS_MIME_FIELD_MESSAGE_ID;
extern tsapi const char *TS_MIME_FIELD_NEWSGROUPS;
extern tsapi const char *TS_MIME_FIELD_ORGANIZATION;
extern tsapi const char *TS_MIME_FIELD_PATH;
extern tsapi const char *TS_MIME_FIELD_PRAGMA;
extern tsapi const char *TS_MIME_FIELD_PROXY_AUTHENTICATE;
extern tsapi const char *TS_MIME_FIELD_PROXY_AUTHORIZATION;
extern tsapi const char *TS_MIME_FIELD_PROXY_CONNECTION;
extern tsapi const char *TS_MIME_FIELD_PUBLIC;
extern tsapi const char *TS_MIME_FIELD_RANGE;
extern tsapi const char *TS_MIME_FIELD_REFERENCES;
extern tsapi const char *TS_MIME_FIELD_REFERER;
extern tsapi const char *TS_MIME_FIELD_REPLY_TO;
extern tsapi const char *TS_MIME_FIELD_RETRY_AFTER;
extern tsapi const char *TS_MIME_FIELD_SENDER;
extern tsapi const char *TS_MIME_FIELD_SERVER;
extern tsapi const char *TS_MIME_FIELD_SET_COOKIE;
extern tsapi const char *TS_MIME_FIELD_STRICT_TRANSPORT_SECURITY;
extern tsapi const char *TS_MIME_FIELD_SUBJECT;
extern tsapi const char *TS_MIME_FIELD_SUMMARY;
extern tsapi const char *TS_MIME_FIELD_TE;
extern tsapi const char *TS_MIME_FIELD_TRANSFER_ENCODING;
extern tsapi const char *TS_MIME_FIELD_UPGRADE;
extern tsapi const char *TS_MIME_FIELD_USER_AGENT;
extern tsapi const char *TS_MIME_FIELD_VARY;
extern tsapi const char *TS_MIME_FIELD_VIA;
extern tsapi const char *TS_MIME_FIELD_WARNING;
extern tsapi const char *TS_MIME_FIELD_WWW_AUTHENTICATE;
extern tsapi const char *TS_MIME_FIELD_XREF;
extern tsapi const char *TS_MIME_FIELD_X_FORWARDED_FOR;
extern tsapi const char *TS_MIME_FIELD_FORWARDED;

/* --------------------------------------------------------------------------
   MIME fields string lengths */
extern tsapi int TS_MIME_LEN_ACCEPT;
extern tsapi int TS_MIME_LEN_ACCEPT_CHARSET;
extern tsapi int TS_MIME_LEN_ACCEPT_ENCODING;
extern tsapi int TS_MIME_LEN_ACCEPT_LANGUAGE;
extern tsapi int TS_MIME_LEN_ACCEPT_RANGES;
extern tsapi int TS_MIME_LEN_AGE;
extern tsapi int TS_MIME_LEN_ALLOW;
extern tsapi int TS_MIME_LEN_APPROVED;
extern tsapi int TS_MIME_LEN_AUTHORIZATION;
extern tsapi int TS_MIME_LEN_BYTES;
extern tsapi int TS_MIME_LEN_CACHE_CONTROL;
extern tsapi int TS_MIME_LEN_CLIENT_IP;
extern tsapi int TS_MIME_LEN_CONNECTION;
extern tsapi int TS_MIME_LEN_CONTENT_BASE;
extern tsapi int TS_MIME_LEN_CONTENT_ENCODING;
extern tsapi int TS_MIME_LEN_CONTENT_LANGUAGE;
extern tsapi int TS_MIME_LEN_CONTENT_LENGTH;
extern tsapi int TS_MIME_LEN_CONTENT_LOCATION;
extern tsapi int TS_MIME_LEN_CONTENT_MD5;
extern tsapi int TS_MIME_LEN_CONTENT_RANGE;
extern tsapi int TS_MIME_LEN_CONTENT_TYPE;
extern tsapi int TS_MIME_LEN_CONTROL;
extern tsapi int TS_MIME_LEN_COOKIE;
extern tsapi int TS_MIME_LEN_DATE;
extern tsapi int TS_MIME_LEN_DISTRIBUTION;
extern tsapi int TS_MIME_LEN_ETAG;
extern tsapi int TS_MIME_LEN_EXPECT;
extern tsapi int TS_MIME_LEN_EXPIRES;
extern tsapi int TS_MIME_LEN_FOLLOWUP_TO;
extern tsapi int TS_MIME_LEN_FROM;
extern tsapi int TS_MIME_LEN_HOST;
extern tsapi int TS_MIME_LEN_IF_MATCH;
extern tsapi int TS_MIME_LEN_IF_MODIFIED_SINCE;
extern tsapi int TS_MIME_LEN_IF_NONE_MATCH;
extern tsapi int TS_MIME_LEN_IF_RANGE;
extern tsapi int TS_MIME_LEN_IF_UNMODIFIED_SINCE;
extern tsapi int TS_MIME_LEN_KEEP_ALIVE;
extern tsapi int TS_MIME_LEN_KEYWORDS;
extern tsapi int TS_MIME_LEN_LAST_MODIFIED;
extern tsapi int TS_MIME_LEN_LINES;
extern tsapi int TS_MIME_LEN_LOCATION;
extern tsapi int TS_MIME_LEN_MAX_FORWARDS;
extern tsapi int TS_MIME_LEN_MESSAGE_ID;
extern tsapi int TS_MIME_LEN_NEWSGROUPS;
extern tsapi int TS_MIME_LEN_ORGANIZATION;
extern tsapi int TS_MIME_LEN_PATH;
extern tsapi int TS_MIME_LEN_PRAGMA;
extern tsapi int TS_MIME_LEN_PROXY_AUTHENTICATE;
extern tsapi int TS_MIME_LEN_PROXY_AUTHORIZATION;
extern tsapi int TS_MIME_LEN_PROXY_CONNECTION;
extern tsapi int TS_MIME_LEN_PUBLIC;
extern tsapi int TS_MIME_LEN_RANGE;
extern tsapi int TS_MIME_LEN_REFERENCES;
extern tsapi int TS_MIME_LEN_REFERER;
extern tsapi int TS_MIME_LEN_REPLY_TO;
extern tsapi int TS_MIME_LEN_RETRY_AFTER;
extern tsapi int TS_MIME_LEN_SENDER;
extern tsapi int TS_MIME_LEN_SERVER;
extern tsapi int TS_MIME_LEN_SET_COOKIE;
extern tsapi int TS_MIME_LEN_STRICT_TRANSPORT_SECURITY;
extern tsapi int TS_MIME_LEN_SUBJECT;
extern tsapi int TS_MIME_LEN_SUMMARY;
extern tsapi int TS_MIME_LEN_TE;
extern tsapi int TS_MIME_LEN_TRANSFER_ENCODING;
extern tsapi int TS_MIME_LEN_UPGRADE;
extern tsapi int TS_MIME_LEN_USER_AGENT;
extern tsapi int TS_MIME_LEN_VARY;
extern tsapi int TS_MIME_LEN_VIA;
extern tsapi int TS_MIME_LEN_WARNING;
extern tsapi int TS_MIME_LEN_WWW_AUTHENTICATE;
extern tsapi int TS_MIME_LEN_XREF;
extern tsapi int TS_MIME_LEN_X_FORWARDED_FOR;
extern tsapi int TS_MIME_LEN_FORWARDED;

/* --------------------------------------------------------------------------
   HTTP values */
extern tsapi const char *TS_HTTP_VALUE_BYTES;
extern tsapi const char *TS_HTTP_VALUE_CHUNKED;
extern tsapi const char *TS_HTTP_VALUE_CLOSE;
extern tsapi const char *TS_HTTP_VALUE_COMPRESS;
extern tsapi const char *TS_HTTP_VALUE_DEFLATE;
extern tsapi const char *TS_HTTP_VALUE_GZIP;
extern tsapi const char *TS_HTTP_VALUE_BROTLI;
extern tsapi const char *TS_HTTP_VALUE_IDENTITY;
extern tsapi const char *TS_HTTP_VALUE_KEEP_ALIVE;
extern tsapi const char *TS_HTTP_VALUE_MAX_AGE;
extern tsapi const char *TS_HTTP_VALUE_MAX_STALE;
extern tsapi const char *TS_HTTP_VALUE_MIN_FRESH;
extern tsapi const char *TS_HTTP_VALUE_MUST_REVALIDATE;
extern tsapi const char *TS_HTTP_VALUE_NONE;
extern tsapi const char *TS_HTTP_VALUE_NO_CACHE;
extern tsapi const char *TS_HTTP_VALUE_NO_STORE;
extern tsapi const char *TS_HTTP_VALUE_NO_TRANSFORM;
extern tsapi const char *TS_HTTP_VALUE_ONLY_IF_CACHED;
extern tsapi const char *TS_HTTP_VALUE_PRIVATE;
extern tsapi const char *TS_HTTP_VALUE_PROXY_REVALIDATE;
extern tsapi const char *TS_HTTP_VALUE_PUBLIC;

/* --------------------------------------------------------------------------
   HTTP values string lengths */
extern tsapi int TS_HTTP_LEN_BYTES;
extern tsapi int TS_HTTP_LEN_CHUNKED;
extern tsapi int TS_HTTP_LEN_CLOSE;
extern tsapi int TS_HTTP_LEN_COMPRESS;
extern tsapi int TS_HTTP_LEN_DEFLATE;
extern tsapi int TS_HTTP_LEN_GZIP;
extern tsapi int TS_HTTP_LEN_BROTLI;
extern tsapi int TS_HTTP_LEN_IDENTITY;
extern tsapi int TS_HTTP_LEN_KEEP_ALIVE;
extern tsapi int TS_HTTP_LEN_MAX_AGE;
extern tsapi int TS_HTTP_LEN_MAX_STALE;
extern tsapi int TS_HTTP_LEN_MIN_FRESH;
extern tsapi int TS_HTTP_LEN_MUST_REVALIDATE;
extern tsapi int TS_HTTP_LEN_NONE;
extern tsapi int TS_HTTP_LEN_NO_CACHE;
extern tsapi int TS_HTTP_LEN_NO_STORE;
extern tsapi int TS_HTTP_LEN_NO_TRANSFORM;
extern tsapi int TS_HTTP_LEN_ONLY_IF_CACHED;
extern tsapi int TS_HTTP_LEN_PRIVATE;
extern tsapi int TS_HTTP_LEN_PROXY_REVALIDATE;
extern tsapi int TS_HTTP_LEN_PUBLIC;

/* --------------------------------------------------------------------------
   HTTP methods */
extern tsapi const char *TS_HTTP_METHOD_CONNECT;
extern tsapi const char *TS_HTTP_METHOD_DELETE;
extern tsapi const char *TS_HTTP_METHOD_GET;
extern tsapi const char *TS_HTTP_METHOD_HEAD;
extern tsapi const char *TS_HTTP_METHOD_OPTIONS;
extern tsapi const char *TS_HTTP_METHOD_POST;
extern tsapi const char *TS_HTTP_METHOD_PURGE;
extern tsapi const char *TS_HTTP_METHOD_PUT;
extern tsapi const char *TS_HTTP_METHOD_TRACE;
extern tsapi const char *TS_HTTP_METHOD_PUSH;

/* --------------------------------------------------------------------------
   HTTP methods string lengths */
extern tsapi int TS_HTTP_LEN_CONNECT;
extern tsapi int TS_HTTP_LEN_DELETE;
extern tsapi int TS_HTTP_LEN_GET;
extern tsapi int TS_HTTP_LEN_HEAD;
extern tsapi int TS_HTTP_LEN_OPTIONS;
extern tsapi int TS_HTTP_LEN_POST;
extern tsapi int TS_HTTP_LEN_PURGE;
extern tsapi int TS_HTTP_LEN_PUT;
extern tsapi int TS_HTTP_LEN_TRACE;
extern tsapi int TS_HTTP_LEN_PUSH;

/* --------------------------------------------------------------------------
   TLS Next Protocol well-known protocol names. */

extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_0_9;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_1_0;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_1_1;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_2_0;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_3;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_3_D27;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_QUIC;
extern tsapi const char *const TS_ALPN_PROTOCOL_HTTP_QUIC_D27;

extern tsapi int TS_ALPN_PROTOCOL_INDEX_HTTP_0_9;
extern tsapi int TS_ALPN_PROTOCOL_INDEX_HTTP_1_0;
extern tsapi int TS_ALPN_PROTOCOL_INDEX_HTTP_1_1;
extern tsapi int TS_ALPN_PROTOCOL_INDEX_HTTP_2_0;
extern tsapi int TS_ALPN_PROTOCOL_INDEX_HTTP_3;
extern tsapi int TS_ALPN_PROTOCOL_INDEX_HTTP_QUIC;

extern tsapi const char *const TS_ALPN_PROTOCOL_GROUP_HTTP;
extern tsapi const char *const TS_ALPN_PROTOCOL_GROUP_HTTP2;

extern tsapi const char *const TS_PROTO_TAG_HTTP_1_0;
extern tsapi const char *const TS_PROTO_TAG_HTTP_1_1;
extern tsapi const char *const TS_PROTO_TAG_HTTP_2_0;
extern tsapi const char *const TS_PROTO_TAG_HTTP_3;
extern tsapi const char *const TS_PROTO_TAG_HTTP_QUIC;
extern tsapi const char *const TS_PROTO_TAG_TLS_1_3;
extern tsapi const char *const TS_PROTO_TAG_TLS_1_2;
extern tsapi const char *const TS_PROTO_TAG_TLS_1_1;
extern tsapi const char *const TS_PROTO_TAG_TLS_1_0;
extern tsapi const char *const TS_PROTO_TAG_TCP;
extern tsapi const char *const TS_PROTO_TAG_UDP;
extern tsapi const char *const TS_PROTO_TAG_IPV4;
extern tsapi const char *const TS_PROTO_TAG_IPV6;

/* --------------------------------------------------------------------------
   MLoc Constants */
/**
    Use TS_NULL_MLOC as the parent in calls that require a parent
    when an TSMLoc does not have a parent TSMLoc. For example if
    the TSMLoc is obtained by a call to TSHttpTxnClientReqGet(),

 */
#define TS_NULL_MLOC ((TSMLoc)0)

/* --------------------------------------------------------------------------
   HostStatus types */

typedef enum {
  TS_HOST_STATUS_INIT,
  TS_HOST_STATUS_DOWN,
  TS_HOST_STATUS_UP,
} TSHostStatus;

/* MUST match proxy/HostStatus.h Reason.
 * If a value is added here, it MUST be added there with the same value.
 */
typedef enum {
  TS_HOST_STATUS_ACTIVE      = 0x1,
  TS_HOST_STATUS_LOCAL       = 0x2,
  TS_HOST_STATUS_MANUAL      = 0x4,
  TS_HOST_STATUS_SELF_DETECT = 0x8,
  TS_HOST_STATUS_ALL         = 0xf,
} TSHostStatusReason;

/* --------------------------------------------------------------------------
   Interface for the UUID APIs. https://www.ietf.org/rfc/rfc4122.txt. */
typedef enum {
  TS_UUID_UNDEFINED = 0,
  TS_UUID_V1        = 1,
  TS_UUID_V2,
  TS_UUID_V3,
  TS_UUID_V4, /* At this point, this is the only implemented version (or variant) */
  TS_UUID_V5,
} TSUuidVersion;

#define TS_UUID_STRING_LEN 36
#define TS_CRUUID_STRING_LEN (TS_UUID_STRING_LEN + 19 + 1) /* UUID-len + len(uint64_t) + '-' */
typedef struct tsapi_uuid *TSUuid;

/* Various HTTP "control" modes */
typedef enum {
  TS_HTTP_CNTL_LOGGING_MODE,
  TS_HTTP_CNTL_INTERCEPT_RETRY_MODE,
  TS_HTTP_CNTL_RESPONSE_CACHEABLE,
  TS_HTTP_CNTL_REQUEST_CACHEABLE,
  TS_HTTP_CNTL_SERVER_NO_STORE,
  TS_HTTP_CNTL_TXN_DEBUG,
  TS_HTTP_CNTL_SKIP_REMAPPING
} TSHttpCntlType;

#ifdef __cplusplus
namespace ts
{
  static const int NO_FD = -1; ///< No or invalid file descriptor.
}
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */