#include "SymbolResolver/dobby_symbol_resolver.h"
#include "dobby/common.h"

#include <elf.h>
#include <dlfcn.h>
#include <link.h>
#include <string.h>

#include "mmap_file_util.h"

#include "PlatformUtil/ProcessRuntimeUtility.h"

#include <vector>

#undef LOG_TAG
#define LOG_TAG "DobbySymbolResolver"

typedef struct elf_ctx {
  void *header;

  uintptr_t load_bias;

  ElfW(Shdr) * sym_sh_;
  ElfW(Shdr) * dynsym_sh_;

  const char *strtab_;
  ElfW(Sym) * symtab_;

  const char *dynstrtab_;
  ElfW(Sym) * dynsymtab_;

  size_t nbucket_;
  size_t nchain_;
  uint32_t *bucket_;
  uint32_t *chain_;

  size_t gnu_nbucket_;
  uint32_t *gnu_bucket_;
  uint32_t *gnu_chain_;
  uint32_t gnu_maskwords_;
  uint32_t gnu_shift2_;
  ElfW(Addr) * gnu_bloom_filter_;
} elf_ctx_t;

static void get_syms(ElfW(Ehdr) * header, ElfW(Sym) * *symtab_ptr, char **strtab_ptr, int *count_ptr) {
  ElfW(Shdr) *section_header = NULL;
  section_header = (ElfW(Shdr) *)((addr_t)header + header->e_shoff);

  ElfW(Shdr) *section_strtab_section_header = NULL;
  section_strtab_section_header = (ElfW(Shdr) *)((addr_t)section_header + header->e_shstrndx * header->e_shentsize);
  char *section_strtab = NULL;
  section_strtab = (char *)((addr_t)header + section_strtab_section_header->sh_offset);

  for (int i = 0; i < header->e_shnum; ++i) {
    const char *section_name = (const char *)(section_strtab + section_header->sh_name);
    if (section_header->sh_type == SHT_SYMTAB && strcmp(section_name, ".symtab") == 0) {
      *symtab_ptr = (ElfW(Sym) *)((addr_t)header + section_header->sh_offset);
      *count_ptr = section_header->sh_size / sizeof(ElfW(Sym));
    }

    if (section_header->sh_type == SHT_STRTAB && strcmp(section_name, ".strtab") == 0) {
      *strtab_ptr = (char *)((addr_t)header + section_header->sh_offset);
    }
    section_header = (ElfW(Shdr) *)((addr_t)section_header + header->e_shentsize);
  }
}

int elf_ctx_init(elf_ctx_t *ctx, void *header_, size_t mapped_size) {
  ElfW(Ehdr) *ehdr = (ElfW(Ehdr) *)header_;

  // Basic ELF validation
  if (!ehdr || mapped_size < sizeof(ElfW(Ehdr)))
    return -1;
  if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0)
    return -1;

  ctx->header = ehdr;

  ElfW(Addr) ehdr_addr = (ElfW(Addr))ehdr;

  // Validate program header table
  if (ehdr->e_phoff == 0 || ehdr->e_phnum == 0 ||
      ehdr->e_phoff + (size_t)ehdr->e_phnum * ehdr->e_phentsize > mapped_size) {
    // No usable program headers — still try sections below
  } else {
    // Handle dynamic segment
    ElfW(Addr) addr = 0;
    ElfW(Dyn) *dyn = NULL;
    ElfW(Phdr) *phdr = reinterpret_cast<ElfW(Phdr) *>(ehdr_addr + ehdr->e_phoff);
    for (size_t i = 0; i < ehdr->e_phnum; i++) {
      if (phdr[i].p_type == PT_DYNAMIC) {
        if (phdr[i].p_offset < mapped_size)
          dyn = reinterpret_cast<ElfW(Dyn) *>(ehdr_addr + phdr[i].p_offset);
      } else if (phdr[i].p_type == PT_LOAD) {
        addr = ehdr_addr + phdr[i].p_offset - phdr[i].p_vaddr;
        if (ctx->load_bias == 0)
          ctx->load_bias = ehdr_addr - (phdr[i].p_vaddr - phdr[i].p_offset);
      } else if (phdr[i].p_type == PT_PHDR) {
        ctx->load_bias = (ElfW(Addr))phdr - phdr[i].p_vaddr;
      }
    }
  }

  // Handle section — validate before any dereference
  if (ehdr->e_shoff != 0 && ehdr->e_shnum != 0 && ehdr->e_shstrndx < ehdr->e_shnum) {
    // Verify section header table fits within the mapped region
    size_t sh_table_end = (size_t)ehdr->e_shoff + (size_t)ehdr->e_shnum * (size_t)ehdr->e_shentsize;
    if (sh_table_end <= mapped_size) {
      ElfW(Shdr) *shdr = reinterpret_cast<ElfW(Shdr) *>(ehdr_addr + ehdr->e_shoff);

      // Validate section string table entry
      ElfW(Shdr) *shstr_sh = &shdr[ehdr->e_shstrndx];
      char *shstrtab = NULL;
      if (shstr_sh->sh_offset < mapped_size && shstr_sh->sh_offset + shstr_sh->sh_size <= mapped_size) {
        shstrtab = (char *)((addr_t)ehdr_addr + shstr_sh->sh_offset);
      }

      for (size_t i = 0; i < ehdr->e_shnum; i++) {
        // Bounds-check each section's offset
        if (shdr[i].sh_offset >= mapped_size)
          continue;

        if (shdr[i].sh_type == SHT_SYMTAB) {
          ctx->sym_sh_ = &shdr[i];
          ctx->symtab_ = (ElfW(Sym) *)(ehdr_addr + shdr[i].sh_offset);
        } else if (shdr[i].sh_type == SHT_STRTAB && shstrtab &&
                   shdr[i].sh_name < shstr_sh->sh_size &&
                   strcmp(shstrtab + shdr[i].sh_name, ".strtab") == 0) {
          ctx->strtab_ = (const char *)(ehdr_addr + shdr[i].sh_offset);
        } else if (shdr[i].sh_type == SHT_DYNSYM) {
          ctx->dynsym_sh_ = &shdr[i];
          ctx->dynsymtab_ = (ElfW(Sym) *)(ehdr_addr + shdr[i].sh_offset);
        } else if (shdr[i].sh_type == SHT_STRTAB && shstrtab &&
                   shdr[i].sh_name < shstr_sh->sh_size &&
                   strcmp(shstrtab + shdr[i].sh_name, ".dynstr") == 0) {
          ctx->dynstrtab_ = (const char *)(ehdr_addr + shdr[i].sh_offset);
        }
      }
    }
  }

  return 0;
}

