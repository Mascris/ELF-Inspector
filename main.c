#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const bool isElf(unsigned char *ehdr){
  if (ehdr[EI_MAG0] == ELFMAG0 && ehdr[EI_MAG1] == ELFMAG1 && ehdr[EI_MAG2] == ELFMAG2 && ehdr[EI_MAG3] == ELFMAG3)
    return true;

  return false;
}

const int isClass(unsigned char *ehdr){
  switch (ehdr[EI_CLASS]) {
    case ELFCLASS64: return 64;
    case ELFCLASS32: return 32;
    default:         return 0;
  }
}


const char *isMachine(uint16_t machine){
  switch (machine) {
    case EM_X86_64: return "Advanced Micro Devices X86-64";
    case EM_386: return "Intel 80386";
    case EM_AARCH64: return "ARM AArch64";
    default: return "Unkown Machine";
  }
}


int main(int argc, char *argv[]){
  int result;

  if (argc < 2) {
    fprintf(stderr, "Please enter 2 values");
    return 1;
  }

  FILE *file = fopen(argv[1], "rb");
  if (file == NULL){
    perror("open");
    return 1;
  }

  Elf64_Ehdr ehdr;
  fread(&ehdr, 1, sizeof(ehdr), file);

  if (isElf(ehdr.e_ident) == false) {fprintf(stderr, "Not Elf file"); fclose(file); return 1;};


  printf("ELF Header:\n");
  printf("entry point: %lx\n",ehdr.e_entry);

  printf("Magic Number: ");
  for (long unsigned int i = 0; i < sizeof(ehdr.e_ident); i++) {
    printf("%02x",ehdr.e_ident[i]);
  }
  printf("\n");
  
    result = isClass(ehdr.e_ident);
  if (result == 0) {
    fprintf(stderr, "This Class is invalid\n"); fclose(file); return 1;}
  else{
    printf("Class: %d-bit architecture\n",result);}

    printf("Machine: %s",isMachine(ehdr.e_machine));

  fclose(file);
  return 0;
}
