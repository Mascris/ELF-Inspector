#include <stdio.h>

int main(int argc, char *argv[])
{
  FILE *file = fopen("data.bin", "rb");

  fseek(file, 10, SEEK_SET);

  long sizee = ftell(file);
  printf("sizeis :%ld\n",sizee);

  fclose(file);
  return 0;
}
