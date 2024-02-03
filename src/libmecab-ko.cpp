//
//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#if defined(_WIN32) && !defined(__CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "mecab-ko.h"
#include "tokenizer.h"
#include "utils.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace {
const char kUnknownError[] = "Unknown Error";
const size_t kErrorBufferSize = 256;
}

#if defined(_WIN32) && !defined(__CYGWIN__)
namespace {
DWORD g_tls_index = TLS_OUT_OF_INDEXES;
}

const char *getGlobalError() {
  LPVOID data = ::TlsGetValue(g_tls_index);
  return data == NULL ? kUnknownError : reinterpret_cast<const char *>(data);
}

void setGlobalError(const char *str) {
  char *data = reinterpret_cast<char *>(::TlsGetValue(g_tls_index));
  if (data == NULL) {
    return;
  }
  strncpy(data, str, kErrorBufferSize - 1);
  data[kErrorBufferSize - 1] = '\0';
}

HINSTANCE DllInstance = 0;

extern "C" {
  BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID) {
    LPVOID data = 0;
    if (!DllInstance) {
      DllInstance = hinst;
    }
    switch (dwReason) {
      case DLL_PROCESS_ATTACH:
        if ((g_tls_index = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
          return FALSE;
        }
        // Not break in order to initialize the TLS.
      case DLL_THREAD_ATTACH:
        data = (LPVOID)::LocalAlloc(LPTR, kErrorBufferSize);
        if (data) {
          ::TlsSetValue(g_tls_index, data);
        }
        break;
      case DLL_THREAD_DETACH:
        data = ::TlsGetValue(g_tls_index);
        if (data) {
          ::LocalFree((HLOCAL)data);
        }
        break;
      case DLL_PROCESS_DETACH:
        data = ::TlsGetValue(g_tls_index);
        if (data) {
          ::LocalFree((HLOCAL)data);
        }
        ::TlsFree(g_tls_index);
        g_tls_index = TLS_OUT_OF_INDEXES;
        break;
      default:
        break;
    }
    return TRUE;
  }
}
#else  // _WIN32
namespace {
#ifdef HAVE_TLS_KEYWORD
__thread char kErrorBuffer[kErrorBufferSize];
#else
char kErrorBuffer[kErrorBufferSize];
#endif
}

const char *getGlobalError() {
  return kErrorBuffer;
}

void setGlobalError(const char *str) {
  strncpy(kErrorBuffer, str, kErrorBufferSize - 1);
  kErrorBuffer[kErrorBufferSize - 1] = '\0';
}
#endif

mecab_ko_t* mecab_ko_new(int argc, char **argv) {
  MeCabKo::Tagger *tagger = MeCabKo::createTagger(argc, argv);
  if (!tagger) {
    MeCabKo::deleteTagger(tagger);
    return 0;
  }
  return reinterpret_cast<mecab_ko_t *>(tagger);
}

mecab_ko_t* mecab_ko_new2(const char *arg) {
  MeCabKo::Tagger *tagger = MeCabKo::createTagger(arg);
  if (!tagger) {
    MeCabKo::deleteTagger(tagger);
    return 0;
  }
  return reinterpret_cast<mecab_ko_t *>(tagger);
}

const char *mecab_ko_version() {
  return MeCabKo::Tagger::version();
}

const char* mecab_ko_strerror(mecab_ko_t *tagger) {
  if (!tagger) {
    return MeCabKo::getLastError();
  }
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->what();
}

void mecab_ko_destroy(mecab_ko_t *tagger) {
  MeCabKo::Tagger *ptr = reinterpret_cast<MeCabKo::Tagger *>(tagger);
  MeCabKo::deleteTagger(ptr);
  ptr = 0;
}

int  mecab_ko_get_partial(mecab_ko_t *tagger) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->partial();
}

void mecab_ko_set_partial(mecab_ko_t *tagger, int partial) {
  reinterpret_cast<MeCabKo::Tagger *>(tagger)->set_partial(partial);
}

float  mecab_ko_get_theta(mecab_ko_t *tagger) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->theta();
}

