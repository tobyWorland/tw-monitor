/*
 * A very basic and slow way of sending files
 *
 * The current usart driver is not very sophisticated and there isn't a hardware FIFO to use.
 * So until more is done to give it a software FIFO and possibly use
 * DMA, or turn flow control on, this will have to do.
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define ACK 6

int serial_fd = -1;
FILE *file = NULL;
long file_size = -1;

void usage(const char *prog_name) {
    fprintf(stderr, "%s <serial> <file_to_send>\n", prog_name);
    exit(1);
}

void open_serial(const char *path) {
    serial_fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd == -1) {
        fprintf(stderr, "On trying to open serial '%s' ", path);
        perror("fopen");
        exit(2);
    }

    struct termios termios;
    cfmakeraw(&termios);
    cfsetospeed(&termios, B115200);
    cfsetispeed(&termios, 0); // Same as output speed
    if (tcsetattr(serial_fd, TCSANOW, &termios) == -1) {
        fprintf(stderr, "On setting serial settings '%s' ", path);
        perror("tcsetattr");
        exit(2);
    }
}

void open_file(const char *path) {
    file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "On trying to open file '%s' ", path);
        perror("fopen");
        exit(3);
    }

    // Get size of file
    if (fseek(file, 0, SEEK_END)) {
        fprintf(stderr, "On getting size of file '%s' ", path);
        perror("fseek");
        exit(4);
    }

    file_size = ftell(file);
    if (file_size < 0) {
        fprintf(stderr, "On getting size of file '%s' ", path);
        perror("ftell");
        exit(4);
    }

    rewind(file);
}

bool transfer_byte(uint8_t byte) {
    if (write(serial_fd, &byte, 1) == -1) {
        fprintf(stderr, "Error on sending byte\n");
        perror("write");
        return false;
    }

    if (read(serial_fd, &byte, 1) == -1) {
        fprintf(stderr, "Error on receiving byte\n");
        perror("read");
        return false;
    }

    if (byte == ACK) {
        //printf("ACKd\n");
        return true;
    } else {
        fprintf(stderr, "Target didn't ACK!\n");
        return false;
    }
}

bool transfer_bytes(const uint8_t *ptr, size_t len) {
    while (len--) {
        if (!transfer_byte(*ptr++)) {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage(argv[0]);
    }

    const char *serialpath = argv[1];
    const char *filepath = argv[2];

    open_serial(serialpath);
    open_file(filepath);

    uint16_t len_u16 = (uint16_t)file_size;
    if (len_u16 != file_size) {
        fprintf(stderr, "File is too big (%lu bytes)\n", file_size);
        exit(5);
    }

    printf("Size: %d bytes\n", file_size);
    transfer_bytes((void*)&len_u16, sizeof(len_u16));

    printf("Length sent. Sending file bytes...\n");

    time_t last_time = 0;
    for (int i = 0; i < file_size; i++) {
        // Redisplay sending message if a second has past since the last message
        time_t current_time = time(NULL);
        if (current_time != last_time) {
            printf("\rSending %d of %d (%.02f%)...", i, file_size, 100.0f / file_size * i);
            fflush(stdout);
            last_time = current_time;
        }

        // Get byte from file
        int b = fgetc(file);
        //printf("Putting %02X\n", b);
        if (b == EOF) {
            fprintf(stderr, "Error on reading byte from serial\n");
            break;
        }

        // Send byte over serial
        if (!transfer_byte(b)) {
            break;
        }
    }
    putchar('\n');

    fclose(file);
    close(serial_fd);

    return 0;
}
