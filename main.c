#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

bool is_elf(unsigned char *ehdr){
  if (ehdr[EI_MAG0] == ELFMAG0 && ehdr[EI_MAG1] == ELFMAG1 && ehdr[EI_MAG2] == ELFMAG2 && ehdr[EI_MAG3] == ELFMAG3)
    return true;

  return false;
}

int get_class(unsigned char *ehdr){
  switch (ehdr[EI_CLASS]) {
    case ELFCLASS64: return 64;
    case ELFCLASS32: return 32;
    default:         return 0;
  }
}

const char *machine_name(uint16_t machine){
  switch (machine) {
    case EM_X86_64:   return "Advanced Micro Devices X86-64";
    case EM_386:      return "Intel 80386";
    case EM_AARCH64:  return "ARM AArch64";
    default:          return "Unknown Machine";
  }
}

const char *type_name(uint16_t type){
  switch (type) {
    case ET_REL:  return "A relocatable file";
    case ET_EXEC: return "An executable file";
    case ET_DYN:  return "A shared file";
    case ET_CORE: return "A core file";
  default:        return "An Unknown type";
  }
}

const char *is_data(unsigned char *ehdr){
    switch (ehdr[EI_DATA]) {
      case ELFDATA2LSB: return "Little-endian";
      case ELFDATA2MSB: return "Big-endian";
    default:            return "Unknown data format";
    }
}

void print_flag(uint64_t flag){
  putchar(flag & SHF_WRITE ? 'W' : '-');
  putchar(flag & SHF_ALLOC ? 'A' : '-');
  putchar(flag & SHF_EXECINSTR ? 'X' : '-');
}

int main(int argc, char *argv[]){
  int result;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>\n",argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "rb");
  if (file == NULL){
    perror("open");
    return 1;
  }

  Elf64_Ehdr ehdr;

  if(fread(&ehdr, 1, sizeof(ehdr), file) != sizeof(ehdr)){
    fprintf(stderr, "File too small\n");
    fclose(file);
    return 1;
  }

  if (ehdr.e_shoff == 0 || ehdr.e_shnum == 0) {
      fprintf(stderr, "No section headers");
      fclose(file);
      return 1;
  }

  if (ehdr.e_shentsize != sizeof(Elf64_Shdr)) {
      fprintf(stderr, "Wrong sizes in sections header");
      fclose(file);
      return 1;
  }

  if (is_elf(ehdr.e_ident) == false) {fprintf(stderr, "Not Elf file\n"); fclose(file); return 1;}

  printf("ELF Header:\n");

  printf("Magic Number: ");
  for (size_t i = 0; i < sizeof(ehdr.e_ident); i++) {
    printf("%02x ",ehdr.e_ident[i]);
  }
  printf("\n");
  
    result = get_class(ehdr.e_ident);
  if (result == 0) {
    fprintf(stderr, "This Class is invalid\n"); fclose(file); return 1;}
  else if (result == 32) {  fprintf(stderr, "only 64-bit supported right now\n"); fclose(file); return 1;}
  else
    printf("Class: %d-bit architecture\n",result);

  printf("Data: %s\n",is_data(ehdr.e_ident));
  printf("Type: %s\n",type_name(ehdr.e_type));
  printf("Machine: %s\n",machine_name(ehdr.e_machine));

  printf("entry point: 0x%" PRIx64 "\n",ehdr.e_entry);
  printf("Program headers: 0x%" PRIx64 "\n",ehdr.e_phoff);
  printf("Section headers: 0x%" PRIx64 "\n",ehdr.e_shoff);

  printf("Flag: 0x%x\n",ehdr.e_flags);
  
  printf("Elf size of this headers: %d (bytes)\n",ehdr.e_ehsize);
  printf("Elf headers size in one entry: %d (bytes)\n",ehdr.e_phentsize);
  printf("Number of Program headers: %d\n",ehdr.e_phnum);

  printf("Number of section headers: %d\n",ehdr.e_shnum);
  printf("size of sections headers: %d (bytes)\n",ehdr.e_shentsize);
  printf("index of string table: %d\n",ehdr.e_shstrndx);

  //Section Headrs
  printf("\n Section Headers: \n");

  Elf64_Shdr shdrs[ehdr.e_shnum];
  fseek(file, ehdr.e_shoff, SEEK_SET);
  fread(shdrs, sizeof(Elf64_Shdr), ehdr.e_shnum, file);

  Elf64_Shdr strtab_hdr = shdrs[ehdr.e_shstrndx];
  char *shstrtab = malloc(strtab_hdr.sh_size);

  if (ehdr.e_shstrndx >= ehdr.e_shnum) {
    fprintf(stderr, "Wrong e_shstrndx\n");
    free(shstrtab);
    fclose(file);
    return 1;
  }

  fseek(file, strtab_hdr.sh_offset, SEEK_SET);
  fread(shstrtab, 1, strtab_hdr.sh_size, file);

  for (int i = 0; i < ehdr.e_shnum; i++) {
    //&shstrtab[shdrs[i].sh_name] same as name
    char *name = shstrtab + shdrs[i].sh_name;
    printf("[%2d] Name: %-21s Offset: 0x%010" PRIx64 " Size: %-3" PRIu64 " bytes Flag: ",
            i, name, shdrs[i].sh_offset, shdrs[i].sh_size);
    print_flag(shdrs[i].sh_flags);
    printf("\n");
  }

  free(shstrtab);
  fclose(file);
  return 0;
}
