#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define WIDTH_GUEST 24
#define HEIGHT_GUEST 20

typedef struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;

typedef struct frame {
    color_t values[HEIGHT_GUEST][WIDTH_GUEST];
} frame_t;

const char *FB_DEV_PATH = "/dev/fb1";  // Framebuffer device
const char *UDP_TARGET_IP = "192.168.1.196";  // Hardcoded IP
const int UDP_TARGET_PORT = 8080;  // Hardcoded port
const int FRAME_BUFFER_SIZE = WIDTH_GUEST * HEIGHT_GUEST * 4;  // 32-bit RGBA framebuffer size

void send_frame(frame_t *frame, int sockfd, struct sockaddr_in *target_addr) {
    // Send the frame data as a UDP packet
    ssize_t sent_bytes = sendto(sockfd, frame, sizeof(frame_t), 0,
                                (struct sockaddr *)target_addr, sizeof(*target_addr));
    if (sent_bytes < 0) {
        perror("Failed to send UDP packet");
    } else {
        // printf("Sent %ld bytes as UDP packet\n", sent_bytes);
    }
}

int main() {
    int fb_fd;
    uint8_t framebuffer[FRAME_BUFFER_SIZE];  // Buffer for reading /dev/fb1 data
    frame_t frame;
    int sockfd;
    struct sockaddr_in target_addr;

    // Open the framebuffer device file
    fb_fd = open(FB_DEV_PATH, O_RDONLY);
    if (fb_fd < 0) {
        perror("Failed to open framebuffer device");
        return EXIT_FAILURE;
    }

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Failed to create UDP socket");
        close(fb_fd);
        return EXIT_FAILURE;
    }

    // Set up the target address structure
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(UDP_TARGET_PORT);
    if (inet_pton(AF_INET, UDP_TARGET_IP, &target_addr.sin_addr) <= 0) {
        perror("Invalid target IP address");
        close(fb_fd);
        close(sockfd);
        return EXIT_FAILURE;
    }

    while (1) {
        // Read the framebuffer content
        ssize_t bytes_read = pread(fb_fd, framebuffer, FRAME_BUFFER_SIZE, 0);
        if (bytes_read != FRAME_BUFFER_SIZE) {
            perror("Failed to read framebuffer data");
            break;
        }

        // Convert framebuffer data to frame_t format (ignoring the alpha channel)
        for (int y = 0; y < HEIGHT_GUEST; y++) {
            for (int x = 0; x < WIDTH_GUEST; x++) {
                int fb_index = (y * WIDTH_GUEST + x) * 4;  // 4 bytes per pixel (32bpp)
                frame.values[y][x].red = framebuffer[fb_index + 2];    // Red
                frame.values[y][x].green = framebuffer[fb_index + 1];  // Green
                frame.values[y][x].blue = framebuffer[fb_index];       // Blue
            }
        }

        // Send the frame as a UDP packet
        send_frame(&frame, sockfd, &target_addr);

        // Sleep for a bit (e.g., 1/30th of a second for ~30 FPS)
        usleep(1000000 / 30);  // 30 FPS
    }

    // Cleanup
    close(fb_fd);
    close(sockfd);
    return EXIT_SUCCESS;
}
