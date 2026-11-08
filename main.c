#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isElf(char *ehdr){
  if (ehdr[EI_MAG0] == ELFMAG0 && ehdr[EI_MAG1] == ELFMAG1 && ehdr[EI_MAG2] == ELFMAG2 && ehdr[EI_MAG3] == ELFMAG3)
    return true;

  return false;
}


int main(int argc, char *argv[]){

  Elf64_Ehdr ehdr;
  

  FILE *file = fopen(argv[1], "rb");
  if (file == NULL){
    perror("open");
    return 1;
  }
  fread(&ehdr, 1, sizeof(ehdr), file);

  printf("entry point: %lx\n",ehdr.e_entry);
  for (int i = 0; i < sizeof(ehdr.e_ident); i++) {
    printf("%02x",ehdr.e_ident[i]);
  }
  printf("\n");

  if (isElf(argv[1]))
    printf("This is a Elf File\n");
  else
   printf("This isnt an Elf file\n");

  if (ehdr.e_ident[EI_CLASS] == ELFCLASS64) {
      printf("this file use 64 bit architecture");
  }
  else if (ehdr.e_ident[EI_CLASS] == ELFCLASS32) {
      printf("this file use 32 bit architecture");
  }
  else {
      printf("invalide architecture");
  }

  fclose(file);
  return 0;
}
