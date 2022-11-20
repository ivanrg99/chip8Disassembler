#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct file_contents
{
    uint16_t *data;
    size_t size;
} typedef file_contents;

uint16_t changeEndianness16(int16_t val)
{
    return (val << 8) |           // left-shift always fills with zeros
           ((val >> 8) & 0x00ff); // right-shift sign-extends, so force to zero
}

file_contents read_file(const char *path)
{
    file_contents res = {0};
    FILE *f = fopen(path, "rb");
    if (f)
    {
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

void parse_opcode(uint16_t opcode)
{
    opcode = changeEndianness16(opcode);
    printf("\n%X", opcode);

    switch((opcode & 0xF000 >> 12))
    {


    }

   

}

int main()
{
    file_contents rom = read_file("Brick.ch8");
    if (rom.data)
    {
        while (rom.size != 0)
        {
            parse_opcode(*rom.data);
            rom.data++;
            rom.size -= 2;
        }
    }

    return 0;
}