void mecab_ko_set_theta(mecab_ko_t *tagger, float theta) {
  reinterpret_cast<MeCabKo::Tagger *>(tagger)->set_theta(theta);
}

int  mecab_ko_get_lattice_level(mecab_ko_t *tagger) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->lattice_level();
}

void mecab_ko_set_lattice_level(mecab_ko_t *tagger, int level) {
  reinterpret_cast<MeCabKo::Tagger *>(tagger)->set_lattice_level(level);
}

int mecab_ko_get_all_morphs(mecab_ko_t *tagger) {
  return static_cast<int>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->all_morphs());
}

void mecab_ko_set_all_morphs(mecab_ko_t *tagger, int all_morphs) {
  reinterpret_cast<MeCabKo::Tagger *>(tagger)->set_all_morphs(all_morphs);
}

const char* mecab_ko_sparse_tostr(mecab_ko_t *tagger, const char *str) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->parse(str);
}

const char* mecab_ko_sparse_tostr2(mecab_ko_t *tagger, const char *str, size_t len) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->parse(str, len);
}

char* mecab_ko_sparse_tostr3(mecab_ko_t *tagger, const char *str, size_t len,
                          char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->parse(
          str, len, out, len2));
}

const mecab_ko_node_t* mecab_ko_sparse_tonode(mecab_ko_t *tagger, const char *str) {
  return reinterpret_cast<const mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->parseToNode(str));
}

const mecab_ko_node_t* mecab_ko_sparse_tonode2(mecab_ko_t *tagger,
                                         const char *str, size_t len) {
  return reinterpret_cast<const mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->parseToNode(str, len));
}

const char* mecab_ko_nbest_sparse_tostr(mecab_ko_t *tagger, size_t N,
                                     const char *str) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->parseNBest(N, str);
}

const char* mecab_ko_nbest_sparse_tostr2(mecab_ko_t *tagger, size_t N,
                                      const char* str, size_t len) {
  return reinterpret_cast<MeCabKo::Tagger *>(
      tagger)->parseNBest(N, str, len);
}

char* mecab_ko_nbest_sparse_tostr3(mecab_ko_t *tagger, size_t N,
                                const char *str, size_t len,
                                char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<MeCabKo::Tagger *>(
          tagger)->parseNBest(N, str, len, out, len2));
}

int mecab_ko_nbest_init(mecab_ko_t *tagger, const char *str) {
  return reinterpret_cast<
      MeCabKo::Tagger *>(tagger)->parseNBestInit(str);
}

int mecab_ko_nbest_init2(mecab_ko_t *tagger, const char *str, size_t len) {
  return reinterpret_cast<
      MeCabKo::Tagger *>(tagger)->parseNBestInit(str, len);
}

const char* mecab_ko_nbest_next_tostr(mecab_ko_t *tagger) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->next();
}

char* mecab_ko_nbest_next_tostr2(mecab_ko_t *tagger, char *out, size_t len2) {
  return const_cast<char *>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->next(out, len2));
}

const mecab_ko_node_t* mecab_ko_nbest_next_tonode(mecab_ko_t *tagger) {
  return reinterpret_cast<const mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->nextNode());
}

const char* mecab_ko_format_node(mecab_ko_t *tagger, const mecab_ko_node_t* n) {
  return reinterpret_cast<MeCabKo::Tagger *>(tagger)->formatNode(n);
}

const mecab_ko_dictionary_info_t *mecab_ko_dictionary_info(mecab_ko_t *tagger) {
  return reinterpret_cast<const mecab_ko_dictionary_info_t *>(
      reinterpret_cast<MeCabKo::Tagger *>(tagger)->dictionary_info());
}

int mecab_ko_parse_lattice(mecab_ko_t *mecab, mecab_ko_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<MeCabKo::Tagger *>(mecab)->parse(
          reinterpret_cast<MeCabKo::Lattice *>(lattice)));
}

mecab_ko_lattice_t *mecab_ko_lattice_new() {
  return reinterpret_cast<mecab_ko_lattice_t *>(MeCabKo::createLattice());
}