static void *iterate_symbol_table_impl(const char *symbol_name, ElfW(Sym) * symtab, const char *strtab, int count) {
  for (int i = 0; i < count; ++i) {
    ElfW(Sym) *sym = symtab + i;
    const char *symbol_name_ = strtab + sym->st_name;
    if (strcmp(symbol_name_, symbol_name) == 0) {
      return (void *)sym->st_value;
    }
  }
  return NULL;
}

void *elf_ctx_iterate_symbol_table(elf_ctx_t *ctx, const char *symbol_name) {
  void *result = NULL;
  if (ctx->symtab_ && ctx->strtab_) {
    size_t count = ctx->sym_sh_->sh_size / sizeof(ElfW(Sym));
    result = iterate_symbol_table_impl(symbol_name, ctx->symtab_, ctx->strtab_, count);
    if (result)
      return result;
  }

  if (ctx->dynsymtab_ && ctx->dynstrtab_) {
    size_t count = ctx->dynsym_sh_->sh_size / sizeof(ElfW(Sym));
    result = iterate_symbol_table_impl(symbol_name, ctx->dynsymtab_, ctx->dynstrtab_, count);
    if (result)
      return result;
  }
  return NULL;
}

void *resolve_elf_internal_symbol(const char *library_name, const char *symbol_name) {
  void *result = NULL;
  LOG(LOG_LEVEL_INFO, "[Test MyStartPoint] resolve_elf_internal_symbol library_name :%s symbol_name : %s",library_name,symbol_name);
  if (library_name) {
    RuntimeModule module = ProcessRuntimeUtility::GetProcessModule(library_name);

    if (module.load_address) {
      auto mmapFileMng = MmapFileManager(module.path);
      auto file_mem = mmapFileMng.map();
      elf_ctx_t ctx;
      memset(&ctx, 0, sizeof(elf_ctx_t));
      if (file_mem) {
        elf_ctx_init(&ctx, file_mem, mmapFileMng.mmap_buffer_size);
        result = elf_ctx_iterate_symbol_table(&ctx, symbol_name);
      }
      if (result)
        result = (void *)((addr_t)result + (addr_t)module.load_address - ((addr_t)file_mem - (addr_t)ctx.load_bias));
    }
  }

  if (!result) {
    auto ProcessModuleMap = ProcessRuntimeUtility::GetProcessModuleMap();
      LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]-----------------------------");
    for (auto module : ProcessModuleMap) {

      if (module.load_address) {
        auto mmapFileMng = MmapFileManager(module.path);
        auto file_mem = mmapFileMng.map();

        elf_ctx_t ctx;
        memset(&ctx, 0, sizeof(elf_ctx_t));
        if (file_mem) {
          elf_ctx_init(&ctx, file_mem, mmapFileMng.mmap_buffer_size);
          result = elf_ctx_iterate_symbol_table(&ctx, symbol_name);
        }

        if (result)
          result = (void *)((addr_t)result + (addr_t)module.load_address - ((addr_t)file_mem - (addr_t)ctx.load_bias));
      }

      if (result)
        break;
    }
  }

    LOG(LOG_LEVEL_INFO, "[Test MyStartPoint] resolve_elf_internal_symbol Exit : %p ",result);
  return result;
}

// impl at "android_restriction.cc"
extern std::vector<void *> linker_get_solist();

PUBLIC void *DobbySymbolResolver(const char *image_name, const char *symbol_name_pattern) {
  void *result = NULL;

#if 0
  auto solist = linker_get_solist();
  for (auto soinfo : solist) {
    uintptr_t handle = linker_soinfo_to_handle(soinfo);
    if (image_name == NULL || strstr(linker_soinfo_get_realpath(soinfo), image_name) != 0) {
      result = dlsym((void *)handle, symbol_name_pattern);
      if (result)
        return result;
    }
  }
#endif
    LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]-----------------------------");
  result = dlsym(RTLD_DEFAULT, symbol_name_pattern);
    LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]-----------------------------");
  if (result)
    return result;
    LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]-----------------------------");
  result = resolve_elf_internal_symbol(image_name, symbol_name_pattern);
    LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]-----------------------------");
  return result;
}