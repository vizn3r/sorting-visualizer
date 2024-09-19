#include "./src/algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <time.h>
#include <unistd.h>

int ARR_SIZE = 10;

/*
 * TODO:
 * 1. graph renderer
 * 2. interface for sorting algorithms
 * 3. anything cool that comes to your mind :D
 *    - sorting sound
 *    - everything will be defined in "algorithm.c" file
 *    - different types of charts
 *
 * */

typedef struct {
  size_t width;  // width of viewport
  size_t height; // height of viewport
} window_t;

typedef struct {
  char ch;       // character to make columns off
  size_t n_cols; // number of elements in sorting array basically

  // active color
  // sorted color
} chart_t;

window_t window;
chart_t chart;

void stdin_dis() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void stdin_en() {
  struct termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  oldt.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void print_chart(int *sort_arr) {
  printf("\033[0;0H");
  fflush(stdout);
  char **display = (char **)malloc(sizeof(char) * window.width * window.height);
  for (int i = 0; i < window.height; i++) {
    display[i] = (char *)malloc(sizeof(char) * window.width);
    for (int j = 0; j < window.width; j++)
      display[i][j] = ' ';
  }

  // Column printing putting chars into "display" array
  for (int i = 0; i < window.width; i++)
    for (int j = 0; j < window.height; j++) {
      if (window.height - j <= sort_arr[i % (ARR_SIZE * window.width)])
        display[j][i] = chart.ch;
    }

  // Final display print
  for (int i = 0; i < window.height; i++)
    write(STDOUT_FILENO, display[i], sizeof(char) * window.width);
  // puts(display[i]);
  // printf("%c", display[i][j]);

  free(display);
  return;
}

// Update screen every array change
void update(int *arr) {
  struct winsize w;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &w);

  window.height = w.ws_row;
  window.width = w.ws_col;
  print_chart(arr);

  // Timer
  unsigned int counter = 0;
  while (1) {
    if (counter == FRAME_MS)
      return;
    usleep(1000);
    counter++;
  }
}

int main(int argc, char **argv) {
  struct winsize w;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &w);

  // Initialize
  window.height = w.ws_row;
  window.width = w.ws_col;
  chart.ch = '#';
  chart.n_cols = 0;

  // Init array
  int *sort_arr = (int *)malloc(sizeof(int) * window.width);
  ARR_SIZE = window.height;

  for (int i = 1; i <= ARR_SIZE; i++)
    sort_arr[i] = i;

  srand(time(NULL));
  for (int i = ARR_SIZE - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = sort_arr[i];
    sort_arr[i] = sort_arr[j];
    sort_arr[j] = temp;
  }

  // Handle args
  if (argc != 1) {
    printf("Sorting Algorithm Visualizer\nby Simon Vizner (vizn3r)\n\n");
    printf("Width: %d\nHeight: %d\n", (int)window.width, (int)window.height);
    return 0;
  }

  // Prepare terminal for displaying
  stdin_dis();
  printf("\x1b[2J");
  fflush(stdout);
  setvbuf(stdout, NULL, _IONBF, 0);

  // SORT!!!
  sort(sort_arr, ARR_SIZE);

  printf("Finnished\n");

  return 0;
}