void mecab_ko_lattice_destroy(mecab_ko_lattice_t *lattice) {
  MeCabKo::Lattice *ptr = reinterpret_cast<MeCabKo::Lattice *>(lattice);
  MeCabKo::deleteLattice(ptr);
  ptr = 0;
}

void mecab_ko_lattice_clear(mecab_ko_lattice_t *lattice) {
  reinterpret_cast<MeCabKo::Lattice *>(lattice)->clear();
}

int mecab_ko_lattice_is_available(mecab_ko_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->is_available());
}
mecab_ko_node_t *mecab_ko_lattice_get_bos_node(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->bos_node());
}

mecab_ko_node_t *mecab_ko_lattice_get_eos_node(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->eos_node());
}

mecab_ko_node_t **mecab_ko_lattice_get_all_begin_nodes(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<mecab_ko_node_t **>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->begin_nodes());
}

mecab_ko_node_t **mecab_ko_lattice_get_all_end_nodes(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<mecab_ko_node_t **>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->end_nodes());
}

mecab_ko_node_t *mecab_ko_lattice_get_begin_nodes(mecab_ko_lattice_t *lattice,
                                            size_t pos) {
  return reinterpret_cast<mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->begin_nodes(pos));
}

mecab_ko_node_t    *mecab_ko_lattice_get_end_nodes(mecab_ko_lattice_t *lattice,
                                             size_t pos) {
  return reinterpret_cast<mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->end_nodes(pos));
}

const char  *mecab_ko_lattice_get_sentence(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->sentence();
}

void  mecab_ko_lattice_set_sentence(mecab_ko_lattice_t *lattice,
                                 const char *sentence) {
  reinterpret_cast<MeCabKo::Lattice *>(lattice)->set_sentence(sentence);
}

void mecab_ko_lattice_set_sentence2(mecab_ko_lattice_t *lattice,
                                 const char *sentence, size_t len) {
  reinterpret_cast<MeCabKo::Lattice *>(lattice)->set_sentence(
      sentence, len);
}

size_t mecab_ko_lattice_get_size(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->size();
}

double mecab_ko_lattice_get_z(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->Z();
}

void mecab_ko_lattice_set_z(mecab_ko_lattice_t *lattice, double Z) {
  reinterpret_cast<MeCabKo::Lattice *>(lattice)->set_Z(Z);
}

double mecab_ko_lattice_get_theta(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->theta();
}

void mecab_ko_lattice_set_theta(mecab_ko_lattice_t *lattice, double theta) {
  reinterpret_cast<MeCabKo::Lattice *>(lattice)->set_theta(theta);
}

int mecab_ko_lattice_next(mecab_ko_lattice_t *lattice) {
  return static_cast<int>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->next());
}

int mecab_ko_lattice_get_request_type(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->request_type();
}

int mecab_ko_lattice_has_request_type(mecab_ko_lattice_t *lattice,
                                   int request_type) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->has_request_type(request_type);
}

void mecab_ko_lattice_set_request_type(mecab_ko_lattice_t *lattice,
                                    int request_type) {
  reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->set_request_type(request_type);
}

void mecab_ko_lattice_add_request_type(mecab_ko_lattice_t *lattice,
                                    int request_type) {
  reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->add_request_type(request_type);
}

void mecab_ko_lattice_remove_request_type(mecab_ko_lattice_t *lattice,
                                       int request_type) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->remove_request_type(request_type);
}

mecab_ko_node_t    *mecab_ko_lattice_new_node(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Lattice *>(lattice)->newNode());
}

const char *mecab_ko_lattice_tostr(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->toString();
}

const char *mecab_ko_lattice_tostr2(mecab_ko_lattice_t *lattice,
                                 char *buf, size_t size) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->toString(buf, size);
}
const char *mecab_ko_lattice_nbest_tostr(mecab_ko_lattice_t *lattice,
                                      size_t N) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->enumNBestAsString(N);
}
const char *mecab_ko_lattice_nbest_tostr2(mecab_ko_lattice_t *lattice,
                                       size_t N, char *buf, size_t size) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->enumNBestAsString(N, buf, size);
}

