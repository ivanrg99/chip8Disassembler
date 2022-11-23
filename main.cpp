#include <cstdio>

#ifndef _WIN32
#include <byteswap.h>
#endif
#include <cstdint>
#include <cstdlib>
#include <iostream>

struct file_contents {
    uint16_t *data;
    size_t size;
} typedef file_contents;

file_contents read_file(const char *path) {
    file_contents res = {nullptr};
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
    // printf("\n%X =>", opcode);
    const uint16_t op = (opcode & 0xF000) >> 12;
    const uint16_t Vx = (opcode & 0x0F00) >> 8;
    const uint16_t Vy = (opcode & 0x00F0) >> 4;
    const uint16_t lastByte = (opcode & 0x000F);
    const uint16_t last2Bytes = (opcode & 0x00FF);
    const uint16_t last3Bytes = (opcode & 0x0FFF);

    // std::cout << std::hex << Vy << std::endl;

    if (opcode == 0x00E0)
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "CLS"
                  << "\t\t\t "
                  << "#Clear the display\n";
    else if (opcode == 0x00EE)
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "RET"
                  << "\t\t\t "
                  << "#Return from a subroutine\n";

    switch (op) {
    case 0x0:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "NOP"
                  << "\t\t\t "
                  << "#No operation\n";
        break;
    case 0x1:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "JP " << std::hex << last3Bytes << "\t\t "
                  << "#Jump to location 0x" << std::hex << last3Bytes << "\n";
        break;
    case 0x2:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "CALL " << std::hex << last3Bytes << "\t\t "
                  << "#Call subroutine at 0x" << std::hex << last3Bytes << "\n";
        break;
    case 0x3:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "SE V" << std::dec << Vx << ", " << std::dec << last2Bytes
                  << "\t\t "
                  << "#Skip next instruction if V" << Vx << std::dec
                  << " == " << std::dec << last2Bytes << "\n";
        break;
    case 0x4:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "SNE V" << std::dec << Vx << ", " << std::dec << last2Bytes
                  << "\t\t "
                  << "#Skip next instruction if V" << Vx << std::dec
                  << " != " << std::dec << last2Bytes << "\n";
        break;
    case 0x5:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "SE V" << std::dec << Vx << ", V" << std::dec << Vy
                  << "\t\t "
                  << "#Skip next instruction if V" << Vx << std::dec << " == V"
                  << std::dec << Vy << "\n";
        break;
    case 0x6:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "LD V" << std::dec << Vx << ", " << std::dec << last2Bytes
                  << "\t\t "
                  << "#Set V" << Vx << std::dec << " = " << std::dec
                  << last2Bytes << "\n";
        break;
    case 0x7:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "ADD V" << std::dec << Vx << ", " << std::dec << last2Bytes
                  << "\t\t "
                  << "#Set V" << Vx << std::dec << " += " << std::dec
                  << last2Bytes << "\n";
        break;
    case 0x8:
        switch (lastByte) {
        case 0:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " = V" << std::dec << Vy
                      << "\n";
            break;
        case 1:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "OR V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " |= V" << std::dec << Vy
                      << "\n";
            break;
        case 2:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "AND V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " &= V" << std::dec << Vy
                      << "\n";
            break;
        case 3:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "XOR V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " ^= V" << std::dec << Vy
                      << "\n";
            break;
        case 4:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "ADD V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " += V" << std::dec << Vy
                      << ", set V15 = carry\n";
            break;
        case 5:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "SUB V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " -= V" << std::dec << Vy
                      << ", set V15 = NOT borrow\n";
            break;
        case 6:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "SHR V" << std::dec << Vx << " {, V" << std::dec << Vy
                      << "}\t\t "
                      << "#Set V" << Vx << std::dec << " = V" << std::dec << Vx
                      << " SHR 1\n";
            break;
        case 7:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "SUBN V" << std::dec << Vx << ", V" << std::dec << Vy
                      << "\t\t "
                      << "#Set V" << Vx << std::dec << " = V" << std::dec << Vy
                      << " - V" << std::dec << Vx << ", set V15 = NOT borrow\n";
            break;
        case 0xE:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "SHL V" << std::dec << Vx << " {, V" << std::dec << Vy
                      << "}\t\t "
                      << "#Set V" << Vx << std::dec << " = V" << std::dec << Vx
                      << " SHL 1\n";
            break;
        default:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "UNKNOWN OPCODE\n";
            break;
        }
        break;
    case 0x9:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "SNE V" << std::dec << Vx << ", V" << std::dec << Vy
                  << "\t\t "
                  << "#Skip next instruction if V" << Vx << std::dec << " != V"
                  << std::dec << Vy << "\n";
        break;
    case 0xA:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "LD I, " << std::hex << last3Bytes << "\t\t "
                  << "#Set I = " << std::dec << last3Bytes << "\n";
        break;
    case 0xB:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "JP V0, " << std::hex << last3Bytes << "\t\t "
                  << "#Jump to location 0x" << std::hex << last3Bytes
                  << " + V0\n";
        break;
    case 0xC:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "RND V" << std::dec << Vx << ", " << std::dec << last2Bytes
                  << "\t\t "
                  << "#Set V" << Vx << std::dec << " = (Random byte & "
                  << std::hex << last2Bytes << ")\n";
        break;
    case 0xD:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "DRW V" << std::dec << Vx << ", V" << std::dec << Vy
                  << ", " << std::dec << lastByte << "\t "
                  << "#Display " << std::dec << lastByte
                  << "-byte sprite starting at memory location I at (V"
                  << std::dec << Vx << ", V" << std::dec << Vy
                  << "), set V15 = collision \n";
        break;
    case 0xE:
        if (last2Bytes == 0x9E)
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "SKP V" << std::dec << Vx << "\t\t "
                      << "#Skip next instruction if key with the value of V"
                      << std::dec << Vx << " is pressed\n";
        else if (last2Bytes == 0xA1)
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "SKNP V" << std::dec << Vx << "\t\t "
                      << "#Skip next instruction if key with the value of V"
                      << std::dec << Vx << " is not pressed\n";
        break;
    case 0xF:
        switch (last2Bytes) {
        case 0x07:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD V, " << std::dec << Vx << ", DT\t\t "
                      << "#Set V" << std::dec << Vx << " = delay timer value"
                      << "\n";
            break;
        case 0x0A:
            std::cout
                << "0x" << std::uppercase << std::hex << opcode << "\t | "
                << "LD V, " << std::dec << Vx << ", K\t\t "
                << "#Wait for a key press, store the value of the key in V"
                << std::dec << Vx << "\n";
            break;
        case 0x15:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD DT, V" << std::dec << Vx << "\t\t "
                      << "#Set delay timer =  V" << std::dec << Vx << "\n";
            break;
        case 0x18:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD ST, V" << std::dec << Vx << "\t\t "
                      << "#Set sound timer =  V" << std::dec << Vx << "\n";
            break;
        case 0x1E:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "ADD I, V" << std::dec << Vx << "\t\t "
                      << "#Set I += V" << std::dec << Vx << "\n";
            break;
        case 0x29:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD F, V" << std::dec << Vx << "\t\t "
                      << "#Set I = location of sprite for digit V" << std::dec
                      << Vx << "\n";
            break;
        case 0x33:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD B, V" << std::dec << Vx << "\t\t "
                      << "#Store BCD representation of V" << std::dec << Vx
                      << " in memory locations I, I+1 and I+2\n";
            break;
        case 0x55:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD [I], V" << std::dec << Vx << "\t\t "
                      << "#Store registers V0 through V" << std::dec << Vx
                      << " in memory starting at location I\n";
            break;
        case 0x65:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "LD V" << std::dec << Vx << ", [I]\t\t "
                      << "#Read registers V0 through V" << std::dec << Vx
                      << " from memory starting at location I\n";
            break;
        default:
            std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                      << "UNKNOWN OPCODE\n";
            break;
        }
        break;
    default:
        std::cout << "0x" << std::uppercase << std::hex << opcode << "\t | "
                  << "UNKNOWN OPCODE\n";
        break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: chip8disassembler <path>" << std::endl;
        return -1;
    }
    file_contents rom = read_file(argv[1]);
    if (rom.data) {
        while (rom.size != 0) {
            parse_opcode(*rom.data);
            rom.data++;
            rom.size -= 2;
        }
    }

    return 0;
}
