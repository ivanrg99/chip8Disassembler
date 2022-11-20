#include <stdio.h>

#ifndef _WIN32
#include <byteswap.h>
#endif
#include <stdint.h>
#include <stdlib.h>

struct file_contents {
  uint16_t *data;
  size_t size;
} typedef file_contents;

file_contents read_file(const char *path) {
  file_contents res = {0};
  FILE *f = fopen(path, "rb");
  if (f) {
    fseek(f, 0, SEEK_END);
    size_t f_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    res.data = (uint16_t *)malloc(f_size);
    fread(res.data, f_size, 1, f);

    res.size = f_size;
    fclose(f);
  }

  return res;
}

void parse_opcode(uint16_t opcode) {
#ifdef _WIN32
  opcode = _byteswap_ushort(opcode);
#else
  opcode = __bswap_16(opcode);
#endif
  printf("\n%X", opcode);

  switch ((opcode & 0xF000 >> 12)) {}
}

int main() {
  file_contents rom = read_file("Brick.ch8");
  if (rom.data) {
    while (rom.size != 0) {
      parse_opcode(*rom.data);
      rom.data++;
      rom.size -= 2;
    }
  }

  return 0;
}
