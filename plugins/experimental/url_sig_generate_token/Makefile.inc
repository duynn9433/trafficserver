#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

#experimental_url_sig_generate_token_url_sig_generate_token_la_CPPFLAGS = $(AM_CPPFLAGS) $(FFMPEG_CFLAGS)
#experimental_url_sig_generate_token_url_sig_generate_token_la_CXXFLAGS = $(AM_CXXFLAGS)

pkglib_LTLIBRARIES += experimental/url_sig_generate_token/url_sig_generate_token.la

experimental_url_sig_generate_token_url_sig_generate_token_la_SOURCES = \
  experimental/url_sig_generate_token/url_sig_generate_token.c \
  experimental/url_sig_generate_token/hls_add_token.c \
  experimental/url_sig_generate_token/hls_add_token.h \
  experimental/url_sig_generate_token/str_builder.c \
  experimental/url_sig_generate_token/str_builder.h \
  experimental/url_sig_generate_token/url_sig_config.cpp \
  experimental/url_sig_generate_token/url_sig_config.h


#experimental_url_sig_generate_token_url_sig_generate_token_la_LDFLAGS = \
#$(AM_LDFLAGS) \
#-L$(top_builddir)/src/tscpp/api \
#$(FFMPEG_LIBS)

#experimental_url_sig_generate_token_url_sig_generate_token_la_LIBADD = \
#-ltscppapi \
#$(FFMPEG_LIBS)