int mecab_ko_lattice_has_constraint(mecab_ko_lattice_t *lattice) {
  return static_cast<bool>(reinterpret_cast<MeCabKo::Lattice *>(
                               lattice)->has_constraint());
}

int mecab_ko_lattice_get_boundary_constraint(mecab_ko_lattice_t *lattice,
                                          size_t pos) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->boundary_constraint(pos);
}

const char *mecab_ko_lattice_get_feature_constraint(mecab_ko_lattice_t *lattice,
                                                 size_t pos) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->feature_constraint(pos);
}

void mecab_ko_lattice_set_boundary_constraint(mecab_ko_lattice_t *lattice,
                                           size_t pos, int boundary_type) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->set_boundary_constraint(pos, boundary_type);
}

void mecab_ko_lattice_set_feature_constraint(mecab_ko_lattice_t *lattice,
                                          size_t begin_pos, size_t end_pos,
                                          const char *feature) {
  return reinterpret_cast<MeCabKo::Lattice *>(
      lattice)->set_feature_constraint(begin_pos, end_pos, feature);
}

void mecab_ko_lattice_set_result(mecab_ko_lattice_t *lattice,
                              const char *result) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->set_result(result);
}

const char *mecab_ko_lattice_strerror(mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<MeCabKo::Lattice *>(lattice)->what();
}

mecab_ko_model_t *mecab_ko_model_new(int argc, char **argv) {
  MeCabKo::Model *model = MeCabKo::createModel(argc, argv);
  if (!model) {
    MeCabKo::deleteModel(model);
    return 0;
  }
  return reinterpret_cast<mecab_ko_model_t *>(model);
}

mecab_ko_model_t *mecab_ko_model_new2(const char *arg) {
  MeCabKo::Model *model = MeCabKo::createModel(arg);
  if (!model) {
    MeCabKo::deleteModel(model);
    return 0;
  }
  return reinterpret_cast<mecab_ko_model_t *>(model);
}

void mecab_ko_model_destroy(mecab_ko_model_t *model) {
  MeCabKo::Model *ptr = reinterpret_cast<MeCabKo::Model *>(model);
  MeCabKo::deleteModel(ptr);
  ptr = 0;
}

mecab_ko_t *mecab_ko_model_new_tagger(mecab_ko_model_t *model) {
  return reinterpret_cast<mecab_ko_t *>(
      reinterpret_cast<MeCabKo::Model *>(model)->createTagger());
}

mecab_ko_lattice_t *mecab_ko_model_new_lattice(mecab_ko_model_t *model) {
  return reinterpret_cast<mecab_ko_lattice_t *>(
      reinterpret_cast<MeCabKo::Model *>(model)->createLattice());
}

int mecab_ko_model_swap(mecab_ko_model_t *model, mecab_ko_model_t *new_model) {
  return static_cast<int>(
      reinterpret_cast<MeCabKo::Model *>(model)->swap(
          reinterpret_cast<MeCabKo::Model *>(new_model)));
}

const mecab_ko_dictionary_info_t* mecab_ko_model_dictionary_info(
    mecab_ko_model_t *model) {
  return reinterpret_cast<const mecab_ko_dictionary_info_t *>(
      reinterpret_cast<MeCabKo::Model *>(model)->dictionary_info());
}

int mecab_ko_model_transition_cost(mecab_ko_model_t *model,
                                unsigned short rcAttr,
                                unsigned short lcAttr) {
  return reinterpret_cast<MeCabKo::Model *>(model)->transition_cost(
      rcAttr, lcAttr);
}

mecab_ko_node_t *mecab_ko_model_lookup(mecab_ko_model_t *model,
                                 const char *begin,
                                 const char *end,
                                 mecab_ko_lattice_t *lattice) {
  return reinterpret_cast<mecab_ko_node_t *>(
      reinterpret_cast<MeCabKo::Model *>(model)->lookup(
          begin, end,
          reinterpret_cast<MeCabKo::Lattice *>(lattice)));
}
