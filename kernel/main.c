void main() {
    char* video_memory = (char*) 0xb7ffe;
    char message[] = "Joel ";
    int i;
    for (i = 0; i < sizeof(message) / sizeof(char) - 1; i += 1) {
        video_memory += 2;
        *video_memory = message[i];
    }
}