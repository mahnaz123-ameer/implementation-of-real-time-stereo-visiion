void readPGMP2(const char *filename, image_t *image) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read PGM header
    char magic[3];
    fscanf(file, "%2s\n", magic);
    if (magic[0] != 'P' || magic[1] != '2') {
        fprintf(stderr, "Not a PGM P2 file\n");
        exit(EXIT_FAILURE);
    }

    // Skip comments
    char c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n');
        c = getc(file);
    }
    ungetc(c, file);

    // Read image dimensions
    fscanf(file, "%d %d\n", &(image->imagesize_x), &(image->imagesize_y));

    // Allocate memory for pixel array
    image->pixel = (int **)malloc(image->imagesize_x * sizeof(int *));
    for (int i = 0; i < image->imagesize_x; ++i) {
        image->pixel[i] = (int *)malloc(image->imagesize_y * sizeof(int));
    }

    // Read pixel data
    for (int y = 0; y < image->imagesize_y; ++y) {
        for (int x = 0; x < image->imagesize_x; ++x) {
            fscanf(file, "%d", &(image->pixel[x][y]));
        }
    }

    fclose(file);
}
