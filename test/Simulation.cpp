#ifdef SIMULATION_MODULE

// Auxiliary libraries
#include <stdio.h>
#include <string.h>

// Library being tested
#include <Module.h>

const char *EMPTY_MSG = "";

const char **lcdContentUp = &EMPTY_MSG;
const char **lcdContentDown = &EMPTY_MSG;

void setUp(void) {
  lcdContentUp = &EMPTY_MSG;
  lcdContentDown = &EMPTY_MSG;
}

void tearDown(void) {}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  printf("\nLCD:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentUp, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentDown, KNRM);
  printf("%s----------------%s\n\n\n", KWHTBLU, KNRM);

}

int readLevel() {
  return 0;
}

void showHelp() {
  printf("HELP\n");
  printf("m - mode button\n");
  printf("s - set button\n");
  printf("t - timing interrupt\n");
  printf("q - quit\n");
}

void digitalWriteMocked(unsigned char pin, unsigned char value) {}

int main() {

  Module m;

  m.setup();
  m.setFactor(SECS_CYCLE_FACTOR_DEFAULT);
  m.setStdoutWriteFunction(displayLcdMockupFunctionString);
  m.setReadLevelFunction(readLevel);
  m.setDigitalWriteFunction(digitalWriteMocked);

  char c;
  bool keep = true, mode = false, set = false, timing = true, help = true;
  while(keep) {
    c = getchar();
    switch (c) {
      case 'm':
      case 'M':
        mode = true; break;
      case 's':
      case 'S':
        set = true;
        break;
      case 'q':
      case 'Q':
        keep = false;
        break;
      case 't':
      case 'T':
        timing = true;
        break;
      default:
        help = true;
        break;
    }
    if (help) {
      showHelp();
    } else {
      m.loop(mode, set, timing);
    }
    mode = false; set = false; timing = false; help = false;
  }
}

#endif // SIMULATION_MODULE
