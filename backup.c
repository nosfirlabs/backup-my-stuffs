#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s SOURCE DESTINATION\n", argv[0]);
    return 1;
  }

  // Open the source directory
  DIR *source_dir = opendir(argv[1]);
  if (source_dir == NULL) {
    perror("Error opening source directory");
    return 1;
  }

  // Create the destination directory if it doesn't exist
  struct stat st = {0};
  if (stat(argv[2], &st) == -1) {
    mkdir(argv[2], 0700);
  }

  // Iterate over the files in the source directory
  struct dirent *entry;
  while ((entry = readdir(source_dir)) != NULL) {
    // Skip the current and parent directories
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // Concatenate the source and destination paths
    char source_path[1024];
    snprintf(source_path, sizeof(source_path), "%s/%s", argv[1], entry->d_name);

    char dest_path[1024];
    snprintf(dest_path, sizeof(dest_path), "%s/%s", argv[2], entry->d_name);

    // Copy the file from the source to the destination
    if (link(source_path, dest_path) != 0) {
      perror("Error copying file");
      return 1;
    }
  }

  // Close the source directory
  closedir(source_dir);

  return 0;
}
